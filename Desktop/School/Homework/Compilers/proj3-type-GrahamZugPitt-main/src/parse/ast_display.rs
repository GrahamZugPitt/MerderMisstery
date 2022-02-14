
use std::fmt::{ Display, Formatter, Result as FmtResult };
use crate::parse::ast::*;

impl Display for Ident {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		write!(f, "{}", self.name)
	}
}

impl Display for AstType {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use AstType::*;

		match self {
			Void        => write!(f, "()"),
			Bool        => write!(f, "bool"),
			Int         => write!(f, "int"),
			String      => write!(f, "string"),
			Struct(id)  => write!(f, "{}", id),

			Func { args, ret, .. } => {
				write!(f, "fn(")?;

				if args.len() > 0 {
					write!(f, "{}", args[0])?;

					for a in &args[1..] {
						write!(f, ", {}", a)?;
					}
				}

				if **ret == AstType::Void {
					write!(f, ")")
				} else {
					write!(f, "): {}", ret)
				}
			}
		}
	}
}

impl Display for UnOp {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use UnOp::*;

		write!(f, "{}", match self {
			Neg => "-",
			Not => "not ",
		})
	}
}

impl Display for BinOp {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use BinOp::*;

		write!(f, "{}", match self {
			Add       => "+",
			Sub       => "-",
			Mul       => "*",
			Div       => "/",
			Mod       => "%",
			Shl       => "<<",
			Shr       => ">>",
			Less      => "<",
			Greater   => ">",
			LessEq    => "<=",
			GreaterEq => ">=",
			Eq        => "==",
			NotEq     => "!=",
			And       => "and",
			Or        => "or",
		})
	}
}

impl Display for Exp {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use ExpKind::*;

		match &self.kind {
			Unary  { op, lhs      } => write!(f, "({}{})", op, lhs),
			Binary { op, lhs, rhs } => write!(f, "({} {} {})", lhs, op, rhs),
			Id     (name)           => write!(f, "{}", name),
			IntLit (value)          => write!(f, "{}", value),
			BoolLit(value)          => write!(f, "{}", value),
			StrLit (value)          => write!(f, "\"{}\"", value),
			Call   { callee, args } => {
				write!(f, "{}(", callee)?;
				let mut first = true;
				for arg in args {
					if first { first = false; } else { write!(f, ", ")?; }
					write!(f, "{}", arg)?;
				}

				write!(f, ")")
			}
			Ctor { ty, args } => {
				write!(f, "new {}(", ty)?;
				let mut first = true;
				for arg in args {
					if first { first = false; } else { write!(f, ", ")?; }
					write!(f, "{}", arg)?;
				}

				write!(f, ")")
			}
			Field { obj, name } => write!(f, "{}.{}", obj, name),
		}
	}
}

impl Stmt {
	fn fmt_helper(&self, f: &mut Formatter, depth: usize) -> FmtResult {
		let indent = "  ".repeat(depth);

		use StmtKind::*;

		match &self.kind {
			Block { stmts } => {
				write!(f, "{{\n")?;

				for s in stmts {
					s.fmt_helper(f, depth + 1)?;
				}

				write!(f, "{}}}\n", indent)?;
			}

			If { cond, then, else_ } => {
				write!(f, "{}if {} ", indent, cond)?;
				then.fmt_helper(f, depth)?;

				if let Some(else_stmt) = else_ {
					write!(f, "{}else ", indent)?;
					else_stmt.fmt_helper(f, depth)?;
				}
			}

			While { cond, code } => {
				write!(f, "{}while {} ", indent, cond)?;
				code.fmt_helper(f, depth)?;
			}

			For { var, hi, code } => {
				write!(f, "{}for {} in {}, {} ", indent, var.name, var.init, hi)?;
				code.fmt_helper(f, depth)?;
			}

			Exp { exp } => {
				write!(f, "{}{};\n", indent, exp)?;
			}

			Assign { dst, src } => {
				write!(f, "{}{} = {};\n", indent, dst, src)?;
			}

			Return { val } => {
				match val {
					Some(exp) => write!(f, "{}return {};\n", indent, exp)?,
					None      => write!(f, "{}return;\n", indent)?,
				}
			}

			Let { decl } => write!(f, "{}", decl)?,
		}

		return Ok(());
	}
}

impl Display for Stmt {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		self.fmt_helper(f, 0)
	}
}

impl FuncDecl {
	fn fmt_helper(&self, f: &mut Formatter, depth: usize) -> FmtResult {
		let indent = "  ".repeat(depth);

		write!(f, "{}fn {}(", indent, self.name)?;

		if let Some(FuncArg { name, ty }) = &self.this {
			write!(f, "{}: {}", name, ty)?;

			if self.args.len() > 0 {
				write!(f, ", ")?;
			}
		}

		if self.args.len() > 0 {
			write!(f, "{}: {}", self.args[0].name.name, self.args[0].ty)?;

			for arg in &self.args[1..] {
				write!(f, ", {}: {}", arg.name.name, arg.ty)?;
			}
		}

		if *self.return_type != AstType::Void {
			write!(f, ") : {} ", *self.return_type)?;
		} else {
			write!(f, ") ")?;
		}

		self.code.fmt_helper(f, depth)
	}
}

impl VarDecl {
	fn fmt_helper(&self, f: &mut Formatter, depth: usize) -> FmtResult {
		let indent = "  ".repeat(depth);
		writeln!(f, "{}let {} = {};", indent, self.name, self.init)
	}
}

impl Display for VarDecl {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		self.fmt_helper(f, 0)
	}
}

impl Decl {
	fn fmt_helper(&self, f: &mut Formatter, depth: usize) -> FmtResult {
		let indent = "  ".repeat(depth);

		use Decl::*;

		match &self {
			Var(d) => {
				d.fmt_helper(f, depth)?;
			}

			Func(d) => {
				d.fmt_helper(f, depth)?;
			}

			Struct(StructDecl { name, base, fields, methods }) => {
				write!(f, "{}struct {}", indent, name)?;
				if let Some(b) = base { write!(f, " : {}", b)?; }
				writeln!(f, " {{")?;

				for Field { name, ty, .. } in fields {
					writeln!(f, "{}  {}: {},", indent, name, ty)?;
				}

				for m in methods {
					m.fmt_helper(f, depth + 1)?;
				}
				writeln!(f, "{}}}", indent)?;
			}
		}

		Ok(())
	}
}

impl Display for Decl {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		self.fmt_helper(f, 0)
	}
}

impl Display for Program {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		for item in &self.items {
			write!(f, "{}", item)?;
		}

		Ok(())
	}
}