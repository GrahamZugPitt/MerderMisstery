
use crate::*;

pub(crate) struct TypecheckPass<'nc> {
	nc: &'nc NameCtx,
	tc: TypeCtx,
}

impl<'nc> TypecheckPass<'nc> {
	/// Runs the typechecking pass on a program. On success, gives a `NameCtx` which
	/// contains everything name-related: the scope tree, symbol table, declarations map,
	/// and use map.
	pub(crate) fn run(prog: &Program, nc: &'nc NameCtx) -> SemResult<TypeCtx> {
		let mut p = Self::new(nc);
		p.add_stdlib();
		p.visit_program(prog)?;
		Ok(p.finish())
	}

	fn new(nc: &'nc NameCtx) -> Self {
		Self {
			nc,
			tc: TypeCtx::new(),
		}
	}

	fn finish(self) -> TypeCtx {
		self.tc
	}

	// -------------------------------------------------------------------------------------------
	// helpers

	fn add_stdlib(&mut self) {
		let g = self.nc.get_globals();

		self.tc.set_sym_type(self.nc.lookup(g, "print_i").unwrap(), Type::new_func(
			vec![Type::new_int()],
			Type::new_void(),
		));
		self.tc.set_sym_type(self.nc.lookup(g, "print_s").unwrap(), Type::new_func(
			vec![Type::new_string()],
			Type::new_void(),
		));
		self.tc.set_sym_type(self.nc.lookup(g, "print_c").unwrap(), Type::new_func(
			vec![Type::new_int()],
			Type::new_void(),
		));
		self.tc.set_sym_type(self.nc.lookup(g, "println_i").unwrap(), Type::new_func(
			vec![Type::new_int()],
			Type::new_void(),
		));
		self.tc.set_sym_type(self.nc.lookup(g, "println_s").unwrap(), Type::new_func(
			vec![Type::new_string()],
			Type::new_void(),
		));
		self.tc.set_sym_type(self.nc.lookup(g, "println_c").unwrap(), Type::new_func(
			vec![Type::new_int()],
			Type::new_void(),
		));
	}

	fn set_decl_type(&mut self, ident: &Ident, ty: Box<Type>) {
		let sym_id = self.nc.sym_declared_by(ident).expect("node is not a declaration");
		self.tc.set_sym_type(sym_id, ty);
	}

	fn check_types_match(&self, node: NodeId, act: &Box<Type>, exp: &Box<Type>, loc: &str)
	-> SemResult<()> {
		if **act != **exp {
			Err(SemError::type_mismatch(node, &exp.to_string(), &act.to_string(), loc))
		} else {
			Ok(())
		}
	}

	// only valid assignment LHSes are variables and field accesses.
	// the object of a field access doesn't matter cause it could be e.g. `f().x`, that's ok.
	fn check_assignment_lhs(&self, dst: &Box<Exp>) -> SemResult<()> {
		let is_ok = match &dst.kind {
			ExpKind::Id(id) => self.nc.symbol(self.nc.sym_used_by(id).unwrap()).is_variable(),
			ExpKind::Field { .. } => true,
			_ => false,
		};

		if is_ok {
			Ok(())
		} else {
			Err(SemError::invalid_lhs(&dst))
		}
	}

	fn get_referenced_symbol(&self, user: &Ident) -> &Symbol {
		self.nc.symbol(self.nc.sym_used_by(user).expect("invalid symref"))
	}

	#[allow(dead_code)]
	fn get_sym_type(&self, sym_id: SymbolId) -> Option<Box<Type>> {
		self.tc.get_sym_type(sym_id)
	}

	#[allow(dead_code)]
	fn function_type_error(&self, e: &Box<Exp>, actual: &Box<Type>) -> SemError {
		SemError::type_mismatch(e.id, "function type", &actual.to_string(), &e.to_string())
	}

	#[allow(dead_code)]
	fn lookup_struct_field(&self, struct_sym_id: SymbolId, name: &Ident) -> SemResult<SymbolId> {
		// must be a struct...
		assert!(self.nc.symbol(struct_sym_id).kind == SymbolKind::Struct);
		let field_scope = self.nc.symbol_scope(struct_sym_id).unwrap();
		if let Some(sym_id) = self.nc.lookup_no_traverse(field_scope, &name.name) {
			Ok(sym_id)
		} else {
			Err(SemError::no_field_named(&name))
		}
	}

	#[allow(dead_code)]
	fn lookup_struct_ctor(&self, struct_sym_id: SymbolId) -> Option<SymbolId> {
		// must be a struct...
		assert!(self.nc.symbol(struct_sym_id).kind == SymbolKind::Struct);
		let field_scope = self.nc.symbol_scope(struct_sym_id).unwrap();
		self.nc.lookup_no_traverse(field_scope, "constructor")
	}

	#[allow(dead_code)]
	fn check_struct_type(&self, exp: &Exp, ty: &Box<Type>) -> SemResult<SymbolId> {
		if let Type::Struct(ret) = **ty {
			Ok(ret)
		} else {
			Err(SemError::type_mismatch(exp.id, "struct type", &ty.to_string(), &exp.to_string()))
		}
	}

	#[allow(dead_code)]
	fn check_func_args(&mut self, exp_id: NodeId, args: &[Box<Exp>], arg_types: &[Box<Type>])
	-> SemResult<()> {
		// check number of args
		if arg_types.len() != args.len() {
			return Err(SemError::wrong_num_args(exp_id, arg_types.len(), args.len()));
		}

		// then check if the args match
		for i in 0 .. arg_types.len() {
			let arg_exp_type = self.visit_exp(&args[i])?;
			self.check_types_match(args[i].id, &arg_exp_type, &arg_types[i],
				&format!("argument {}", i + 1))?;
		}

		Ok(())
	}

	// -------------------------------------------------------------------------------------------
	// Visiting Program, Types, Decls

	fn visit_program(&mut self, prog: &Program) -> SemResult<()> {
		// first pass does global vars and struct fields...
		for d in &prog.items {
			match d.as_ref() {
				Decl::Var(decl)    => self.visit_global_var_decl(decl)?,
				Decl::Struct(decl) => self.visit_struct_decl_fields(decl)?,
				_ => {}
			}
		}

		// then we do functions, struct bases, and struct methods.
		for d in &prog.items {
			match d.as_ref() {
				Decl::Func(decl)   => self.visit_func_decl(decl)?,
				Decl::Struct(decl) => {
					self.visit_struct_decl_base(decl)?;
					self.visit_struct_decl_methods(decl)?;
				}
				_ => {}
			}
		}

		Ok(())
	}

	fn visit_type(&mut self, ty: &Box<AstType>) -> SemResult<Box<Type>> {
		match ty.as_ref() {
			AstType::Void   => Ok(Type::new_void()),
			AstType::Bool   => Ok(Type::new_bool()),
			AstType::Int    => Ok(Type::new_int()),
			AstType::String => Ok(Type::new_string()),

			AstType::Func { args, ret, .. } => {
				let mut new_args = Vec::new();

				for a in args {
					new_args.push(self.visit_type(&a)?);
				}

				let ret = self.visit_type(ret)?;
				Ok(Type::new_func(new_args, ret))
			}

			AstType::Struct(name) => {
				let sym = self.get_referenced_symbol(name);

				if sym.kind == SymbolKind::Struct {
					Ok(Type::new_struct(sym.id))
				} else {
					Err(SemError::not_a_type(&name))
				}
			}
		}
	}

	fn visit_global_var_decl(&mut self, decl: &VarDecl) -> SemResult<()> {
		let ty = match &decl.init.kind {
			ExpKind::IntLit(..)  => Type::new_int(),
			ExpKind::StrLit(..)  => Type::new_string(),
			ExpKind::BoolLit(..) => Type::new_bool(),
			_ => return Err(SemError::non_constant_init(&decl.name)),
		};

		self.set_decl_type(&decl.name, ty);
		Ok(())
	}

	fn visit_local_var_decl(&mut self, decl: &VarDecl) -> SemResult<Box<Type>> {
		let ty = self.visit_exp(&decl.init)?;

		if ty.is_void() {
			return Err(SemError::bad_var_type(&decl.name));
		}

		self.set_decl_type(&decl.name, ty.clone());
		Ok(ty)
	}

	fn visit_field(&mut self, field: &Field) -> SemResult<()> {
		let ty = self.visit_type(&field.ty)?;

		if ty.is_void() {
			return Err(SemError::bad_field_type(&field.name));
		}

		self.set_decl_type(&field.name, ty);
		Ok(())
	}

	fn visit_struct_decl_fields(&mut self, decl: &StructDecl) -> SemResult<()> {
		for field in &decl.fields {
			self.visit_field(field)?;
		}

		Ok(())
	}

	fn visit_struct_decl_base(&mut self, decl: &StructDecl) -> SemResult<()> {
		if let Some(..) = &decl.base {
			// TODO: struct inheritance.
			unimplemented!("struct inheritance");
		}
		Ok(())
	}

	fn visit_struct_decl_methods(&mut self, decl: &StructDecl) -> SemResult<()> {
		for m in &decl.methods {
			self.visit_func_decl(&m)?;
		}

		Ok(())
	}

	fn visit_arg(&mut self, arg: &FuncArg) -> SemResult<Box<Type>> {
		let ty = self.visit_type(&arg.ty)?;
		self.set_decl_type(&arg.name, ty.clone());
		Ok(ty)
	}

	fn visit_func_decl(&mut self, decl: &FuncDecl) -> SemResult<()> {
		if let Some(this) = &decl.this {
			self.visit_arg(this)?;
		}

		let mut arg_types = Vec::new();

		for arg in &decl.args {
			let ty = self.visit_arg(arg)?;

			if ty.is_void() {
				return Err(SemError::bad_arg_type(&arg.name));
			}

			arg_types.push(ty);
		}

		let ret = self.visit_type(&decl.return_type)?;
		self.set_decl_type(&decl.name, Type::new_func(arg_types, ret.clone()));
		self.visit_stmt(&decl.code, &ret)?;
		Ok(())
	}

	// -------------------------------------------------------------------------------------------
	// Visiting Statements

	fn visit_stmt(&mut self, s: &Box<Stmt>, rty: &Box<Type>) -> SemResult<()> {
		use StmtKind::*;

		match &s.kind {
			Block { stmts } => {
				for s in stmts {
					self.visit_stmt(&s, rty)?;
				}
			}

			If { cond, then, else_ } => {
				let cond_ty = self.visit_exp(&cond)?;
				self.check_types_match(cond.id, &cond_ty, &Type::new_bool(), "'if' condition")?;

				self.visit_stmt(&then, rty)?;

				if let Some(e) = else_ {
					self.visit_stmt(&e, rty)?;
				}
			}

			While { cond, code } => {
				let cond_ty = self.visit_exp(&cond)?;
				self.check_types_match(cond.id, &cond_ty, &Type::new_bool(), "'while' condition")?;

				self.visit_stmt(&code, rty)?;
			}

			For { var, hi, code } => {
				let var_ty = self.visit_local_var_decl(&var)?;

				self.check_types_match(var.name.id, &var_ty, &Type::new_int(),
					"'for' loop lower bound")?;

				let hi_ty = self.visit_exp(&hi)?;
				self.check_types_match(hi.id, &hi_ty, &Type::new_int(), "'for' loop upper bound")?;

				self.visit_stmt(&code, rty)?;
			}

			Exp { exp } => {
				let exp_ty = self.visit_exp(&exp)?;
				if !exp_ty.is_void() {
					return Err(SemError::non_void_expr(&s));
				}
			}

			Assign { dst, src } => {
				self.check_assignment_lhs(&dst)?;
				let dst_ty = self.visit_exp(&dst)?;
				let src_ty = self.visit_exp(&src)?;
				self.check_types_match(s.id, &src_ty, &dst_ty, "rhs of assignment")?;
			}

			Return { val } => {
				if let Some(v) = val {
					let val_ty = self.visit_exp(&v)?;
					self.check_types_match(
						s.id, &val_ty, rty, "return value")?;
				} else if !rty.is_void() {
					return Err(SemError::type_mismatch(
						s.id, &Type::new_void().to_string(), &rty.to_string(), "return"));
				}
			}

			Let { decl } => {
				self.visit_local_var_decl(decl)?;
			}
		}

		Ok(())
	}

	// -------------------------------------------------------------------------------------------
	// Visiting Expressions

	fn visit_exp(&mut self, e: &Box<Exp>) -> SemResult<Box<Type>> {
		use ExpKind::*;

		let ty = match &e.kind {
			IntLit(..) =>  Type::new_int(),

			BoolLit(..) =>  Type::new_bool(),

			StrLit(..) =>  Type::new_string(),

			Id(ident) => {
				let ty = self.get_sym_type(self.get_referenced_symbol(ident).id);

				match ty {
					Some(ty) => ty,

					None => return Err(SemError::not_a_value(&ident))
				}

			}

			Field{obj, name} =>{
				let obj_ty = self.visit_exp(obj)?;
				let sym_id = self.check_struct_type(&obj, &obj_ty)?;
				let field = self.lookup_struct_field(sym_id, &name)?;
				let opt = self.get_sym_type(field);
				opt.unwrap()
			}

			Unary{op, lhs} => {
				let lhs_ty = self.visit_exp(lhs)?;
				let op_ty = match op{
					UnOp::Neg => Type::new_int(),

					_ => Type::new_bool()
				};
				self.check_types_match(lhs.id, &lhs_ty, &op_ty, &e.to_string())?;
				op_ty
			}

			Call{callee, args} =>{
				let mut callee_ty = self.visit_exp(callee)?;

				match *callee_ty {
					Type::Func { args: arg_types, ret: ret_type } => {
						self.check_func_args(e.id, &args, &arg_types)?;
						callee_ty = ret_type;
					},
					
					_ => {
						return Err(self.function_type_error(&e, &callee_ty));
					}
				}
				callee_ty
			}

			Ctor{ty, args} => {
				let ty_ty = self.visit_type(ty)?;
				let sym_id = self.check_struct_type(&e, &ty_ty)?;
				let opt = self.lookup_struct_ctor(sym_id);
				let ctor = match opt{
					Some(temp_ctor) => {
						let sym_type = self.get_sym_type(temp_ctor).unwrap();
						match *sym_type{
							Type::Func { args: arg_types, ret: _ret_type } => {
								self.check_func_args(e.id, &args, &arg_types)?;
								ty_ty
							}
							_ => {
								return Err(self.function_type_error(&e, &sym_type));
							}
						}
						},

					None => {if args.len() > 0 {return Err(SemError::wrong_num_args(e.id, 0, args.len()));} 
							ty_ty}
				};
				ctor
			}

			Binary { op, lhs, rhs } => { 
				let lhs_ty = self.visit_exp(lhs)?;
				let rhs_ty = self.visit_exp(rhs)?;
				match op {
					BinOp::Eq | BinOp::NotEq => {
						if lhs_ty.is_void(){
							return Err(SemError::type_mismatch(lhs.id, "non-void type", &lhs_ty.to_string(), &format!("lhs of '{}'", op)))
						}
						self.check_types_match(rhs.id, &rhs_ty, &lhs_ty, &format!("rhs of '{}'", op))?;
						Type::new_bool()
					}
					BinOp::Less | BinOp::Greater | BinOp::LessEq | BinOp::GreaterEq => {
						self.check_types_match(lhs.id, &lhs_ty, &Type::new_int(), &format!("lhs of '{}'", op))?;
						self.check_types_match(rhs.id, &rhs_ty, &lhs_ty, &format!("rhs of '{}'", op))?;
						Type::new_bool()
					}
					BinOp::And | BinOp::Or => {
						self.check_types_match(lhs.id, &lhs_ty, &Type::new_bool(), &format!("lhs of '{}'", op))?;
						self.check_types_match(rhs.id, &rhs_ty, &lhs_ty, &format!("rhs of '{}'", op))?;
						Type::new_bool()
					}

					BinOp::Sub | BinOp::Mul | BinOp::Div | BinOp::Mod | BinOp::Shl | BinOp::Shr => {
						self.check_types_match(lhs.id, &lhs_ty, &Type::new_int(), &format!("lhs of '{}'", op))?;
						self.check_types_match(rhs.id, &rhs_ty, &lhs_ty, &format!("rhs of '{}'", op))?;
						Type::new_int()
					}

					BinOp::Add => {
						if !(lhs_ty.is_int() || lhs_ty.is_string()){
							return Err(SemError::type_mismatch(lhs.id, "'int' or 'string'", &lhs_ty.to_string(), &format!("lhs of '{}'", op)))
						}
						self.check_types_match(rhs.id, &rhs_ty, &lhs_ty, &format!("rhs of '{}'", op))?;
						lhs_ty
					}
				}
			}

		};

		self.tc.set_node_type(e.id, ty.clone());
		Ok(ty)
	}
}