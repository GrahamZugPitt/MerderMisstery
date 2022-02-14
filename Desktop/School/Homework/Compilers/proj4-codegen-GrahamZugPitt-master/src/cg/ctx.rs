
use crate::*;

pub struct CgCtx<'a> {
	nc: &'a NameCtx,
	tc: &'a TypeCtx,
}

impl<'a> CgCtx<'a> {
	pub fn new(nc: &'a NameCtx, tc: &'a TypeCtx) -> Self {
		Self { nc, tc }
	}

	pub fn sym(&self, sym_id: SymbolId) -> &Symbol {
		self.nc.symbol(sym_id)
	}

	pub fn sym_name(&self, sym_id: SymbolId) -> &str {
		&self.nc.symbol(sym_id).name
	}

	#[track_caller]
	pub fn sym_type(&self, sym_id: SymbolId) -> Box<Type> {
		self.tc.get_sym_type(sym_id).unwrap()
	}

	pub fn sym_and_type(&self, sym_id: SymbolId) -> (&Symbol, Box<Type>) {
		(self.sym(sym_id), self.sym_type(sym_id))
	}

	pub fn scope_name(&self, sym_id: ScopeId) -> &str {
		self.nc.scope_name(sym_id)
	}

	#[track_caller]
	pub fn node_type(&self, id: NodeId) -> Box<Type> {
		self.tc.get_node_type(id).unwrap()
	}

	#[track_caller]
	pub fn sym_declared_by(&self, id: &Ident) -> SymbolId {
		self.nc.sym_declared_by(id).expect("node is not a declaration")
	}

	#[track_caller]
	pub fn sym_used_by(&self, id: &Ident) -> SymbolId {
		self.nc.sym_used_by(id).unwrap()
	}

	#[track_caller]
	pub fn lookup_struct_field(&self, struct_sym_id: SymbolId, name: &str) -> Option<SymbolId> {
		let field_scope = self.nc.symbol_scope(struct_sym_id).unwrap();
		self.nc.lookup_no_traverse(field_scope, name)
	}
}
