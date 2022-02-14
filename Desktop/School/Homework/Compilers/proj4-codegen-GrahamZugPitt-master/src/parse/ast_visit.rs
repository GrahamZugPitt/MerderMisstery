
use crate::*;

pub trait Visitor: Sized {
	fn visit_ident       (&mut self, _id: &Ident)       { /* nothing to do here */      }
	fn visit_type        (&mut self, ty: &Box<AstType>) { walk::type_(self, ty);        }
	fn visit_program     (&mut self, prog: &Program)    { walk::program(self, prog);    }
	fn visit_decl        (&mut self, decl: &Box<Decl>)  { walk::decl(self, decl);       }
	fn visit_func_decl   (&mut self, fd: &FuncDecl)     { walk::func_decl(self, fd);    }
	fn visit_func_arg    (&mut self, fa: &FuncArg)      { walk::func_arg(self, fa);     }
	fn visit_struct_decl (&mut self, sd: &StructDecl)   { walk::struct_decl(self, sd);  }
	fn visit_struct_field(&mut self, sf: &Field)        { walk::struct_field(self, sf); }
	fn visit_var_decl    (&mut self, vd: &VarDecl)      { walk::var_decl(self, vd);     }
	fn visit_stmt        (&mut self, stmt: &Box<Stmt>)  { walk::stmt(self, stmt);       }
	fn visit_exp         (&mut self, exp: &Box<Exp>)    { walk::exp(self, exp);         }
}

// Functions that do the default actions of walking over children of AST nodes.
// Implementors of Visitor aren't required to use these, but can if they like.
pub mod walk {
	use super::*;

	pub fn type_(v: &mut impl Visitor, ty: &Box<AstType>) {
		use AstType::*;

		match ty.as_ref() {
			Func { args, ret, .. } => {
				for a in args { v.visit_type(a); }
				v.visit_type(ret);
			}
			Struct(id) => v.visit_ident(id),

			// no other type has any internal structure
			_ => {}
		}
	}

	pub fn program(v: &mut impl Visitor, prog: &Program) {
		for i in &prog.items { v.visit_decl(&i); }
	}

	pub fn decl(v: &mut impl Visitor, decl: &Box<Decl>) {
		match decl.as_ref() {
			Decl::Func(fd)   => v.visit_func_decl(fd),
			Decl::Struct(sd) => v.visit_struct_decl(sd),
			Decl::Var(vd)    => v.visit_var_decl(vd),
		}
	}

	pub fn func_decl(v: &mut impl Visitor, fd: &FuncDecl) {
		v.visit_ident(&fd.name);
		if let Some(arg) = &fd.this { v.visit_func_arg(&arg); }
		for arg in &fd.args { v.visit_func_arg(&arg); }
		v.visit_type(&fd.return_type);
		v.visit_stmt(&fd.code);
	}

	pub fn func_arg(v: &mut impl Visitor, fa: &FuncArg) {
		v.visit_ident(&fa.name);
		v.visit_type(&fa.ty);
	}

	pub fn struct_decl(v: &mut impl Visitor, sd: &StructDecl) {
		v.visit_ident(&sd.name);
		if let Some(base) = &sd.base { v.visit_type(&base); }
		for f in &sd.fields { v.visit_struct_field(&f); }
		for m in &sd.methods { v.visit_func_decl(&m); }
	}

	pub fn struct_field(v: &mut impl Visitor, sf: &Field) {
		v.visit_ident(&sf.name);
		v.visit_type(&sf.ty);
	}

	pub fn var_decl(v: &mut impl Visitor, vd: &VarDecl) {
		v.visit_ident(&vd.name);
		v.visit_exp(&vd.init);
	}

	pub fn exp(v: &mut impl Visitor, exp: &Box<Exp>) {
		use ExpKind::*;

		match &exp.as_ref().kind {
			Id(..) | IntLit(..) | BoolLit(..) | StrLit(..) => {}

			Unary { lhs, .. } => v.visit_exp(&lhs),
			Binary { lhs, rhs, .. } => {
				v.visit_exp(&lhs);
				v.visit_exp(&rhs);
			}
			Call { callee, args } => {
				v.visit_exp(&callee);
				for a in args { v.visit_exp(&a); }
			}
			Field { obj, name } => {
				v.visit_exp(&obj);
				v.visit_ident(&name);
			}
			Ctor { ty, args } => {
				v.visit_type(&ty);
				for a in args { v.visit_exp(&a); }
			}
		}
	}

	pub fn stmt(v: &mut impl Visitor, stmt: &Box<Stmt>) {
		use StmtKind::*;

		match &stmt.as_ref().kind {
			Block { stmts } => {
				for s in stmts { v.visit_stmt(&s); }
			}
			If { cond, then, else_ } => {
				v.visit_exp(&cond);
				v.visit_stmt(&then);
				if let Some(else_) = &else_ { v.visit_stmt(&else_); }
			}
			While { cond, code } => {
				v.visit_exp(&cond);
				v.visit_stmt(&code);
			}
			For { var, hi, code } => {
				v.visit_var_decl(&var);
				v.visit_exp(&hi);
				v.visit_stmt(&code);
			}
			Exp { exp } => v.visit_exp(&exp),
			Assign { dst, src } => {
				v.visit_exp(&dst);
				v.visit_exp(&src);
			}
			Return { val } => {
				if let Some(val) = &val { v.visit_exp(&val); }
			}
			Let { decl } => v.visit_var_decl(decl),
		}
	}
}

pub trait ResultVisitor: Sized {
	type E;

	fn visit_ident(&mut self, _id: &Ident) -> Result<(), Self::E> {
		Ok(())
	}

	fn visit_type(&mut self, ty: &Box<AstType>) -> Result<(), Self::E> {
		walk_res::type_(self, ty)
	}

	fn visit_program(&mut self, prog: &Program) -> Result<(), Self::E> {
		walk_res::program(self, prog)
	}

	fn visit_decl(&mut self, decl: &Box<Decl>) -> Result<(), Self::E> {
		walk_res::decl(self, decl)
	}

	fn visit_func_decl(&mut self, fd: &FuncDecl) -> Result<(), Self::E> {
		walk_res::func_decl(self, fd)
	}

	fn visit_func_arg(&mut self, fa: &FuncArg) -> Result<(), Self::E> {
		walk_res::func_arg(self, fa)
	}

	fn visit_struct_decl(&mut self, sd: &StructDecl) -> Result<(), Self::E> {
		walk_res::struct_decl(self, sd)
	}

	fn visit_struct_field(&mut self, sf: &Field) -> Result<(), Self::E> {
		walk_res::struct_field(self, sf)
	}

	fn visit_var_decl(&mut self, vd: &VarDecl) -> Result<(), Self::E> {
		walk_res::var_decl(self, vd)
	}

	fn visit_stmt(&mut self, stmt: &Box<Stmt>) -> Result<(), Self::E> {
		walk_res::stmt(self, stmt)
	}

	fn visit_exp(&mut self, exp: &Box<Exp>) -> Result<(), Self::E> {
		walk_res::exp(self, exp)
	}
}

// Functions that do the default actions of walking over children of AST nodes.
// Implementors of Visitor aren't required to use these, but can if they like.
pub mod walk_res {
	use super::*;

	pub fn type_<Error>(v: &mut impl ResultVisitor<E = Error>, ty: &Box<AstType>)
	-> Result<(), Error>{
		use AstType::*;

		match ty.as_ref() {
			Func { args, ret, .. } => {
				for a in args { v.visit_type(a)?; }
				v.visit_type(ret)?;
			}
			Struct(id) => v.visit_ident(id)?,

			// no other type has any internal structure
			_ => {}
		}

		Ok(())
	}

	pub fn program<Error>(v: &mut impl ResultVisitor<E = Error>, prog: &Program)
	-> Result<(), Error>{
		for i in &prog.items { v.visit_decl(&i)?; }
		Ok(())
	}

	pub fn decl<Error>(v: &mut impl ResultVisitor<E = Error>, decl: &Box<Decl>)
	-> Result<(), Error>{
		match decl.as_ref() {
			Decl::Func(fd)   => v.visit_func_decl(fd)?,
			Decl::Struct(sd) => v.visit_struct_decl(sd)?,
			Decl::Var(vd)    => v.visit_var_decl(vd)?,
		}

		Ok(())
	}

	pub fn func_decl<Error>(v: &mut impl ResultVisitor<E = Error>, fd: &FuncDecl)
	-> Result<(), Error>{
		v.visit_ident(&fd.name)?;
		if let Some(arg) = &fd.this { v.visit_func_arg(&arg)?; }
		for arg in &fd.args { v.visit_func_arg(&arg)?; }
		v.visit_type(&fd.return_type)?;
		v.visit_stmt(&fd.code)?;
		Ok(())
	}

	pub fn func_arg<Error>(v: &mut impl ResultVisitor<E = Error>, fa: &FuncArg)
	-> Result<(), Error>{
		v.visit_ident(&fa.name)?;
		v.visit_type(&fa.ty)?;
		Ok(())
	}

	pub fn struct_decl<Error>(v: &mut impl ResultVisitor<E = Error>, sd: &StructDecl)
	-> Result<(), Error>{
		v.visit_ident(&sd.name)?;
		if let Some(base) = &sd.base { v.visit_type(&base)?; }
		for f in &sd.fields { v.visit_struct_field(&f)?; }
		for m in &sd.methods { v.visit_func_decl(&m)?; }
		Ok(())
	}

	pub fn struct_field<Error>(v: &mut impl ResultVisitor<E = Error>, sf: &Field)
	-> Result<(), Error>{
		v.visit_ident(&sf.name)?;
		v.visit_type(&sf.ty)?;
		Ok(())
	}

	pub fn var_decl<Error>(v: &mut impl ResultVisitor<E = Error>, vd: &VarDecl)
	-> Result<(), Error>{
		v.visit_ident(&vd.name)?;
		v.visit_exp(&vd.init)?;
		Ok(())
	}

	pub fn exp<Error>(v: &mut impl ResultVisitor<E = Error>, exp: &Box<Exp>)
	-> Result<(), Error>{
		use ExpKind::*;

		match &exp.as_ref().kind {
			Id(..) | IntLit(..) | BoolLit(..) | StrLit(..) => {}

			Unary { lhs, .. } => v.visit_exp(&lhs)?,

			Binary { lhs, rhs, .. } => {
				v.visit_exp(&lhs)?;
				v.visit_exp(&rhs)?;
			}
			Call { callee, args } => {
				v.visit_exp(&callee)?;
				for a in args { v.visit_exp(&a)?; }
			}
			Field { obj, name } => {
				v.visit_exp(&obj)?;
				v.visit_ident(&name)?;
			}
			Ctor { ty, args } => {
				v.visit_type(&ty)?;
				for a in args { v.visit_exp(&a)?; }
			}
		}

		Ok(())
	}

	pub fn stmt<Error>(v: &mut impl ResultVisitor<E = Error>, stmt: &Box<Stmt>)
	-> Result<(), Error>{
		use StmtKind::*;

		match &stmt.as_ref().kind {
			Block { stmts } => {
				for s in stmts { v.visit_stmt(&s)?; }
			}
			If { cond, then, else_ } => {
				v.visit_exp(&cond)?;
				v.visit_stmt(&then)?;
				if let Some(else_) = &else_ { v.visit_stmt(&else_)?; }
			}
			While { cond, code } => {
				v.visit_exp(&cond)?;
				v.visit_stmt(&code)?;
			}
			For { var, hi, code } => {
				v.visit_var_decl(&var)?;
				v.visit_exp(&hi)?;
				v.visit_stmt(&code)?;
			}
			Exp { exp } => v.visit_exp(&exp)?,
			Assign { dst, src } => {
				v.visit_exp(&dst)?;
				v.visit_exp(&src)?;
			}
			Return { val } => {
				if let Some(val) = &val { v.visit_exp(&val)?; }
			}
			Let { decl } => v.visit_var_decl(decl)?,
		}

		Ok(())
	}
}