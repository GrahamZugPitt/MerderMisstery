
use crate::*;

pub(crate) struct SymbolsPass {
	b: NameCtxBuilder,
}

impl SymbolsPass {
	/// Runs the symbols pass on a program. On success, gives a `NameCtx` which
	/// contains everything name-related: the scope tree, symbol table, declarations map,
	/// and use map.
	pub(crate) fn run(prog: &Program) -> SemResult<NameCtx> {
		let mut p = Self::new();
		p.add_stdlib()?;
		p.visit_program(prog)?;
		Ok(p.finish())
	}

	fn new() -> Self {
		Self {
			b: NameCtxBuilder::new(),
		}
	}

	fn finish(self) -> NameCtx {
		self.b.finish()
	}

	// the "standard library" is the built-in functions that every program can see.
	// this is cause the language itself has no module/include/import system.
	fn add_stdlib(&mut self) -> SemResult<()> {
		self.b.add_extern_func("print_i")?;
		self.b.add_extern_func("print_s")?;
		self.b.add_extern_func("print_c")?;
		self.b.add_extern_func("println_i")?;
		self.b.add_extern_func("println_s")?;
		self.b.add_extern_func("println_c")?;
		Ok(())
	}

	// -------------------------------------------------------------------------------------------
	// visiting methods

	fn visit_program(&mut self, prog: &Program) -> SemResult<()> {
		// pass 1: just insert all global vars, structs, and function symbols into global scope.
		// this will allow for forward references any program item.
		for d in &prog.items {
			match d.as_ref() {
				Decl::Var(decl)    => self.b.add_var(&decl.name)?,
				Decl::Struct(decl) => self.b.add_struct(&decl.name)?,
				Decl::Func(decl)   => self.b.add_func(&decl.name)?,
			};
		}

		// pass 2: actually visit each thing.
		for d in &prog.items {
			match d.as_ref() {
				Decl::Var(decl)    => self.visit_var_decl(decl)?,
				Decl::Struct(decl) => self.visit_struct_decl(decl)?,
				Decl::Func(decl)   => self.visit_func_decl(decl)?,
			}
		}

		Ok(())
	}

	fn visit_type(&mut self, ty: &Box<AstType>) -> SemResult<()> {
		match ty.as_ref() {
			AstType::Func { args, ret, .. } => {
				for a in args { self.visit_type(&a)?; }
				self.visit_type(&ret)?;
			}

			AstType::Struct(name) => {
				// here's the other place names are resolved: struct type names!
				// like in a function argument (fn f(s: S)) or in a ctor (new S()).
				// we don't check if the name is *actually* referring to a struct
				// until the type checking phase.
				self.b.resolve(name)?;
			}

			_ => {}
		}

		Ok(())
	}

	fn visit_var_decl(&mut self, decl: &VarDecl) -> SemResult<()> {
		// visit the initializer first, so this variable won't appear on the rhs of the =.
		self.visit_exp(&decl.init)?;

		// only add the var if it's not global, cause global vars have already been added.
		if !self.b.is_global() {
			self.b.add_var(&decl.name)?;
		}

		Ok(())
	}

	fn visit_arg(&mut self, arg: &FuncArg) -> SemResult<()> {
		// a function argument *is* a variable.
		self.b.add_var(&arg.name)?;
		self.visit_type(&arg.ty)?;
		Ok(())
	}

	fn visit_func_decl(&mut self, decl: &FuncDecl) -> SemResult<()> {
		self.b.enter_decl(&decl.name);
			if let Some(this) = &decl.this {
				self.visit_arg(this)?;
			}

			for arg in &decl.args {
				self.visit_arg(arg)?;
			}

			self.visit_type(&decl.return_type)?;
			self.visit_block_stmt(&decl.code)?;
		self.b.leave_scope();

		Ok(())
	}

	fn visit_struct_decl(&mut self, decl: &StructDecl) -> SemResult<()> {
		self.b.enter_decl(&decl.name);
			if let Some(..) = &decl.base {
				// TODO: struct inheritance.
				unimplemented!("struct inheritance");
			}

			// have to add the fields and methods to the struct's scope here, cause
			// that wasn't done in the first phase.

			for Field { name, ty } in &decl.fields {
				if name.name == "constructor" {
					return Err(SemError::bad_field_name(name.id, "constructor"));
				}

				self.b.add_field(&name)?;
				self.visit_type(&ty)?;
			}

			for m in &decl.methods {
				self.b.add_func(&m.name)?;
				self.visit_func_decl(&m)?;
			}
		self.b.leave_scope();
		Ok(())
	}

	// this is called on { blocks } which introduce a new scope...
	fn visit_block_scope(&mut self, s: &Box<Stmt>, scope_name: &str) -> SemResult<()> {
		if let StmtKind::Block { .. } = &s.kind {
			self.b.enter_scope(scope_name);
				self.visit_block_stmt(s)?;
			self.b.leave_scope();
			Ok(())
		} else {
			panic!("aaaa '{}' doesn't have a block stmt as its body", scope_name);
		}
	}

	// and this is called to just visit the statements within a { block },
	// without introducing a new scope. this is so that for loops and function decls
	// don't have an extra unnecessary layer of scope. it doesn't affect correctness,
	// it just makes the resulting scope tree cleaner and easier to understand, and
	// the tests easier to write.
	fn visit_block_stmt(&mut self, s: &Box<Stmt>) -> SemResult<()> {
		if let StmtKind::Block { stmts } = &s.kind {
			for s in stmts {
				self.visit_stmt(&s)?;
			}

			Ok(())
		} else {
			panic!("aaaa visit_block_stmt called with something that is not a block");
		}
	}

	fn visit_stmt(&mut self, s: &Box<Stmt>) -> SemResult<()> {
		use StmtKind::*;

		match &s.kind {
			Block { .. } =>
				self.visit_block_scope(&s, &format!("<anonymous block {:?}", s.id))?,

			If { cond, then, else_ } => {
				self.visit_exp(&cond)?;
				self.visit_block_scope(&then, &format!("<if clause of {:?}>", s.id))?;

				if let Some(e) = else_ {
					self.visit_block_scope(&e, &format!("<else clause of {:?}>", s.id))?;
				}
			}

			While { cond, code } => {
				self.visit_exp(&cond)?;
				self.visit_block_scope(&code, &format!("<while loop {:?}>", s.id))?;
			}

			For { var, hi, code } => {
				self.b.enter_scope(&format!("<for loop {:?}>", s.id));
					self.visit_var_decl(&var)?;
					self.visit_exp(&hi)?;
					self.visit_block_stmt(&code)?;
				self.b.leave_scope();
			}

			Exp { exp } => self.visit_exp(&exp)?,

			Assign { dst, src } => {
				self.visit_exp(&dst)?;
				self.visit_exp(&src)?;
			}

			Return { val } => {
				if let Some(v) = val {
					self.visit_exp(&v)?;
				}
			}

			Let { decl } => {
				self.visit_var_decl(decl)?;
			}
		}

		Ok(())
	}

	fn visit_exp(&mut self, e: &Box<Exp>) -> SemResult<()> {
		use ExpKind::*;

		match &e.kind {
			// here it is, really the only thing that needs to be done for expressions:
			// resolve identifiers.
			Id(name) => self.b.resolve(&name)?,

			IntLit(..) | BoolLit(..) | StrLit(..) => {}

			Unary { lhs, .. } => self.visit_exp(&lhs)?,

			Binary { lhs, rhs, .. } => {
				self.visit_exp(&lhs)?;
				self.visit_exp(&rhs)?;
			}

			Call { callee, args } => {
				self.visit_exp(&callee)?;
				for arg in args {
					self.visit_exp(arg)?;
				}
			}

			// field name cannot be resolved until second pass, since we don't know type yet.
			Field { obj, .. } => self.visit_exp(&obj)?,

			Ctor { ty, args } => {
				self.visit_type(ty)?;

				for arg in args {
					self.visit_exp(arg)?;
				}
			}
		}

		Ok(())
	}
}