
use crate::*;

// ------------------------------------------------------------------------------------
// Tests
// ------------------------------------------------------------------------------------

#[test]
fn exp_literals() {
	exp_ok("10", intlit(10),
		Type::new_int());

	exp_ok("\"hello\"", strlit("hello"),
		Type::new_string());

	exp_ok("true", boollit(true),
		Type::new_bool());

	exp_ok("false", boollit(false),
		Type::new_bool());
}

#[test]
fn exp_ident() {
	use SemErrorKind::*;

	// these just exist.
	exp_ok("var_int", id("var_int"),
		Type::new_int());

	exp_ok("var_str", id("var_str"),
		Type::new_string());

	exp_ok("var_bool", id("var_bool"),
		Type::new_bool());

	// this is just making sure the standard library functions are accessible.
	test_ok("// empty source code", prog(vec![]))
		.should_have("println_i", Type::new_func(vec![Type::new_int()],    Type::new_void()))
		.should_have("println_s", Type::new_func(vec![Type::new_string()], Type::new_void()))
		.should_have("println_c", Type::new_func(vec![Type::new_int()],    Type::new_void()))
		.should_have("print_i",   Type::new_func(vec![Type::new_int()],    Type::new_void()))
		.should_have("print_s",   Type::new_func(vec![Type::new_string()], Type::new_void()))
		.should_have("print_c",   Type::new_func(vec![Type::new_int()],    Type::new_void()));

	// can't use a struct's name where an expression is expected.
	exp_err("Point", id("Point"),
		NotAValue("Point".into()));
}

#[test]
fn exp_field() {
	use SemErrorKind::*;

	// this should work and give the type of the field.
	exp_ok("var_point.x", field(id("var_point"), "x"),
		Type::new_int());

	exp_ok("var_point.y", field(id("var_point"), "y"),
		Type::new_int());

	exp_ok("var_point.constructor", field(id("var_point"), "constructor"),
		Type::new_func(vec![Type::new_int(), Type::new_int()], Type::new_void()));

	// can't get fields from non-structs.
	exp_mismatch("var_int.x", field(id("var_int"), "x"),
		"struct type", // expected type
		"int",         // what you gave
		"var_int");    // context

	// var_point is a struct, but it has no field z.
	exp_err("var_point.z", field(id("var_point"), "z"),
		NoFieldNamed("z".into()));
}

#[test]
fn exp_unary() {
	// negation
	exp_ok("-5", neg(intlit(5)),
		Type::new_int());

	exp_ok("--5", neg(neg(intlit(5))),
		Type::new_int());

	exp_ok("-var_int", neg(id("var_int")),
		Type::new_int());

	exp_mismatch("-true", neg(boollit(true)),
		"int",      // expected type
		"bool",     // what you gave
		"(-true)"); // context

	exp_mismatch("-var_bool", neg(id("var_bool")),
		"int",          // expected type
		"bool",         // what you gave
		"(-var_bool)"); // context

	// logical 'not'
	exp_ok("not true", not(boollit(true)),
		Type::new_bool());

	exp_ok("not not true", not(not(boollit(true))),
		Type::new_bool());

	exp_mismatch("not 5", not(intlit(5)),
		"bool",     // expected type
		"int",      // what you gave
		"(not 5)"); // context

	exp_mismatch("not var_int", not(id("var_int")),
		"bool",           // expected type
		"int",            // what you gave
		"(not var_int)"); // context
}

#[test]
fn exp_call() {
	use SemErrorKind::*;

	stmt_ok("println_i(10);", Stmt::new_exp(call("println_i", vec![intlit(10)])));

	exp_ok("add(5, 10)", call("add", vec![intlit(5), intlit(10)]),
		Type::new_int());

	exp_err("println_i()", call("println_i", vec![]),
		WrongNumArgs { exp: 1, act: 0 });

	exp_err("println_i(10, 20)", call("println_i", vec![intlit(10), intlit(20)]),
		WrongNumArgs { exp: 1, act: 2 });

	exp_mismatch("println_s(10)", call("println_s", vec![intlit(10)]),
		"string",      // expected type
		"int",         // what you gave
		"argument 1"); // context

	exp_err("add()", call("add", vec![]),
		WrongNumArgs { exp: 2, act: 0 });

	exp_err("add(1, 2, 3)", call("add", vec![intlit(1), intlit(2), intlit(3)]),
		WrongNumArgs { exp: 2, act: 3 });

	exp_mismatch("add(true, 10)", call("add", vec![boollit(true), intlit(10)]),
		"int",         // expected type
		"bool",        // what you gave
		"argument 1"); // context

	exp_mismatch("add(3, true)", call("add", vec![intlit(3), boollit(true)]),
		"int",         // expected type
		"bool",        // what you gave
		"argument 2"); // context

	exp_mismatch("var_int(10)", call("var_int", vec![intlit(10)]),
		"function type", // expected type
		"int",           // what you gave
		"var_int(10)");  // context
}

#[test]
fn exp_ctor() {
	use SemErrorKind::*;

	// can't use `new` on something that isn't a struct.
	exp_err("new var_int()", ctor("var_int", vec![]),
		NotAType("var_int".into()));

	// Empty has no constructor, so you can only `new` it with 0 arguments.
	exp_ok_with("new Empty()", ctor("Empty", vec![]),
		|tcc| tcc.get_struct_type("Empty"));

	exp_err("new Empty(3)", ctor("Empty", vec![intlit(3)]),
		WrongNumArgs { exp: 0, act: 1 });

	// Point has a constructor that takes 2 int arguments.
	exp_ok_with("new Point(3, 4)", ctor("Point", vec![intlit(3), intlit(4)]),
		|tcc| tcc.get_struct_type("Point"));

	exp_err("new Point(3)", ctor("Point", vec![intlit(3)]),
		WrongNumArgs { exp: 2, act: 1 });

	exp_err("new Point()", ctor("Point", vec![]),
		WrongNumArgs { exp: 2, act: 0 });

	exp_mismatch("new Point(3, true)", ctor("Point", vec![intlit(3), boollit(true)]),
		"int",         // expected type
		"bool",        // what you gave
		"argument 2"); // context
}

#[test]
fn exp_equality() {
	// two of same type is okay.
	exp_ok("3 == 5", eq(intlit(3), intlit(5)),
		Type::new_bool());

	exp_ok("3 != 5", neq(intlit(3), intlit(5)),
		Type::new_bool());

	exp_ok("\"a\" == \"b\"", eq(strlit("a"), strlit("b")),
		Type::new_bool());

	exp_ok("\"a\" != \"b\"", neq(strlit("a"), strlit("b")),
		Type::new_bool());

	// even function types can be compared.
	exp_ok("println_s == println_s", eq(id("println_s"), id("println_s")),
		Type::new_bool());

	// but void is not okay to compare (like what println_i returns).
	exp_mismatch("println_i(5) == 5", eq(call("println_i", vec![intlit(5)]), intlit(5)),
		"non-void type", // expected type
		"()",            // what you gave
		"lhs of '=='");  // context
}

#[test]
fn exp_relational() {
	// can compare two ints, only
	exp_ok("3 < 5", lt(intlit(3), intlit(5)),
		Type::new_bool());

	exp_ok("3 <= 5", le(intlit(3), intlit(5)),
		Type::new_bool());

	exp_ok("3 > 5", gt(intlit(3), intlit(5)),
		Type::new_bool());

	exp_ok("3 >= 5", ge(intlit(3), intlit(5)),
		Type::new_bool());

	exp_mismatch("true < 5", lt(boollit(true), intlit(5)),
		"int",         // expected type
		"bool",        // what you gave
		"lhs of '<'"); // context

	exp_mismatch("3 < true", lt(intlit(3), boollit(true)),
		"int",         // expected type
		"bool",        // what you gave
		"rhs of '<'"); // context
}

#[test]
fn exp_logical() {
	// only booleans allowed!
	exp_ok("true and false", and(boollit(true), boollit(false)),
		Type::new_bool());

	exp_ok("true or false", or(boollit(true), boollit(false)),
		Type::new_bool());

	exp_ok("var_int == 3 or var_str == \"hi\"",
		or(eq(id("var_int"), intlit(3)), eq(id("var_str"), strlit("hi"))),
		Type::new_bool());

	exp_mismatch("3 or 4", or(intlit(3), intlit(4)),
		"bool",         // expected type
		"int",          // what you gave
		"lhs of 'or'"); // context

	exp_mismatch("true or 4", or(boollit(true), intlit(4)),
		"bool",         // expected type
		"int",          // what you gave
		"rhs of 'or'"); // context
}

#[test]
fn exp_arithmetic() {
	exp_ok("3 + 5", add(intlit(3), intlit(5)),
		Type::new_int());

	exp_ok("\"a\" + \"b\"", add(strlit("a"), strlit("b")),
		Type::new_string());

	exp_ok("3 - 5", sub(intlit(3), intlit(5)),
		Type::new_int());

	exp_ok("3 * 5", mul(intlit(3), intlit(5)),
		Type::new_int());

	exp_ok("3 * 5", div(intlit(3), intlit(5)),
		Type::new_int());

	exp_ok("3 % 5", mod_(intlit(3), intlit(5)),
		Type::new_int());

	exp_ok("3 << 5", shl(intlit(3), intlit(5)),
		Type::new_int());

	exp_ok("3 >> 5", shr(intlit(3), intlit(5)),
		Type::new_int());

	exp_mismatch("3 + \"b\"", add(intlit(3), strlit("b")),
		"int",               // expected type
		"string",            // what you gave
		"rhs of '+'");       // context

	exp_mismatch("\"a\" + 5", add(strlit("a"), intlit(5)),
		"string",            // expected type
		"int",               // what you gave
		"rhs of '+'");       // context

	exp_mismatch("true + 5", add(boollit(true), intlit(5)),
		"'int' or 'string'", // expected type
		"bool",              // what you gave
		"lhs of '+'");       // context

	exp_mismatch("true - 5", sub(boollit(true), intlit(5)),
		"int",               // expected type
		"bool",              // what you gave
		"lhs of '-'");       // context

	exp_mismatch("true * 5", mul(boollit(true), intlit(5)),
		"int",               // expected type
		"bool",              // what you gave
		"lhs of '*'");       // context

	exp_mismatch("true / 5", div(boollit(true), intlit(5)),
		"int",               // expected type
		"bool",              // what you gave
		"lhs of '/'");       // context

	exp_mismatch("true % 5", mod_(boollit(true), intlit(5)),
		"int",               // expected type
		"bool",              // what you gave
		"lhs of '%'");       // context

	exp_mismatch("true << 5", shl(boollit(true), intlit(5)),
		"int",               // expected type
		"bool",              // what you gave
		"lhs of '<<'");      // context

	exp_mismatch("true >> 5", shr(boollit(true), intlit(5)),
		"int",               // expected type
		"bool",              // what you gave
		"lhs of '>>'");      // context

	exp_mismatch("3 - true", sub(intlit(3), boollit(true)),
		"int",               // expected type
		"bool",              // what you gave
		"rhs of '-'");       // context

	exp_mismatch("3 * true", mul(intlit(3), boollit(true)),
		"int",               // expected type
		"bool",              // what you gave
		"rhs of '*'");       // context

	exp_mismatch("3 / true", div(intlit(3), boollit(true)),
		"int",               // expected type
		"bool",              // what you gave
		"rhs of '/'");       // context

	exp_mismatch("3 % true", mod_(intlit(3), boollit(true)),
		"int",               // expected type
		"bool",              // what you gave
		"rhs of '%'");       // context

	exp_mismatch("3 << true", shl(intlit(3), boollit(true)),
		"int",               // expected type
		"bool",              // what you gave
		"rhs of '<<'");      // context

	exp_mismatch("3 >> true", shr(intlit(3), boollit(true)),
		"int",               // expected type
		"bool",              // what you gave
		"rhs of '>>'");      // context
}

// ------------------------------------------------------------------------------------
// Test harness functions
// ------------------------------------------------------------------------------------

#[track_caller]
fn stmt_ok(src: &'static str, stmt: Box<Stmt>) {
	test_ok(src, stmt_ast(stmt));
}

#[track_caller]
fn exp_ok(src: &'static str, exp: Box<Exp>, ty: Box<Type>) {
	test_ok(src, exp_ast(exp))
		.child_named("test")
		.should_have("_RESULT_", ty);
}

#[track_caller]
fn exp_ok_with(src: &'static str, exp: Box<Exp>, ty: impl Fn(&TypeCtxChecker) -> Box<Type>) {
	test_ok(src, exp_ast(exp))
		.child_named("test")
		.should_have_with("_RESULT_", ty);
}

#[track_caller]
fn exp_err(src: &'static str, exp: Box<Exp>, expected: SemErrorKind) {
	test_err(src, exp_ast(exp), expected)
}

#[track_caller]
fn exp_mismatch(src: &'static str, exp: Box<Exp>, expected: &str, act: &str, ctx: &str) {
	exp_err(src, exp, SemErrorKind::TypeMismatch {
		exp: expected.into(),
		act: act.into(),
		ctx: ctx.into(),
	})
}

fn exp_ast(exp: Box<Exp>) -> Program {
	stmt_ast(letstmt("_RESULT_", exp))
}

fn stmt_ast(stmt: Box<Stmt>) -> Program {
	/*
	struct Empty {}

	struct Point {
		x: int,
		y: int,

		fn constructor(x: int, y: int) {
		}
	}

	fn add(x: int, y: int): int {
		return 5;
	}

	fn test(var_int: int, var_str: string, var_bool: bool, var_point: Point) {
		let _RESULT_ = <the expression being tested>;
	}
	*/

	prog(vec![
		structdecl("Empty", None, vec![], vec![]),
		structdecl("Point", None,
			vec![
				("x", AstType::new_int()),
				("y", AstType::new_int()),
			],
			vec![
				fndecl("constructor", Some("Point"), vec![
					("x", AstType::new_int()),
					("y", AstType::new_int()),
				], AstType::new_void(), vec![
					// Stmt::new_assign(field(id("this"), "x"), id("x")),
					// Stmt::new_assign(field(id("this"), "y"), id("y")),
				])
			],
		),
		fndecl("add", None, vec![("x", AstType::new_int()), ("y", AstType::new_int())],
			AstType::new_int(),
		vec![
			// Stmt::new_return(Some(add(id("x"), id("y")))),
			Stmt::new_return(Some(intlit(5))),
		]),
		fndecl("test", None, vec![
			("var_int", AstType::new_int()),
			("var_str", AstType::new_string()),
			("var_bool", AstType::new_bool()),
			("var_point", AstType::new_struct(Ident::new("Point"))),
		],
		AstType::new_void(),
		vec![
			stmt
		])
	])
}

#[track_caller]
fn test_ok(src: &'static str, ast: Program) -> TypeCtxChecker {
	let nc = Compiler::sem_symbols(&ast).expect("don't change the test cases, please");

	match Compiler::sem_typecheck(&ast, &nc) {
		Ok(tc) => TypeCtxChecker::new(src, nc, tc),
		Err(e) =>
			panic!("on '{}', type pass should have succeeded but failed with error: {}", src, e),
	}
}

#[track_caller]
fn test_err(src: &'static str, ast: Program, expected: SemErrorKind) {
	let nc = Compiler::sem_symbols(&ast).expect("don't change the test cases, please");

	match TypecheckPass::run(&ast, &nc) {
		Ok(..) =>
			panic!("on '{}' should have failed with error '{}', but succeeded", src, expected),
		Err(actual) => {
			if expected != actual.kind {
				panic!(concat!("on '{}', should have failed with error:\n\t{}",
					"\nbut failed with this error instead:\n\t{}"),
					src, expected, actual.kind)
			}
		}
	}
}

fn prog(items: Vec<Box<Decl>>) -> Program {
	Program { items }
}

fn letstmt(name: &str, init: Box<Exp>) -> Box<Stmt> {
	Stmt::new_let(VarDecl { name: Ident::new(name), init })
}

fn fndecl(name: &str, this: Option<&str>, args: Vec<(&str, Box<AstType>)>, ret: Box<AstType>,
code: Vec<Box<Stmt>>) -> Box<Decl> {
	let this = this.map(|struct_name| {
		FuncArg {
			name: Ident::new("this"),
			ty: AstType::new_struct(Ident::new(struct_name)),
		}
	});

	let args = args.into_iter().map(|(arg_name, ty)| FuncArg {
		name: Ident::new(arg_name),
		ty,
	}).collect();

	Decl::new_func(FuncDecl {
		name: Ident::new(name),
		this,
		args,
		return_type: ret,
		code: Stmt::new_block(code),
	})
}

fn structdecl(name: &str, base: Option<&str>, fields: Vec<(&str, Box<AstType>)>,
methods: Vec<Box<Decl>>) -> Box<Decl> {
	let base = base.map(|struct_name| AstType::new_struct(Ident::new(struct_name)));

	let fields = fields.into_iter().map(|(field_name, ty)| Field {
		name: Ident::new(field_name),
		ty,
	}).collect();

	let methods = methods.into_iter().map(|decl| {
		match *decl {
			Decl::Func(fd) => fd,
			_ => panic!(),
		}
	}).collect();

	Decl::new_struct(StructDecl {
		name: Ident::new(name),
		base,
		fields,
		methods
	})
}

fn intlit(val: i64) -> Box<Exp> {
	Exp::new_int_lit(val)
}

fn strlit(val: &str) -> Box<Exp> {
	Exp::new_str_lit(val)
}

fn boollit(val: bool) -> Box<Exp> {
	Exp::new_bool_lit(val)
}

fn id(name: &str) -> Box<Exp> {
	Exp::new_id(name)
}

fn field(obj: Box<Exp>, name: &str) -> Box<Exp> {
	Exp::new_field(obj, Ident::new(name))
}

fn call(callee: &str, args: Vec<Box<Exp>>) -> Box<Exp> {
	Exp::new_func_call(Exp::new_id(callee), args)
}

fn ctor(ty: &str, args: Vec<Box<Exp>>) -> Box<Exp> {
	Exp::new_ctor(AstType::new_struct(Ident::new(ty)), args)
}

fn neg(e: Box<Exp>) -> Box<Exp> {
	Exp::new_unary(UnOp::Neg, e)
}

fn not(e: Box<Exp>) -> Box<Exp> {
	Exp::new_unary(UnOp::Not, e)
}

fn eq(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Eq, r)
}

fn neq(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::NotEq, r)
}

fn lt(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Less, r)
}

fn le(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::LessEq, r)
}

fn gt(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Greater, r)
}

fn ge(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::GreaterEq, r)
}

fn and(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::And, r)
}

fn or(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Or, r)
}

fn add(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Add, r)
}

fn sub(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Sub, r)
}

fn mul(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Mul, r)
}

fn div(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Div, r)
}

fn mod_(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Mod, r)
}

fn shl(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Shl, r)
}

fn shr(l: Box<Exp>, r: Box<Exp>) -> Box<Exp> {
	Exp::new_binary(l, BinOp::Shr, r)
}