
use std::fmt::{ Display, Formatter, Result as FmtResult };

use crate::*;

#[derive(Debug, PartialEq, Eq)]
pub enum SemErrorKind {
	RedefinedName(String),
	UndefinedName(String),
	BadFieldName(String),
	NoFieldNamed(String),
	NonConstantInit(String),
	BadArgType(String),
	BadFieldType(String),
	BadVarType(String),
	TypeMismatch { exp: String, act: String, ctx: String },
	NonVoidExpr(String),
	WrongNumArgs { exp: usize, act: usize },
	InvalidLhs(String),
	NotAType(String),
	NotAValue(String),
}

impl Display for SemErrorKind {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use SemErrorKind::*;

		match self {
			RedefinedName(n)   => write!(f, "name '{}' has already been defined.", n),
			UndefinedName(n)   => write!(f, "name '{}' has not been defined.", n),
			BadFieldName(n)    => write!(f, "fields may not be named '{}'.", n),
			NoFieldNamed(n)    => write!(f, "no field named '{}' in the given struct.", n),
			NonConstantInit(n) => write!(f, "global '{}' must have a constant initializer.", n),
			BadArgType(n)      => write!(f, "argument '{}' cannot be of type void.", n),
			BadFieldType(n)    => write!(f, "field '{}' cannot be of type void.", n),
			BadVarType(n)      => write!(f, "variable '{}' cannot be of type void.", n),
			TypeMismatch { exp, act, ctx } =>
				write!(f, "expected type '{}' in '{}', not '{}'", exp, ctx, act),
			NonVoidExpr(e) => write!(f, "expression '{}' does not have void type.", e),
			WrongNumArgs { exp, act } =>
				write!(f, "function takes {} arguments but was given {}", exp, act),
			InvalidLhs(e) => write!(f, "cannot assign into LHS of '{}'.", e),
			NotAType(n)   => write!(f, "'{}' does not name a type.", n),
			NotAValue(n)  => write!(f, "'{}' cannot be used as a value.", n),
		}
	}
}

#[derive(Debug, PartialEq, Eq)]
pub struct SemError {
	pub node_id: NodeId,
	pub kind:    SemErrorKind,
}

impl SemError {
	pub fn redefined_name(node_id: NodeId, name: &str) -> Self {
		Self { node_id, kind: SemErrorKind::RedefinedName(name.into()) }
	}

	pub fn undefined_name(node_id: NodeId, name: &str) -> Self {
		Self { node_id, kind: SemErrorKind::UndefinedName(name.into()) }
	}

	pub fn bad_field_name(node_id: NodeId, name: &str) -> Self {
		Self { node_id, kind: SemErrorKind::BadFieldName(name.into()) }
	}

	pub fn no_field_named(field: &Ident) -> Self {
		Self { node_id: field.id, kind: SemErrorKind::NoFieldNamed(field.name.clone()) }
	}

	pub fn non_constant_init(var: &Ident) -> Self {
		Self { node_id: var.id, kind: SemErrorKind::NonConstantInit(var.name.clone()) }
	}

	pub fn bad_arg_type(arg: &Ident) -> Self {
		Self { node_id: arg.id, kind: SemErrorKind::BadArgType(arg.name.clone()) }
	}

	pub fn bad_field_type(field: &Ident) -> Self {
		Self { node_id: field.id, kind: SemErrorKind::BadFieldType(field.name.clone()) }
	}

	pub fn bad_var_type(var: &Ident) -> Self {
		Self { node_id: var.id, kind: SemErrorKind::BadVarType(var.name.clone()) }
	}

	pub fn type_mismatch(node_id: NodeId, exp: &str, act: &str, ctx: &str) -> Self {
		Self {
			node_id,
			kind: SemErrorKind::TypeMismatch {
				exp: exp.into(),
				act: act.into(),
				ctx: ctx.into()
			}
		}
	}

	pub fn non_void_expr(stmt: &Stmt) -> Self {
		Self { node_id: stmt.id, kind: SemErrorKind::NonVoidExpr(stmt.to_string()) }
	}

	pub fn wrong_num_args(node_id: NodeId, exp: usize, act: usize) -> Self {
		Self { node_id, kind: SemErrorKind::WrongNumArgs { exp, act } }
	}

	pub fn invalid_lhs(exp: &Exp) -> Self {
		Self { node_id: exp.id, kind: SemErrorKind::InvalidLhs(exp.to_string()) }
	}

	pub fn not_a_type(ident: &Ident) -> Self {
		Self { node_id: ident.id, kind: SemErrorKind::NotAType(ident.name.clone()) }
	}

	pub fn not_a_value(ident: &Ident) -> Self {
		Self { node_id: ident.id, kind: SemErrorKind::NotAValue(ident.name.clone()) }
	}
}

impl Display for SemError {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		write!(f, "(node {:?}): {}", self.node_id, self.kind)
	}
}

impl std::error::Error for SemError {}

pub type SemResult<T> = Result<T, SemError>;