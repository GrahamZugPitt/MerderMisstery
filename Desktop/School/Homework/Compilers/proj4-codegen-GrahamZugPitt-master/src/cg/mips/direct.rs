
// The "Direct" MIPS code generator, which takes the analyzed AST
// and produces MIPS assembly language directly from it.

use std::fmt::{ Display, Formatter, Result as FmtResult };
use std::collections::{ HashMap };

use crate::*;
use super::arch::*;

// -----------------------------------------------------------------------------------------------
// Constants

// registers used as temporaries, in the order they are allocated.
const TEMP_REGS: &[Reg] = &[
	Reg::S0, Reg::S1, Reg::S2, Reg::S3, Reg::S4, Reg::S5, Reg::S6, Reg::S7,
];

// how many temporary registers are allowed, which complex expressions
// may be able to exceed.
const MAX_TEMP_REGS: usize = TEMP_REGS.len();

// "load/store size" - represents which "flavor" of load/store instructions
// should be used to access a value.
#[derive(Debug, Clone, Copy)]
#[repr(usize)]
enum LdStSize {
	Byte = 1,
	Word = 4,
}

impl From<usize> for LdStSize {
	fn from(s: usize) -> LdStSize {
		match s {
			1 => LdStSize::Byte,
			4 => LdStSize::Word,
			_ => panic!(),
		}
	}
}

// names of runtime functions
const RT_NEW:    &str = "rt$new";
const RT_CONCAT: &str = "rt$concat";

// helper to make the names of methods used in the output code (so Point.len becomes Point$len)
fn make_method_name(struct_name: &str, method_name: &str) -> String {
	format!("{}${}", struct_name, method_name)
}

// -----------------------------------------------------------------------------------------------
// VarLoc

// the location of a variable, usable as the operand of a load/store instruction.
// also includes the size of the load/store needed to access it, for kinds other
// than functions.
#[derive(Debug, Clone)]
enum VarLoc {
	FpPlus(isize, LdStSize),
	Global(String, LdStSize),
	Field(Reg, usize, LdStSize),
	Func(String),
}

impl Display for VarLoc {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use VarLoc::*;

		match &self {
			FpPlus(offs, _)   => write!(f, "{}(fp)", offs),
			Global(name, _)   => write!(f, "{}", name),
			Func(name)        => write!(f, "{}", name),
			Field(r, offs, _) => write!(f, "{}({})", offs, r),
		}
	}
}

// -----------------------------------------------------------------------------------------------
// align_to

fn align_to(offs: usize, alignment: usize) -> usize {
	let mask = alignment - 1;

	if (offs & mask) == 0 {
		offs
	} else {
		(offs & !mask) + alignment
	}
}

#[test]
fn test_align_to() {
	assert_eq!(align_to(0, 4), 0);
	assert_eq!(align_to(1, 4), 4);
	assert_eq!(align_to(2, 4), 4);
	assert_eq!(align_to(3, 4), 4);
	assert_eq!(align_to(4, 4), 4);
	assert_eq!(align_to(5, 4), 8);
	assert_eq!(align_to(6, 4), 8);
	assert_eq!(align_to(7, 4), 8);
	assert_eq!(align_to(8, 4), 8);
}

// -----------------------------------------------------------------------------------------------
// StructLayout, StructLayoutBuilder

struct StructLayout {
	size:    usize,
	offsets: HashMap<SymbolId, (usize, LdStSize)>,
}

impl StructLayout {
	fn size(&self) -> usize {
		self.size
	}

	#[track_caller]
	fn get_field(&self, field_sym_id: SymbolId, reg: Reg) -> VarLoc {
		let (offs, size) = *self.offsets.get(&field_sym_id).unwrap();
		VarLoc::Field(reg, offs, size)
	}
}

// -----------------------------------------------------------------------------------------------
// StructLayoutBuilder

struct StructLayoutBuilder(StructLayout);

impl StructLayoutBuilder {
	fn new() -> Self {
		Self(StructLayout {
			size: 0,
			offsets: HashMap::new(),
		})
	}

	fn finish(mut self) -> StructLayout {
		self.0.size = align_to(self.0.size, MAX_ALIGN);
		self.0
	}

	fn add(&mut self, sym_id: SymbolId, sizeof: usize, alignment: usize) {
		let new_offset = align_to(self.0.size, alignment);
		let new_size = new_offset + sizeof;
		self.0.offsets.insert(sym_id, (new_offset, LdStSize::from(sizeof)));
		self.0.size = new_size;
	}

	fn add_bool(&mut self, sym_id: SymbolId) {
		self.add(sym_id, 1, 1)
	}

	fn add_int(&mut self, sym_id: SymbolId) {
		self.add(sym_id, 4, 4)
	}

	fn add_ref(&mut self, sym_id: SymbolId) {
		self.add(sym_id, PTR_SIZE, PTR_ALIGN)
	}
}

// -----------------------------------------------------------------------------------------------
// StackFrame

struct StackFrame {
	full_size:  usize,
	local_size: usize,
	num_saved:  usize,
	locs:       HashMap<SymbolId, VarLoc>,
}

impl StackFrame {
	fn add_saved_regs(&mut self, n: usize) {
		self.num_saved += n;
		assert!(self.num_saved <= MAX_TEMP_REGS);

		let num_slots = STACK_SLOT_SIZE * n;
		self.full_size  += num_slots;
		self.local_size += num_slots;
	}

	fn get_sym_var_loc(&self, sym_id: SymbolId) -> Option<VarLoc> {
		self.locs.get(&sym_id).cloned()
	}

	// offset (from fp) of the first (lowest) saved register slot.
	fn first_saved_reg_offs(&self) -> isize {
		-(self.local_size as isize)
	}

	fn num_saved_regs(&self) -> usize {
		self.num_saved
	}

	fn local_size(&self) -> usize {
		self.local_size
	}

	fn full_size(&self) -> usize {
		self.full_size
	}

	fn locs_iter(&self) -> impl Iterator<Item = (&SymbolId, &VarLoc)> {
		let mut pairs = self.locs.iter().collect::<Vec<_>>();
		pairs.sort_by(|a, b| {
			use VarLoc::*;
			match (a.1, b.1) {
				(FpPlus(a, _), FpPlus(b, _)) => b.cmp(a),
				_ => unreachable!(),
			}
		});

		pairs.into_iter()
	}

	#[allow(dead_code)]
	fn dump(&self, ctx: &CgCtx) {
		println!("({} bytes total, {} of which are variables/saved registers)",
			self.full_size, self.local_size);

		for (var_id, loc) in self.locs_iter() {
			println!("    {}: {}", loc, ctx.sym(*var_id).name);
		}
	}
}

// -----------------------------------------------------------------------------------------------
// StackFrameBuilder

struct StackFrameBuilder {
	locs: HashMap<SymbolId, VarLoc>,
	num_args: usize,
	num_vars: usize,
}

impl StackFrameBuilder {
	fn new() -> Self {
		Self {
			locs: HashMap::new(),
			num_args:  0,
			num_vars:  0,
		}
	}

	fn finish(self) -> StackFrame {
		// add two slots for ra and saved fp
		let num_local_slots = self.num_vars + 2;
		let num_total_slots = num_local_slots + self.num_args;

		let full_size = STACK_SLOT_SIZE * num_total_slots;
		let local_size = STACK_SLOT_SIZE * num_local_slots;

		StackFrame {
			full_size,
			local_size,
			locs: self.locs,
			num_saved: 0,
		}
	}

	fn add_arg(&mut self, sym_id: SymbolId, size: LdStSize) {
		let loc = VarLoc::FpPlus((STACK_SLOT_SIZE * self.num_args) as isize, size);
		self.num_args += 1;
		let result = self.locs.insert(sym_id, loc);
		assert!(result.is_none(), "duplicate symbol");
	}

	fn add_var(&mut self, sym_id: SymbolId, size: LdStSize) {
		// +3, because -4 is fp, -8 is ra; -12 is where variables start.
		let loc = VarLoc::FpPlus(-((STACK_SLOT_SIZE * (self.num_vars + 3)) as isize), size);
		self.num_vars += 1;
		let result = self.locs.insert(sym_id, loc);
		assert!(result.is_none(), "duplicate symbol");
	}
}

// -----------------------------------------------------------------------------------------------
// StackFrameVisitor

struct StackFrameVisitor<T: FnMut(&VarDecl)>(T);

impl<T: FnMut(&VarDecl)> Visitor for StackFrameVisitor<T> {
	fn visit_var_decl(&mut self, vd: &VarDecl) {
		(self.0)(vd);
	}
}

// -----------------------------------------------------------------------------------------------
// RegAllocator

struct RegAllocator {
	cur_reg:  usize,
	max_regs: usize,
}

impl RegAllocator {
	fn new() -> Self {
		Self {
			cur_reg:  0,
			max_regs: 0,
		}
	}

	fn max_regs(&self) -> usize {
		self.max_regs
	}

	fn cur_reg(&self) -> Reg {
		TEMP_REGS[self.cur_reg]
	}

	fn push_reg(&mut self) -> Reg {
		assert!(self.cur_reg < MAX_TEMP_REGS, "expression complexity too high; out of temp regs");
		let ret = self.cur_reg();
		self.cur_reg += 1;
		// keep track of maximum register depth
		self.max_regs = std::cmp::max(self.max_regs, self.cur_reg);
		ret
	}

	fn pop_reg(&mut self) {
		assert!(self.cur_reg > 0, "temp reg underflow");
		self.cur_reg -= 1;
	}

	fn is_reg_reserved(&self, reg: Reg) -> bool {
		TEMP_REGS[0 .. self.cur_reg].contains(&reg)
	}
}

// -----------------------------------------------------------------------------------------------
// CgFunction

pub struct CgFunction {
	func:  Function,
	frame: StackFrame,
}

// -----------------------------------------------------------------------------------------------
// CgFuncBuilder

struct CgFuncBuilder {
	name:      String,
	code:      Vec<CodeItem>,
	frame:     StackFrame,
	regs:      RegAllocator,

	#[allow(dead_code)]
	label_idx: usize,
}

impl CgFuncBuilder {
	fn new(decl: &FuncDecl, frame: StackFrame) -> Self {
		Self {
			name: decl.name.name.clone(),
			code: vec![],
			frame,
			regs: RegAllocator::new(),
			label_idx: 0,
		}
	}

	fn finish(self, name_prefix: Option<&str>, ctx: &CgCtx) -> CgFunction {
		let mut frame = self.frame;
		frame.add_saved_regs(self.regs.max_regs());

		let name = match name_prefix {
			Some(prefix) => make_method_name(prefix, &self.name),
			None         => self.name,
		};

		let is_main = name == "main";

		let mut func = if is_main {
			Function::new_global(&name)
		} else {
			Function::new(&name)
		};

		func.comment("Stack Frame:");
		func.comment(&format!("  {} bytes total, {} of which are variables/saved registers",
			frame.full_size, frame.local_size));

		for (var_id, loc) in frame.locs_iter() {
			func.comment(&format!("  {}: {}", loc, ctx.sym(*var_id).name));
		}

		add_prologue(&mut func, &frame);
		func.extend(&self.code);
		add_epilogue(&mut func, &frame, is_main);
		CgFunction { func, frame }
	}

	fn get_sym_var_loc(&self, sym_id: SymbolId) -> Option<VarLoc> {
		self.frame.get_sym_var_loc(sym_id)
	}

	// --------------------------------------------------------------------------------------------
	// Register allocation

	fn cur_reg(&self) -> Reg {
		self.regs.cur_reg()
	}

	fn push_reg(&mut self) -> Reg {
		self.regs.push_reg()
	}

	fn pop_reg(&mut self) {
		self.regs.pop_reg()
	}

	fn is_reg_reserved(&self, reg: Reg) -> bool {
		self.regs.is_reg_reserved(reg)
	}

	// --------------------------------------------------------------------------------------------
	// Labels

	#[allow(dead_code)]
	fn make_label_name(&mut self, name: &str) -> String {
		// if using a standard version of MARS that does not have function-local labels,
		// this would have to be, like, format!("_{}_{}_{}", self.name, name, self.label_idx)
		// to ensure a globally-unique label name.
		let name = format!("_{}_{}", name, self.label_idx);
		self.label_idx += 1;
		name
	}

	#[allow(dead_code)]
	fn label(&mut self, name: &str) {
		self.code.push(CodeItem::new_label(name));
	}

	// --------------------------------------------------------------------------------------------
	// Instructions

	fn move_(&mut self, dst: Reg, src: Reg) {
		self.inst(Inst::Move(dst, src));
	}

	#[allow(dead_code)]
	fn jump(&mut self, label: &str) {
		self.inst(Inst::J(label.into()));
	}

	#[allow(dead_code)]
	fn beqz(&mut self, rs: Reg, label: &str) {
		self.inst(Inst::Beq(rs, Reg::Zero, label.into()));
	}

	#[allow(dead_code)]
	fn bnez(&mut self, rs: Reg, label: &str) {
		self.inst(Inst::Bne(rs, Reg::Zero, label.into()));
	}

	fn load_immediate(&mut self, dst: Reg, src: isize) {
		self.inst(Inst::Li(dst, src));
	}

	fn load_address(&mut self, dst: Reg, label: &str) {
		self.inst(Inst::La(dst, label.into()));
	}

	fn jal(&mut self, name: &str) {
		self.inst(Inst::Jal(name.into()));
	}

	#[allow(dead_code)]
	fn jalr(&mut self, reg: Reg) {
		self.inst(Inst::Jalr(reg));
	}

	#[allow(dead_code)]
	fn set_less_than(&mut self, dst: Reg, rs: Reg, rt: Reg) {
		self.inst(Inst::Slt(dst, rs, rt));
	}

	#[allow(dead_code)]
	fn inc(&mut self, reg: Reg) {
 		self.inst(Inst::Addi(reg, reg, 1));
 	}

 	#[allow(dead_code)]
	fn inst_from_unop(&mut self, op: UnOp, rd: Reg, rs: Reg) {
		match op {
			UnOp::Neg => self.inst(Inst::Neg(rd, rs)),
			UnOp::Not => self.inst(Inst::Seq(rd, rs, Reg::Zero)),
		}
	}

	#[allow(dead_code)]
	fn inst_from_binop(&mut self, op: BinOp, rd: Reg, rs: Reg, rt: Reg) {
		match op {
			BinOp::Add       => self.inst(Inst::Add(rd, rs, rt)),
			BinOp::Sub       => self.inst(Inst::Sub(rd, rs, rt)),
			BinOp::Mul       => self.inst(Inst::Mul(rd, rs, rt)),
			BinOp::Div       => self.inst(Inst::Div(rd, rs, rt)),
			BinOp::Mod       => self.inst(Inst::Rem(rd, rs, rt)),
			BinOp::Shl       => self.inst(Inst::Sllv(rd, rs, rt)),
			BinOp::Shr       => self.inst(Inst::Srav(rd, rs, rt)),
			BinOp::Less      => self.inst(Inst::Slt(rd, rs, rt)),
			BinOp::Greater   => self.inst(Inst::Sgt(rd, rs, rt)),
			BinOp::LessEq    => self.inst(Inst::Sle(rd, rs, rt)),
			BinOp::GreaterEq => self.inst(Inst::Sge(rd, rs, rt)),
			BinOp::Eq        => self.inst(Inst::Seq(rd, rs, rt)),
			BinOp::NotEq     => self.inst(Inst::Sne(rd, rs, rt)),

			BinOp::And | BinOp::Or => unreachable!(),
		}
	}

	fn inst(&mut self, inst: Inst) {
		self.code.push(CodeItem::new_inst(inst));
	}

	// --------------------------------------------------------------------------------------------
	// Abstracted loads and stores

	fn load(&mut self, size: LdStSize, dst: Reg, offs: isize, src: Reg) {
		match size {
			LdStSize::Byte => self.inst(Inst::Lb(dst, offs, src)),
			LdStSize::Word => self.inst(Inst::Lw(dst, offs, src)),
		}
	}

	fn load_name(&mut self, size: LdStSize, dst: Reg, name: &str) {
		match size {
			LdStSize::Byte => self.inst(Inst::Lbn(dst, name.into())),
			LdStSize::Word => self.inst(Inst::Lwn(dst, name.into())),
		}
	}

	fn store(&mut self, size: LdStSize, src: Reg, offs: isize, dst: Reg) {
		match size {
			LdStSize::Byte => self.inst(Inst::Sb(src, offs, dst)),
			LdStSize::Word => self.inst(Inst::Sw(src, offs, dst)),
		}
	}

	fn store_name(&mut self, size: LdStSize, src: Reg, name: &str) {
		match size {
			LdStSize::Byte => self.inst(Inst::Sbn(src, name.into())),
			LdStSize::Word => self.inst(Inst::Swn(src, name.into())),
		}
	}

	// --------------------------------------------------------------------------------------------
	// Function call arguments

	fn make_arg_space(&mut self, num_args: usize) {
		if num_args != 0 {
			let num_bytes = STACK_SLOT_SIZE * num_args;
			self.inst(Inst::Addi(Reg::Sp, Reg::Sp, -(num_bytes as isize)));
		}
	}

	fn set_arg(&mut self, arg_i: usize, src_reg: Reg) {
		let offs = STACK_SLOT_SIZE * arg_i;
		self.inst(Inst::Sw(src_reg, offs as isize, Reg::Sp));
	}
}

fn add_prologue(func: &mut Function, frame: &StackFrame) {
	use Inst::*;
	use Reg::*;

	let local_size = frame.local_size() as isize;

	func.inst(Sw(Fp, -4, Sp));
	func.inst(Sw(Ra, -8, Sp));
	func.inst(Move(Fp, Sp));
	func.inst(Addi(Sp, Sp, -local_size));

	let mut offs = frame.first_saved_reg_offs();

	for i in 0 .. frame.num_saved_regs() {
		func.inst(Sw(TEMP_REGS[i], offs, Fp));
		offs += STACK_SLOT_SIZE as isize;
	}

	func.comment("------------------------------------------------");
}

fn add_epilogue(func: &mut Function, frame: &StackFrame, is_main: bool) {
	use Inst::*;
	use Reg::*;

	func.comment("------------------------------------------------");
	func.label("_return");

	let full_size = frame.full_size() as isize;
	let mut offs = frame.first_saved_reg_offs();

	for i in 0 .. frame.num_saved_regs() {
		func.inst(Lw(TEMP_REGS[i], offs, Fp));
		offs += STACK_SLOT_SIZE as isize;
	}

	func.inst(Lw(Ra, -8, Fp));
	func.inst(Lw(Fp, -4, Fp));
	func.inst(Addi(Sp, Sp, full_size));

	if is_main {
		func.inst(Li(V0, 10));
		func.inst(Syscall);
	} else {
		func.inst(Jr(Ra));
	}
}

// -----------------------------------------------------------------------------------------------
// DirectCg

pub struct DirectCg<'ctx> {
	ctx:            CgCtx<'ctx>,
	funcs:          Vec<CgFunction>,
	data:           DataSeg,
	struct_layouts: HashMap<SymbolId, StructLayout>,
}

enum ExpOrReg<'a> {
	Exp(&'a Box<Exp>),
	Reg(Reg, &'a Box<Type>),
}

impl<'ctx> DirectCg<'ctx> {
	pub(crate) fn run(prog: &Program, nc: &'ctx NameCtx, tc: &'ctx TypeCtx) -> (TextSeg, DataSeg) {
		let mut cg = Self::new(CgCtx::new(nc, tc));
		cg.visit_program(prog);
		cg.finish()
	}

	fn new(ctx: CgCtx<'ctx>) -> Self {
		Self {
			ctx,
			funcs:          vec![],
			data:           DataSeg::new(),
			struct_layouts: HashMap::new(),
		}
	}

	fn finish(mut self) -> (TextSeg, DataSeg) {
		// self.dump();

		let mut text = self.build_text_seg();
		add_stdlib(&mut text, &mut self.data);
		(text, self.data)
	}

	fn build_text_seg(&self) -> TextSeg {
		let mut text = TextSeg::new();

		for f in &self.funcs {
			text.add(f.func.clone());
		}

		text
	}

	#[allow(dead_code)]
	fn dump(&self) {
		if !self.struct_layouts.is_empty() {
			println!("Struct layouts:");

			for (sym_id, layout) in self.struct_layouts.iter() {
				println!("  struct {} ({} bytes)", self.ctx.sym(*sym_id).name, layout.size);

				let mut pairs = layout.offsets.iter().collect::<Vec<_>>();
				pairs.sort_by(|a, b| a.1.0.cmp(&b.1.0));

				for (field_id, (offset, _)) in pairs.into_iter() {
					println!("    {}: {}", self.ctx.sym(*field_id).name, offset);
				}

				println!();
			}
		}

		if !self.funcs.is_empty() {
			println!("Stack frames:");

			for func in self.funcs.iter() {
				print!("  fn {} ", func.func.name());
				func.frame.dump(&self.ctx);
				println!();
			}
		}
	}

	// --------------------------------------------------------------------------------------------
	// Struct layouts

	fn build_struct_layouts(&mut self, prog: &Program) {
		for d in &prog.items {
			if let Decl::Struct(sd) = d.as_ref() {
				self.build_struct_layout(sd);
			}
		}
	}

	fn build_struct_layout(&mut self, decl: &StructDecl) {
		// TODO:
		assert!(decl.base.is_none(), "struct inheritance");

		let struct_sym_id = self.ctx.sym_declared_by(&decl.name);

		let mut b = StructLayoutBuilder::new();

		for field in &decl.fields {
			let field_sym_id = self.ctx.sym_declared_by(&field.name);
			let field_ty = self.ctx.sym_type(field_sym_id);

			match field_ty.as_ref() {
				Type::Bool => b.add_bool(field_sym_id),
				Type::Int  => b.add_int(field_sym_id),
				_          => b.add_ref(field_sym_id),
			}
		}

		self.struct_layouts.insert(struct_sym_id, b.finish());
	}

	#[track_caller]
	fn get_struct_layout(&self, struct_sym_id: SymbolId) -> &StructLayout {
		match self.struct_layouts.get(&struct_sym_id) {
			Some(ret) => ret,
			None => panic!("struct {} did not have a struct layout generated for it",
				self.ctx.sym_name(struct_sym_id)),
		}
	}

	// --------------------------------------------------------------------------------------------
	// Stack frame building

	fn build_stack_frame(&mut self, decl: &FuncDecl) -> StackFrame {
		let mut b = StackFrameBuilder::new();

		// arguments
		if let Some(this) = &decl.this {
			b.add_arg(self.ctx.sym_declared_by(&this.name), LdStSize::Word);
		}

		for arg in &decl.args {
			let sym_id = self.ctx.sym_declared_by(&arg.name);
			let size = self.get_load_store_size(sym_id);
			b.add_arg(sym_id, size);
		}

		// locals
		StackFrameVisitor(|vd| {
			let sym_id = self.ctx.sym_declared_by(&vd.name);
			let size = self.get_load_store_size(sym_id);
			b.add_var(sym_id, size);
		}).visit_func_decl(decl);

		b.finish()
	}

	// --------------------------------------------------------------------------------------------
	// Codegen helpers

	#[allow(dead_code)]
	fn get_sym_var_loc(&self, id: &Ident, b: &CgFuncBuilder) -> VarLoc {
		use SymbolKind::*;

		let sym_id = self.ctx.sym_used_by(id);
		let sym = self.ctx.sym(sym_id);

		match sym.kind {
			LocalVar =>
				b.get_sym_var_loc(sym_id).expect("local variable was not added to stack frame"),

			GlobalVar | ExternVar => {
				let size = self.get_load_store_size(sym_id);
				VarLoc::Global(sym.name.clone(), size)
			}

			Function | ExternFunc =>
				VarLoc::Func(sym.name.clone()),

			Field => unreachable!("use get_field_var_loc for fields"),

			Struct => unreachable!(),
		}
	}

	#[allow(dead_code)]
	#[track_caller]
	fn get_field_var_loc(&self, obj: &Box<Exp>, name: &Ident, reg: Reg) -> VarLoc {
		// yes, we're having to re-look-up fields, like we did in the typechecking
		// pass. this is because the name context never got this info added to it.
		// but, we're guaranteed that these lookups will succeed.
		let struct_sym_id = self.get_struct_sym_id(obj);
		assert!(self.ctx.sym(struct_sym_id).kind == SymbolKind::Struct);
		let field_sym_id = self.ctx.lookup_struct_field(struct_sym_id, &name.name).unwrap();
		self.get_struct_layout(struct_sym_id).get_field(field_sym_id, reg)
	}

	#[allow(dead_code)]
	fn get_struct_method_name(&self, obj: &ExpOrReg, name: &str) -> String {
		let struct_name = match obj {
			ExpOrReg::Exp(obj)   => self.ctx.sym_name(self.get_struct_sym_id(obj)),
			ExpOrReg::Reg(_, ty) => self.ctx.sym_name(ty.symbol_id()),
		};

		make_method_name(struct_name, name)
	}

	#[allow(dead_code)]
	fn get_load_store_size(&self, sym_id: SymbolId) -> LdStSize {
		match self.ctx.sym_type(sym_id).as_ref() {
			Type::Bool => LdStSize::Byte,
			_          => LdStSize::Word,
		}
	}

	#[allow(dead_code)]
	fn loc_into_reg(&self, dst: Reg, loc: VarLoc, b: &mut CgFuncBuilder) {
		use VarLoc::*;

		match loc {
			FpPlus(offs, size)     => b.load(size, dst, offs, Reg::Fp),
			Global(name, size)     => b.load_name(size, dst, &name),
			Func(name)             => b.load_address(dst, &name),
			Field(reg, offs, size) => b.load(size, dst, offs as isize, reg),
		}
	}

	#[allow(dead_code)]
	fn reg_into_loc(&self, loc: VarLoc, src: Reg, b: &mut CgFuncBuilder) {
		use VarLoc::*;

		match loc {
			FpPlus(offs, size)     => b.store(size, src, offs, Reg::Fp),
			Global(name, size)     => b.store_name(size, src, &name),
			// semantic analysis ensures lhs of assignment can't be a function symbol
			Func(..)               => unreachable!(),
			Field(reg, offs, size) => b.store(size, src, offs as isize, reg),
		}
	}

	#[allow(dead_code)]
	fn get_struct_sym_id(&self, e: &Box<Exp>) -> SymbolId {
		self.ctx.node_type(e.id).symbol_id()
	}

	#[allow(dead_code)]
	fn struct_has_constructor(&self, struct_sym_id: SymbolId) -> bool {
		self.ctx.lookup_struct_field(struct_sym_id, "constructor").is_some()
	}

	#[allow(dead_code)]
	fn visit_args(&mut self, this: Option<ExpOrReg>, args: &Vec<Box<Exp>>, b: &mut CgFuncBuilder) {
		let num_args = this.iter().count() + args.len();
		b.make_arg_space(num_args);

		// first do the 'this' argument, if it exists.
		let arg_offset = if let Some(this) = this {
			let reg = match this {
				ExpOrReg::Exp(this)   => self.visit_exp(this, b),
				ExpOrReg::Reg(reg, _) => reg,
			};

			b.set_arg(0, reg);
			1
		} else {
			0
		};

		// then do the rest of the arguments.
		for (i, arg) in args.iter().enumerate() {
			let reg = self.visit_exp(arg, b);
			b.set_arg(i + arg_offset, reg);
		}
	}

	// generate a function call to the `rt$new` runtime function
	#[allow(dead_code)]
	fn call_rt_new(&mut self, dst: Reg, size: usize, b: &mut CgFuncBuilder) {
		b.make_arg_space(1);
		b.load_immediate(dst, size as isize);
		b.set_arg(0, dst);
		b.jal(RT_NEW);
		b.move_(dst, Reg::V0);
	}

	#[allow(dead_code)]
	fn call_rt_concat(&mut self, dst: Reg, lhs: &Box<Exp>, rhs: &Box<Exp>, b: &mut CgFuncBuilder) {
		b.make_arg_space(2);
		let reg = self.visit_exp(lhs, b);
		b.set_arg(0, reg);
		let reg = self.visit_exp(rhs, b);
		b.set_arg(1, reg);
		b.jal(RT_CONCAT);
		b.move_(dst, Reg::V0);
	}

	// --------------------------------------------------------------------------------------------
	// Program and declaration visiting

	fn visit_program(&mut self, prog: &Program) {
		self.build_struct_layouts(prog);
		self.generate_code(prog);
	}

	fn generate_code(&mut self, prog: &Program) {
		for d in &prog.items {
			match d.as_ref() {
				Decl::Var(vd)    => self.visit_global_var_decl(vd),
				Decl::Func(fd)   => self.visit_func_decl(fd, None),
				Decl::Struct(sd) => self.visit_struct_decl(sd),
			}
		}
	}

	fn visit_global_var_decl(&mut self, decl: &VarDecl) {
		let sym_id = self.ctx.sym_declared_by(&decl.name);
		let sym = self.ctx.sym(sym_id);

		match &decl.init.kind {
			ExpKind::IntLit(val)  => self.data.add_int(&sym.name, *val as i32),
			ExpKind::StrLit(val)  => self.data.add_str(&sym.name, val),
			ExpKind::BoolLit(val) => self.data.add_bool(&sym.name, *val),
			_ => unreachable!(),
		}
	}

	#[allow(dead_code)]
	fn visit_local_var_decl(&mut self, decl: &VarDecl, b: &mut CgFuncBuilder) -> VarLoc {
		let src_reg = self.visit_exp(&decl.init, b);
		let dst_id  = self.ctx.sym_declared_by(&decl.name);
		let dst_loc = b.get_sym_var_loc(dst_id).unwrap();
		self.reg_into_loc(dst_loc.clone(), src_reg, b);
		dst_loc
	}

	fn visit_struct_decl(&mut self, decl: &StructDecl) {
		for m in &decl.methods {
			self.visit_func_decl(m, Some(&decl.name.name));
		}
	}

	fn visit_func_decl(&mut self, decl: &FuncDecl, name_prefix: Option<&str>) {
		let frame = self.build_stack_frame(decl);
		let mut b = CgFuncBuilder::new(decl, frame);
		self.visit_stmt(&decl.code, &mut b);
		self.funcs.push(b.finish(name_prefix, &self.ctx));
	}

	// --------------------------------------------------------------------------------------------
	// Statement and expression visiting

	fn visit_stmt(&mut self, s: &Box<Stmt>, b: &mut CgFuncBuilder) {
		use StmtKind::*;

		match &s.kind {
			Block { stmts } => {
				for s in stmts {
					self.visit_stmt(s, b);
				}
			}

			Exp { exp } => {
				 self.visit_exp_no_result(exp, b);
			}

			Let { decl } => {
				self.visit_local_var_decl(&decl, b);
			}

			Return { val } => {
				if let Some(v) = val{
					let reg = self.visit_exp(v, b);
					b.move_(Reg::V0, reg);
				}
				b.jump("_return");
			}

			Assign { dst, src } => {
				match &dst.kind {
					ExpKind::Id(ident) => {
						let reg = self.visit_exp(src, b);
						let loc = self.get_sym_var_loc(ident, b);
						self.reg_into_loc(loc, reg, b);
					}

					ExpKind::Field { obj, name } => {
						let reg = self.visit_exp_no_free(obj, b);
						let reg_2 = self.visit_exp(src, b);
						let loc = self.get_field_var_loc(obj, name, reg);
						self.reg_into_loc(loc, reg_2, b);
						b.pop_reg();
					}

					_ => unreachable!()
				}
			}

			While { cond, code } => {
				let start_l = b.make_label_name("while_cond");
				let end_l = b.make_label_name("while_end");
				b.label(&start_l);
				let reg = self.visit_exp(cond, b);
				b.beqz(reg, &end_l);
				self.visit_stmt(code, b);
				b.jump(&start_l);
				b.label(&end_l);
				
			}

			If { cond, then, else_ } => {
				match else_{
					Some(else_code) =>{
						let end_else = b.make_label_name("end_else");
						let start_else = b.make_label_name("start_else");
						let reg = self.visit_exp(cond, b);
						b.beqz(reg, &start_else);
						self.visit_stmt(then, b);
						b.jump(&end_else);
						b.label(&start_else);
						self.visit_stmt(else_code, b);
						b.label(&end_else);
					}
					_ =>{
						let end_l = b.make_label_name("if_end");
						let reg = self.visit_exp(cond, b);
						b.beqz(reg, &end_l);
						self.visit_stmt(then, b);
						b.label(&end_l);
					}
				}
			}

			For { var, hi, code } => {
				let varloc = self.visit_local_var_decl(&var, b);
				let hi_reg = self.visit_exp_no_free(hi, b);
				let start_l = b.make_label_name("for_cond");
				let end_l = b.make_label_name("for_end");
				b.label(&start_l);
				let reg = b.cur_reg();
				self.loc_into_reg(reg, varloc.clone(), b);
				b.set_less_than(reg, reg, hi_reg);
				b.beqz(reg, &end_l);
				self.visit_stmt(code, b);
				let reg_2 = b.cur_reg();
				self.loc_into_reg(reg_2, varloc.clone(), b);
				b.inc(reg_2);
				self.reg_into_loc(varloc, reg_2, b);
				b.jump(&start_l);
				b.label(&end_l);			
				b.pop_reg();
				
			}
		}
	}

	/// allocates a new register, evaluates the expression into it, frees that register,
	/// then returns the register.
	#[allow(dead_code)]
	fn visit_exp(&mut self, e: &Box<Exp>, b: &mut CgFuncBuilder) -> Reg {
		let dst = b.push_reg();
		self.visit_exp_into_reg(e, dst, b);
		b.pop_reg();
		assert_eq!(dst, b.cur_reg(), "register stack is not equal");
		dst
	}

	/// allocates a new register, evaluates the expression into it, then returns the register
	/// *without* freeing it.
	#[allow(dead_code)]
	fn visit_exp_no_free(&mut self, e: &Box<Exp>, b: &mut CgFuncBuilder) -> Reg {
		let dst = b.push_reg();
		self.visit_exp_into_reg(e, dst, b);
		dst
	}

	/// evaluates the expression without allocating a register. can only be used for evaluating
	/// expressions with void type (so, function calls)
	#[allow(dead_code)]
	fn visit_exp_no_result(&mut self, e: &Box<Exp>, b: &mut CgFuncBuilder) {
		assert!(self.ctx.node_type(e.id).is_void());
		self.visit_exp_into_reg(e, b.cur_reg(), b);
	}

	fn visit_exp_into_reg(&mut self, e: &Box<Exp>, dst: Reg, b: &mut CgFuncBuilder) {
		use ExpKind::*;

		match &e.kind {
			IntLit(val) => {
				b.load_immediate(dst, *val as isize);
			}

			BoolLit(val) => {
				b.load_immediate(dst, *val as isize);
			}

			StrLit(val) => {
				let da_string = self.data.add_str_lit(val);
				b.load_address(dst, &da_string);
			}

			Id(ident) => {
				let loc = self.get_sym_var_loc(ident, b);
				self.loc_into_reg(dst, loc, b);
			}

			Field { obj, name } => {
				self.visit_exp_into_reg(obj, dst, b);
				let loc = self.get_field_var_loc(obj, name, dst);
				self.loc_into_reg(dst, loc, b);
			}

			Unary { op, lhs } => {
				self.visit_exp_into_reg(lhs, dst, b);
				b.inst_from_unop(*op, dst, dst);
			}

			Binary { op: BinOp::And, lhs, rhs } => {
				let three_branch = b.make_label_name("three_branch");
				self.visit_exp_into_reg(lhs, dst, b);
				b.beqz(dst, &three_branch);
				self.visit_exp_into_reg(rhs, dst, b);
				b.label(&three_branch);
				
			}

			Binary { op: BinOp::Or, lhs, rhs } => {
				let three_branch = b.make_label_name("three_branch");
				self.visit_exp_into_reg(lhs, dst, b);
				b.bnez(dst, &three_branch);
				self.visit_exp_into_reg(rhs, dst, b);
				b.label(&three_branch);
			}

			Binary { op, lhs, rhs } => {
				if *op == BinOp::Add && self.ctx.node_type(e.id).is_string() {
			    		self.call_rt_concat(dst, lhs, rhs, b);
				} else {
					self.visit_exp_into_reg(lhs, dst, b);
					let reg = self.visit_exp(rhs, b);
					b.inst_from_binop(*op, dst, dst, reg);
				}
			}

			Call { callee, args } => {
				match &callee.kind {
					Id(name) => {
						let sym = self.ctx.sym(self.ctx.sym_used_by(name));

						match sym.kind {
							SymbolKind::Function | SymbolKind::ExternFunc =>
								self.direct_call(&name.name, args, b),
							_ =>
								self.indirect_call(callee, args, b),
						}
					}
					Field { obj, name } =>
						self.method_call(ExpOrReg::Exp(obj), &name.name, args, b),
					_ =>
						self.indirect_call(callee, args, b),
				}

				if !self.ctx.node_type(e.id).is_void() {
					b.move_(dst, Reg::V0);
				}
			}

			Ctor { args, .. } => {
				let struct_sym_id = self.get_struct_sym_id(&e);
				let size = self.get_struct_layout(struct_sym_id).size();

				self.call_rt_new(dst, size, b);

				if self.struct_has_constructor(struct_sym_id) {
					// dst *should* be a reserved reg here. the only way it can't
					// be is in visit_exp_no_result, which only happens for function
					// calls. but let's be sure.
					assert!(b.is_reg_reserved(dst));
					let ty = self.ctx.node_type(e.id);
					self.method_call(ExpOrReg::Reg(dst, &ty), "constructor", args, b);
				}
			}
		}
	}

	// simplest kind: just `jal` to a function.
	fn direct_call(&mut self, name: &str, args: &Vec<Box<Exp>>, b: &mut CgFuncBuilder) {
		self.visit_args(None, args, b);
		b.jal(name);
	}

	// more complex: have to evaluate callee into a register, then `jalr` it.
	fn indirect_call(&mut self, callee: &Box<Exp>, args: &Vec<Box<Exp>>, b: &mut CgFuncBuilder) {
		let callee_reg = self.visit_exp_no_free(callee, b);
		self.visit_args(None, args, b);
		b.jalr(callee_reg);
		b.pop_reg();
		
	}

	// since we're only doing static dispatch, this is almost the same as a regular
	// function call, with the addition of the extra "this" argument at the front.
	fn method_call(&mut self, obj: ExpOrReg, name: &str, args: &Vec<Box<Exp>>,
	b: &mut CgFuncBuilder) {
		let meth_name = self.get_struct_method_name(&obj, name);
		self.visit_args(Some(obj), args, b);
		b.jal(&meth_name);
	}
}
