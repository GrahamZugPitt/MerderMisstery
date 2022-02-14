
use std::sync::atomic::{ AtomicUsize, Ordering };

// TODO: location info in AST nodes

// ------------------------------------------------------------------------------------
// NodeId
// ------------------------------------------------------------------------------------

#[derive(PartialEq, Eq, Clone, Copy, Hash)]
pub struct NodeId(pub usize);

static NODE_ID: AtomicUsize = AtomicUsize::new(1);

impl NodeId {
	pub const INVALID: NodeId = NodeId(0);

	pub fn new() -> Self {
		NodeId(NODE_ID.fetch_add(1, Ordering::SeqCst))
	}
}

// ------------------------------------------------------------------------------------
// Ident
// ------------------------------------------------------------------------------------

pub struct Ident {
	pub id:   NodeId,
	pub name: String,
}

impl PartialEq for Ident {
	fn eq(&self, other: &Self) -> bool {
		self.name == other.name
	}
}

impl Eq for Ident {}

impl Ident {
	pub fn new(name: &str) -> Self {
		Self { id: NodeId::new(), name: name.into() }
	}

	// Makes a duplicate with the same name but a new id.
	pub fn dup(&self) -> Self {
		Self::new(&self.name)
	}
}

/* -------------------------------------------------------------------------------------
AstType

Grammar:

	Type:       VoidType | 'bool' | 'int' | 'string' | FuncType | StructType
	VoidType:   '(' ')'
	FuncType:   'fn' '(' (Type (',' Type)*)? ')' ':' Type
	StructType: Id
------------------------------------------------------------------------------------- */

#[derive(Debug)]
pub enum AstType {
	Void,
	Bool,
	Int,
	String,
	Func { id: NodeId, args: Vec<Box<AstType>>, ret: Box<AstType> },
	Struct(Ident),
}

impl PartialEq for AstType {
	fn eq(&self, other: &Self) -> bool {
		use AstType::*;

		match (self, other) {
			(Void, Void)     => true,
			(Bool, Bool)     => true,
			(Int, Int)       => true,
			(String, String) => true,
			(Func { args: a1, ret: r1, .. }, Func { args: a2, ret: r2, .. }) =>
				a1 == a2 && r1 == r2,
			(Struct(a), Struct(b)) => a == b,
			_ => false
		}
	}
}

impl Eq for AstType {}

impl AstType {
	pub fn new_void() -> Box<Self> {
		Box::new(AstType::Void)
	}

	pub fn new_bool() -> Box<Self> {
		Box::new(AstType::Bool)
	}

	pub fn new_int() -> Box<Self> {
		Box::new(AstType::Int)
	}

	pub fn new_string() -> Box<Self> {
		Box::new(AstType::String)
	}

	pub fn new_func(args: Vec<Box<Self>>, ret: Box<Self>) -> Box<Self> {
		Box::new(AstType::Func { id: NodeId::new(), args, ret })
	}

	pub fn new_struct(id: Ident) -> Box<Self> {
		Box::new(AstType::Struct(id))
	}
}

/* -------------------------------------------------------------------------------------
Declarations

Grammar:

	Program:    Decl*
	Decl:       VarDecl | FuncDecl | StructDecl

	VarDecl:    'let' Id '=' Exp ';'

	FuncDecl:   'fn' Id '(' FuncArgs? ')' (':' Type)? BlockStmt
	FuncArgs:   FuncArg (',' FuncArg)*
	FuncArg:    Id ':' Type

	StructDecl: 'struct' Id (':' Id)? '{' FieldDecls FuncDecl* '}'
	FieldDecls: FieldDecl (',' FieldDecl)* ','?
	FieldDecl:  Id ':' Type
------------------------------------------------------------------------------------- */

#[derive(Debug, PartialEq, Eq)]
pub struct Program {
	pub items: Vec<Box<Decl>>,
}

#[derive(Debug, PartialEq, Eq)]
pub struct FuncArg {
	pub name: Ident,
	pub ty:   Box<AstType>,
}

#[derive(Debug, PartialEq, Eq)]
pub struct FuncDecl {
	pub name:        Ident,
	pub this:        Option<FuncArg>,
	pub args:        Vec<FuncArg>,
	pub return_type: Box<AstType>,
	pub code:        Box<Stmt>,
}

#[derive(Debug, PartialEq, Eq)]
pub struct Field {
	pub name: Ident,
	pub ty:   Box<AstType>,
}

#[derive(Debug, PartialEq, Eq)]
pub struct StructDecl {
	pub name:    Ident,
	pub base:    Option<Box<AstType>>,
	pub fields:  Vec<Field>,
	pub methods: Vec<FuncDecl>,
}

#[derive(Debug, PartialEq, Eq)]
pub struct VarDecl {
	pub name: Ident,
	pub init: Box<Exp>
}

impl VarDecl {
	pub fn new(name: Ident, init: Box<Exp>) -> Box<Self> {
		Box::new(VarDecl { name, init })
	}
}

#[derive(Debug, PartialEq, Eq)]
pub enum Decl {
	Func(FuncDecl),
	Struct(StructDecl),
	Var(VarDecl),
}

impl Decl {
	pub fn new_func(fd: FuncDecl) -> Box<Self> {
		Box::new(Self::Func(fd))
	}

	pub fn new_struct(sd: StructDecl) -> Box<Self> {
		Box::new(Self::Struct(sd))
	}

	pub fn new_var(vd: VarDecl) -> Box<Self> {
		Box::new(Self::Var(vd))
	}
}

/* -------------------------------------------------------------------------------------
Statements.

	Stmt: BlockStmt | IfStmt | WhileStmt | ForStmt | ExpStmt | AssignStmt | ReturnStmt | LetStmt
	BlockStmt:  '{' Stmt* '}'
	IfStmt:     'if' Exp BlockStmt ('else' (BlockStmt | IfStmt))?
	WhileStmt:  'while' Exp BlockStmt
	ForStmt:    'for' Id 'in' Exp ',' Exp BlockStmt
	ExpStmt:    Exp ';'
	AssignStmt: Exp '=' Exp ';'
	ReturnStmt: 'return' Exp? ';'
	LetStmt:    VarDecl
------------------------------------------------------------------------------------- */

#[derive(Debug, PartialEq, Eq)]
pub enum StmtKind {
	Block  { stmts: Vec<Box<Stmt>> },
	If     { cond:  Box<Exp>, then: Box<Stmt>, else_: Option<Box<Stmt>> },
	While  { cond:  Box<Exp>, code: Box<Stmt> },
	For    { var:   Box<VarDecl>, hi: Box<Exp>, code: Box<Stmt> },
	Exp    { exp:   Box<Exp> },
	Assign { dst:   Box<Exp>, src: Box<Exp> },
	Return { val:   Option<Box<Exp>> },
	Let    { decl:  Box<VarDecl> },
}

#[derive(Debug)]
pub struct Stmt {
	pub id:   NodeId,
	pub kind: StmtKind,
}

impl PartialEq for Stmt {
	fn eq(&self, other: &Self) -> bool {
		self.kind == other.kind
	}
}

impl Eq for Stmt {}

impl Stmt {
	pub fn new(kind: StmtKind) -> Box<Self> {
		Box::new(Self { id: NodeId::new(), kind })
	}

	pub fn new_block(stmts: Vec<Box<Stmt>>) -> Box<Self> {
		Self::new(StmtKind::Block { stmts })
	}

	pub fn new_if(cond: Box<Exp>, then: Box<Stmt>, else_: Option<Box<Stmt>>) -> Box<Self> {
		Self::new(StmtKind::If { cond, then, else_ })
	}

	pub fn new_while(cond: Box<Exp>, code: Box<Stmt>) -> Box<Self> {
		Self::new(StmtKind::While { cond, code })
	}

	pub fn new_for(var: Box<VarDecl>, hi: Box<Exp>, code: Box<Stmt>) -> Box<Self> {
		Self::new(StmtKind::For { var, hi, code })
	}

	pub fn new_exp(exp: Box<Exp>) -> Box<Self> {
		Self::new(StmtKind::Exp { exp })
	}

	pub fn new_assign(dst: Box<Exp>, src: Box<Exp>) -> Box<Self> {
		Self::new(StmtKind::Assign { dst, src })
	}

	pub fn new_return(val: Option<Box<Exp>>) -> Box<Self> {
		Self::new(StmtKind::Return { val })
	}

	pub fn new_let(decl: VarDecl) -> Box<Self> {
		Self::new(StmtKind::Let { decl: Box::new(decl) })
	}

	// whether or not this statement is (or ends with) a return statement.
	// parser guarantees that return statements can only be last ones in blocks.
	pub fn returns(&self) -> bool {
		use StmtKind::*;

		match &self.kind {
			Return { .. } => true,
			Block { stmts } => stmts.last().map_or(false, |s| s.returns()),
			While { code, .. } | For { code, .. } => code.returns(),

			// if statement "returns" if BOTH sides return.
			// if there is no else clause, then it counts as "not returning".
			If { then, else_, .. } =>
				then.returns() &&
				else_.as_ref().map_or(false, |s| s.returns()),

			_ => false,
		}
	}
}

/* -------------------------------------------------------------------------------------
Expressions. Operator precedence and associativity are specified in parse::operator.

	Exp:       Term (BinOp Term)*
	Term:      UnOp* Primary Postfix*
	Primary:   Id | Num | StrLit | BoolLit | Ctor | '(' Exp ')'

	BinOp:     '+'|'-'|'*'|'/'|'%'|'<<'|'>>'|'<'|'>'|'<='|'>='|'=='|'!='|'and'|'or'
	UnOp:      '-'|'not'
	Postfix:   Call | Field
	Call:      '(' (Exp (',' Exp)*)? ')'
	Field:     '.' Id

	Id:        <Token::Id>
	Num:       <Token::Num>
	StrLit:    <Token::StrLit>
	BoolLit:   <Token::True> | <Token::False>
	Ctor:      'new' Id '(' (Exp (',' Exp)*)? ')'
------------------------------------------------------------------------------------- */

#[derive(Debug, PartialEq, Eq, Copy, Clone)]
pub enum UnOp { Neg, Not, }

#[derive(Debug, PartialEq, Eq, Copy, Clone)]
pub enum BinOp { Add, Sub, Mul, Div, Mod, Shl, Shr,
Less, Greater, LessEq, GreaterEq, Eq, NotEq, And, Or }

#[derive(PartialEq, Eq)]
pub enum ExpKind {
	Id     (Ident),
	IntLit (i64),
	BoolLit(bool),
	StrLit (String),
	Unary  { op: UnOp,  lhs: Box<Exp>, },
	Binary { op: BinOp, lhs: Box<Exp>, rhs: Box<Exp>, },
	Call   { callee: Box<Exp>, args: Vec<Box<Exp>> },
	Field  { obj: Box<Exp>, name: Ident, },
	Ctor   { ty: Box<AstType>, args: Vec<Box<Exp>> },
}

#[derive(Debug)]
pub struct Exp {
	pub id: NodeId,
	pub kind: ExpKind,
}

impl PartialEq for Exp {
	fn eq(&self, other: &Self) -> bool {
		self.kind == other.kind
	}
}

impl Eq for Exp {}

impl Exp {
	pub fn new(kind: ExpKind) -> Box<Self> {
		Box::new(Self { id: NodeId::new(), kind })
	}

	pub fn new_id(s: &str) -> Box<Self> {
		Self::new(ExpKind::Id(Ident::new(s)))
	}

	pub fn new_int_lit(v: i64)  -> Box<Self> {
		Self::new(ExpKind::IntLit(v))
	}

	pub fn new_bool_lit(v: bool) -> Box<Self> {
		Self::new(ExpKind::BoolLit(v))
	}

	pub fn new_str_lit(s: &str) -> Box<Self> {
		Self::new(ExpKind::StrLit(s.into()))
	}

	pub fn new_unary(op: UnOp, lhs: Box<Exp>) -> Box<Self> {
		Self::new(ExpKind::Unary { op, lhs })
	}

	pub fn new_binary(lhs: Box<Exp>, op: BinOp, rhs: Box<Exp>) -> Box<Self> {
		Self::new(ExpKind::Binary { op, lhs, rhs })
	}

	pub fn new_func_call(callee: Box<Exp>, args: Vec<Box<Exp>>) -> Box<Self> {
		Self::new(ExpKind::Call { callee, args })
	}

	pub fn new_field(obj: Box<Exp>, name: Ident) -> Box<Self> {
		Self::new(ExpKind::Field { obj, name })
	}

	pub fn new_ctor(ty: Box<AstType>, args: Vec<Box<Exp>>) -> Box<Self> {
		Self::new(ExpKind::Ctor { ty, args })
	}
}