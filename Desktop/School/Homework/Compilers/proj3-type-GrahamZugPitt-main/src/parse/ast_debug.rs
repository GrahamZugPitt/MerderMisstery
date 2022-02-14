
use std::fmt::{ Debug, Formatter, Result as FmtResult };
use crate::parse::ast::*;

impl Debug for NodeId {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		write!(f, "NodeId({})", self.0)
	}
}

impl Debug for Ident {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		write!(f, "Ident({:?}, \"{}\")", self.id, self.name)
	}
}

impl Debug for ExpKind {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use ExpKind::*;

		match self {
			Id(n)      => write!(f, "Id(\"{}\")", n),
			IntLit(i)  => write!(f, "IntLit({})", i),
			BoolLit(b) => write!(f, "BoolLit({})", b),
			StrLit(s)  => write!(f, "StrLit(\"{}\")", s),

			Unary { op, lhs } =>
				f.debug_struct("Unary")
					.field("op", op)
					.field("lhs", lhs)
					.finish(),

			Binary { op, lhs, rhs } =>
				f.debug_struct("Binary")
					.field("op", op)
					.field("lhs", lhs)
					.field("rhs", rhs)
					.finish(),

			Call { callee, args } =>
				f.debug_struct("Call")
					.field("callee", callee)
					.field("args", args)
					.finish(),

			Field { obj, name } =>
				f.debug_struct("Field")
					.field("obj", obj)
					.field("name", name)
					.finish(),

			Ctor { ty, args } =>
				f.debug_struct("Ctor")
					.field("ty", ty)
					.field("args", args)
					.finish(),
		}
	}
}