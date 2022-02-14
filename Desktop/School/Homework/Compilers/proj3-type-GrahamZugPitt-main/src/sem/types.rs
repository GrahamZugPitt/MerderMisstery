
use std::collections::{ HashMap };
use std::fmt::{ Display, Formatter, Result as FmtResult };

use crate::*;

// ------------------------------------------------------------------------------------------------
// Type
// ------------------------------------------------------------------------------------------------

/// The *semantic* representation of a type. While [`AstType`] is just what the user wrote
/// in the source code, this is what they get turned into.
#[derive(Debug, PartialEq, Eq, Clone)]
pub enum Type {
	Void,
	Bool,
	Int,
	String,
	Func { args: Vec<Box<Type>>, ret: Box<Type> },
	Struct(SymbolId),
}

impl Type {
	pub fn new_void() -> Box<Self> {
		Box::new(Type::Void)
	}

	pub fn new_bool() -> Box<Self> {
		Box::new(Type::Bool)
	}

	pub fn new_int() -> Box<Self> {
		Box::new(Type::Int)
	}

	pub fn new_string() -> Box<Self> {
		Box::new(Type::String)
	}

	pub fn new_func(args: Vec<Box<Self>>, ret: Box<Self>) -> Box<Self> {
		Box::new(Type::Func { args, ret })
	}

	pub fn new_struct(id: SymbolId) -> Box<Self> {
		Box::new(Type::Struct(id))
	}

	pub fn is_void(&self) -> bool {
		matches!(self, Type::Void)
	}

	pub fn is_bool(&self) -> bool {
		matches!(self, Type::Bool)
	}

	pub fn is_int(&self) -> bool {
		matches!(self, Type::Int)
	}

	pub fn is_string(&self) -> bool {
		matches!(self, Type::String)
	}

	pub fn is_function(&self) -> bool {
		matches!(self, Type::Func { .. })
	}

	pub fn is_struct(&self) -> bool {
		matches!(self, Type::Struct(..))
	}
}

impl Display for Type {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use Type::*;

		match self {
			Void       => write!(f, "()"),
			Bool       => write!(f, "bool"),
			Int        => write!(f, "int"),
			String     => write!(f, "string"),
			Struct(id) => write!(f, "<{:?}>", id),

			Func { args, ret } => {
				write!(f, "fn(")?;

				if args.len() > 0 {
					write!(f, "{}", args[0])?;

					for a in &args[1..] {
						write!(f, ", {}", a)?;
					}
				}

				if **ret == Type::Void {
					write!(f, ")")
				} else {
					write!(f, "): {}", ret)
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------
// TypeCtx
// ------------------------------------------------------------------------------------------------

pub struct TypeCtx {
	pub sym_types:  HashMap<SymbolId, Box<Type>>,
	pub node_types: HashMap<NodeId, Box<Type>>,
}

impl TypeCtx {
	pub(crate) fn new() -> Self {
		Self {
			sym_types: HashMap::new(),
			node_types: HashMap::new(),
		}
	}

	pub(crate) fn set_sym_type(&mut self, sym_id: SymbolId, ty: Box<Type>) {
		let result = self.sym_types.insert(sym_id, ty);
		assert!(result.is_none());
	}

	pub(crate) fn set_node_type(&mut self, id: NodeId, ty: Box<Type>) {
		let result = self.node_types.insert(id, ty);
		assert!(result.is_none());
	}

	pub(crate) fn get_sym_type(&self, sym_id: SymbolId) -> Option<Box<Type>> {
		self.sym_types.get(&sym_id).map(|x| x.clone())
	}
}

// ------------------------------------------------------------------------------------------------
// TypeCtxChecker
// ------------------------------------------------------------------------------------------------

pub struct TypeCtxChecker {
	ncc: NameCtxChecker,
	tc:  TypeCtx,
}

impl TypeCtxChecker {
	pub fn new(src: &'static str, nc: NameCtx, tc: TypeCtx) -> Self {
		let ncc = NameCtxChecker::new(src, nc);
		Self { ncc, tc }
	}

	pub fn get_struct_type(&self, name: &str) -> Box<Type> {
		self.ncc.get_struct_type(name)
	}

	#[track_caller]
	pub fn child_named(self, name: &str) -> Self {
		let ncc = self.ncc.child_named(name);
		Self { ncc, tc: self.tc }
	}

	#[track_caller]
	pub fn child(self, i: usize) -> Self {
		let ncc = self.ncc.child(i);
		Self { ncc, tc: self.tc }
	}

	#[track_caller]
	pub fn parent(self) -> Self {
		let ncc = self.ncc.parent();
		Self { ncc, tc: self.tc }
	}

	#[track_caller]
	pub fn should_have(self, name: &str, expected: Box<Type>) -> Self {
		let sym_id = self.ncc.lookup(name).expect("don't change the test cases, please");
		let sym_ty = self.tc.get_sym_type(sym_id).expect("don't change the test cases, please");

		if sym_ty != expected {
			panic!("expected type '{}', but your code gave type '{}', in test case:\n{}\n",
				expected, sym_ty, self.ncc.get_src());
		}

		self
	}

	#[track_caller]
	pub fn should_have_with(self, name: &str, expected: impl Fn(&Self) -> Box<Type>) -> Self {
		let sym_id = self.ncc.lookup(name).expect("don't change the test cases, please");
		let sym_ty = self.tc.get_sym_type(sym_id).expect("don't change the test cases, please");
		let expected = expected(&self);

		if sym_ty != expected {
			panic!("expected type '{}', but your code gave type '{}', in test case:\n{}\n",
				expected, sym_ty, self.ncc.get_src());
		}

		self
	}

	#[track_caller]
	pub fn shouldnt_have(self, name: &str) -> Self {
		let ncc = self.ncc.shouldnt_have(name);
		Self { ncc, tc: self.tc }
	}
}