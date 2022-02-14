
use std::fmt::{ Display, Formatter, Result as FmtResult };

// -----------------------------------------------------------------------------------------------
// constants

pub const PTR_SIZE: usize = 4;
pub const PTR_ALIGN: usize = 4;
pub const MAX_ALIGN: usize = 4;
pub const STACK_SLOT_SIZE: usize = 4;

// -----------------------------------------------------------------------------------------------
// Reg

/// All MIPS general-purpose registers.
#[derive(Debug, PartialEq, Eq, Copy, Clone)]
#[repr(u8)]
pub enum Reg {
	Zero, At, V0, V1, A0, A1, A2, A3,
	T0, T1, T2, T3, T4, T5, T6, T7,
	S0, S1, S2, S3, S4, S5, S6, S7,
	T8, T9, K0, K1, Gp, Sp, Fp, Ra,
}

impl Display for Reg {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use Reg::*;

		write!(f, "{}", match self {
			Zero => "zero", At => "at", V0 => "v0", V1 => "v1",
			A0 => "a0", A1 => "a1", A2 => "a2", A3 => "a3",
			T0 => "t0", T1 => "t1", T2 => "t2", T3 => "t3",
			T4 => "t4", T5 => "t5", T6 => "t6", T7 => "t7",
			S0 => "s0", S1 => "s1", S2 => "s2", S3 => "s3",
			S4 => "s4", S5 => "s5", S6 => "s6", S7 => "s7",
			T8 => "t8", T9 => "t9", K0 => "k0", K1 => "k1",
			Gp => "gp", Sp => "sp", Fp => "fp", Ra => "ra",
		})
	}
}

// -----------------------------------------------------------------------------------------------
// DataItemKind

/// Kinds of values that can appear in the .data segment.
#[derive(Debug, PartialEq, Eq, Clone)]
pub enum DataItemKind {
	Bool(bool),
	Int(i32),
	Str(String),
	Ptr(String),
}

impl Display for DataItemKind {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use DataItemKind::*;

		match self {
			Bool(val) => write!(f, ".byte {}", if *val { "1" } else { "0" }),
			Int(val)  => write!(f, ".word {}", val),
			Str(val)  => write!(f, ".asciiz \"{}\"", val.escape_debug()),
			Ptr(name) => write!(f, ".word {}", name),
		}
	}
}

// -----------------------------------------------------------------------------------------------
// DataItem

/// Things that appear in the .data segment. So, global variable declarations.
#[derive(Debug, Clone)]
pub struct DataItem {
	pub name: String,
	pub kind: DataItemKind,
}

impl DataItem {
	pub fn new_bool(name: &str, val: bool) -> Self {
		DataItem { name: name.into(), kind: DataItemKind::Bool(val) }
	}

	pub fn new_int(name: &str, val: i32) -> Self {
		DataItem { name: name.into(), kind: DataItemKind::Int(val) }
	}

	pub fn new_str(name: &str, val: &str) -> Self {
		DataItem { name: name.into(), kind: DataItemKind::Str(val.into()) }
	}

	pub fn new_ptr(name: &str, referent: &str) -> Self {
		DataItem { name: name.into(), kind: DataItemKind::Ptr(referent.into()) }
	}
}

impl Display for DataItem {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		write!(f, "{}: {}", self.name, self.kind)
	}
}

// -----------------------------------------------------------------------------------------------
// DataSeg

/// A representation of the entire .data segment.
#[derive(Debug)]
pub struct DataSeg {
	items:    Vec<DataItem>,
	strlit_n: usize,
}

impl DataSeg {
	pub fn new() -> Self {
		Self {
			items: vec![],
			strlit_n: 0,
		}
	}

	/// add a premade data item.
	pub fn add(&mut self, item: DataItem) {
		self.items.push(item);
	}

	/// add a global bool.
	pub fn add_bool(&mut self, name: &str, val: bool) {
		self.items.push(DataItem::new_bool(name, val));
	}

	/// add a global int.
	pub fn add_int(&mut self, name: &str, val: i32) {
		self.items.push(DataItem::new_int(name, val));
	}

	/// add a global string.
	pub fn add_str(&mut self, name: &str, val: &str) {
		let lit_name = self.add_str_lit(val);
		self.items.push(DataItem::new_ptr(name, &lit_name));
	}

	/// add a string literal. automatically generates a name of the form `STRLIT$n` where
	/// n is a unique number, and returns the generated name.
	pub fn add_str_lit(&mut self, val: &str) -> String {
		let name = format!("STRLIT${}", self.strlit_n);
		self.strlit_n += 1;
		self.items.push(DataItem::new_str(&name, val));
		name
	}
}

impl Display for DataSeg {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		writeln!(f, ".data")?;

		for i in &self.items {
			writeln!(f, "{}", i)?;
		}

		Ok(())
	}
}

// -----------------------------------------------------------------------------------------------
// Inst

/// A MIPS instruction. Only instructions used by the codegen are in this enum.
#[derive(Debug, Clone)]
#[non_exhaustive] // may add more variants in the future
pub enum Inst {
	Add (Reg, Reg, Reg),
	Addi(Reg, Reg, isize),
	And (Reg, Reg, Reg),
	Andi(Reg, Reg, isize),
	Beq (Reg, Reg, String),
	Bge (Reg, Reg, String),
	Bgt (Reg, Reg, String),
	Ble (Reg, Reg, String),
	Blt (Reg, Reg, String),
	Bne (Reg, Reg, String),
	Div (Reg, Reg, Reg),
	J   (String),
	Jal (String),
	Jalr(Reg),
	Jr  (Reg),
	La  (Reg, String),
	Lb  (Reg, isize, Reg),
	Lbn (Reg, String), // load byte, from name
	Li  (Reg, isize),
	Lw  (Reg, isize, Reg),
	Lwn (Reg, String), // load word, from name
	Move(Reg, Reg),
	Mul (Reg, Reg, Reg),
	Neg (Reg, Reg),
	Or  (Reg, Reg, Reg),
	Ori (Reg, Reg, isize),
	Rem (Reg, Reg, Reg),
	Sb  (Reg, isize, Reg),
	Sbn (Reg, String), // store byte, to name
	Seq (Reg, Reg, Reg),
	Sge (Reg, Reg, Reg),
	Sgt (Reg, Reg, Reg),
	Sle (Reg, Reg, Reg),
	Sll (Reg, Reg, isize),
	Sllv(Reg, Reg, Reg),
	Slt (Reg, Reg, Reg),
	Sne (Reg, Reg, Reg),
	Sra (Reg, Reg, isize),
	Srav(Reg, Reg, Reg),
	Sub (Reg, Reg, Reg),
	Sw  (Reg, isize, Reg),
	Swn (Reg, String), // store word, to name
	Syscall,
	Teq (Reg, Reg),
}

impl Display for Inst {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use Inst::*;

		match self {
			Add (a, b, c) => write!(f, "add  {}, {}, {}", a, b, c),
			Addi(a, b, i) => write!(f, "addi {}, {}, {}", a, b, i),
			And (a, b, c) => write!(f, "and  {}, {}, {}", a, b, c),
			Andi(a, b, i) => write!(f, "andi {}, {}, {}", a, b, i),
			Beq (a, b, l) => write!(f, "beq  {}, {}, {}", a, b, l),
			Bge (a, b, l) => write!(f, "bge  {}, {}, {}", a, b, l),
			Bgt (a, b, l) => write!(f, "bgt  {}, {}, {}", a, b, l),
			Ble (a, b, l) => write!(f, "ble  {}, {}, {}", a, b, l),
			Blt (a, b, l) => write!(f, "blt  {}, {}, {}", a, b, l),
			Bne (a, b, l) => write!(f, "bne  {}, {}, {}", a, b, l),
			Div (a, b, c) => write!(f, "div  {}, {}, {}", a, b, c),
			J   (l)       => write!(f, "j    {}", l),
			Jal (l)       => write!(f, "jal  {}", l),
			Jalr(a)       => write!(f, "jalr {}", a),
			Jr  (a)       => write!(f, "jr   {}", a),
			La  (a, l)    => write!(f, "la   {}, {}", a, l),
			Lb  (a, d, b) => write!(f, "lb   {}, {}({})", a, d, b),
			Lbn (a, l)    => write!(f, "lb   {}, {}", a, l),
			Li  (a, i)    => write!(f, "li   {}, {}", a, i),
			Lw  (a, d, b) => write!(f, "lw   {}, {}({})", a, d, b),
			Lwn (a, l)    => write!(f, "lw   {}, {}", a, l),
			Move(a, b)    => write!(f, "move {}, {}", a, b),
			Mul (a, b, c) => write!(f, "mul  {}, {}, {}", a, b, c),
			Neg (a, b)    => write!(f, "neg  {}, {}", a, b),
			Or  (a, b, c) => write!(f, "or   {}, {}, {}", a, b, c),
			Ori (a, b, i) => write!(f, "ori  {}, {}, {}", a, b, i),
			Rem (a, b, c) => write!(f, "rem  {}, {}, {}", a, b, c),
			Sb  (a, d, b) => write!(f, "sb   {}, {}({})", a, d, b),
			Sbn (a, l)    => write!(f, "sb   {}, {}", a, l),
			Seq (a, b, c) => write!(f, "seq  {}, {}, {}", a, b, c),
			Sge (a, b, c) => write!(f, "sge  {}, {}, {}", a, b, c),
			Sgt (a, b, c) => write!(f, "sgt  {}, {}, {}", a, b, c),
			Sle (a, b, c) => write!(f, "sle  {}, {}, {}", a, b, c),
			Sll (a, b, i) => write!(f, "sll  {}, {}, {}", a, b, i),
			Sllv(a, b, c) => write!(f, "sllv {}, {}, {}", a, b, c),
			Slt (a, b, c) => write!(f, "slt  {}, {}, {}", a, b, c),
			Sne (a, b, c) => write!(f, "sne  {}, {}, {}", a, b, c),
			Sra (a, b, i) => write!(f, "sra  {}, {}, {}", a, b, i),
			Srav(a, b, c) => write!(f, "srav {}, {}, {}", a, b, c),
			Sub (a, b, c) => write!(f, "sub  {}, {}, {}", a, b, c),
			Sw  (a, d, b) => write!(f, "sw   {}, {}({})", a, d, b),
			Swn (a, l)    => write!(f, "sw   {}, {}", a, l),
			Syscall       => write!(f, "syscall"),
			Teq (a, b)    => write!(f, "teq  {}, {}", a, b),
		}
	}
}

// -----------------------------------------------------------------------------------------------
// CodeItem

/// A single "line" of code in a function; could be a label, instruction, or comment.
#[derive(Debug, Clone)]
pub enum CodeItem {
	Label(String),
	Inst(Inst, Option<String>),
	Comment(String),
}

impl CodeItem {
	pub fn new_inst(inst: Inst) -> Self {
		Self::Inst(inst, None)
	}

	pub fn new_inst_with_comment(inst: Inst, comm: &str) -> Self {
		Self::Inst(inst, Some(comm.into()))
	}

	pub fn new_label(label: &str) -> Self {
		Self::Label(label.into())
	}

	pub fn new_comment(comm: &str) -> Self {
		Self::Comment(comm.into())
	}
}

impl Display for CodeItem {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use CodeItem::*;

		match &self {
			Label(l)         => writeln!(f, "{}:", l),
			Inst(i, None)    => writeln!(f, "    {}", i),
			Inst(i, Some(c)) => writeln!(f, "    {} # {}", i, c),
			Comment(c)       => writeln!(f, "    # {}", c),
		}
	}
}

// -----------------------------------------------------------------------------------------------
// Function

/// A single function.
#[derive(Debug, Clone)]
pub struct Function {
	name:      String,
	is_global: bool,
	code:      Vec<CodeItem>,
}

impl Function {
	/// Create a new non-global function with empty code.
	pub fn new(name: &str) -> Self {
		Self {
			name:      name.into(),
			is_global: false,
			code:      vec![],
		}
	}

	/// Create a new global function with empty code.
	pub fn new_global(name: &str) -> Self {
		Self {
			name:      name.into(),
			is_global: true,
			code:      vec![],
		}
	}

	/// Get the name.
	pub fn name(&self) -> &str {
		&self.name
	}

	/// Is it global?
	pub fn is_global(&self) -> bool {
		self.is_global
	}

	/// Add an instruction.
	pub fn inst(&mut self, inst: Inst) {
		self.code.push(CodeItem::new_inst(inst));
	}

	/// Add an instruction with a comment on the same line.
	pub fn inst_with_comment(&mut self, inst: Inst, comment: &str) {
		self.code.push(CodeItem::new_inst_with_comment(inst, comment));
	}

	/// Add a label.
	pub fn label(&mut self, label: &str) {
		self.code.push(CodeItem::new_label(label));
	}

	/// Add a comment.
	pub fn comment(&mut self, comment: &str) {
		self.code.push(CodeItem::new_comment(comment));
	}

	/// Appends the given code to this function's code.
	pub fn extend(&mut self, code: &[CodeItem]) {
		self.code.extend(code.iter().cloned());
	}
}

impl Display for Function {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		writeln!(f, "#-----------------------------------------------------------")?;

		if self.is_global {
			writeln!(f, ".globl {}", self.name)?;
		}

		writeln!(f, "{}:", self.name)?;

		for c in &self.code {
			c.fmt(f)?;
		}

		Ok(())
	}
}

// -----------------------------------------------------------------------------------------------
// TextSeg

/// A representation of the entire .text segment.
#[derive(Debug)]
pub struct TextSeg {
	funcs: Vec<Function>,
}

impl TextSeg {
	pub fn new() -> Self {
		Self {
			funcs: vec![],
		}
	}

	pub fn add(&mut self, f: Function) {
		self.funcs.push(f);
	}
}

impl Display for TextSeg {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		writeln!(f, ".text")?;

		for func in &self.funcs {
			func.fmt(f)?;
			writeln!(f)?;
		}

		Ok(())
	}
}