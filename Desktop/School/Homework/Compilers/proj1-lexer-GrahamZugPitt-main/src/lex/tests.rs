use crate::*;
use TokenKind::*;

// ------------------------------------------------------------------------------------
// The tests!
// ------------------------------------------------------------------------------------

#[test]
fn whitespace() {
	test_ok("   \n\t\n", vec![Eof]);
}

#[test]
fn comments() {
	test_ok("//hello\n//world", vec![Eof]);
}

#[test]
fn invalid_chars() {
	test_err("@",   LexErrorKind::InvalidChar, (1, 1));
	test_err("  ^", LexErrorKind::InvalidChar, (1, 3));
}

#[test]
fn simple_symbols() {
	test_ok("%*+-/(){}[];:,.", vec![
		Modulo, Times, Plus, Minus, Divide, LParen, RParen, LBrace, RBrace,
		LBracket, RBracket, Semi, Colon, Comma, Dot, Eof
	]);
}

#[test]
fn complex_symbols() {
	test_ok("= == === !===", vec![Assign, Eq, Eq, Assign, NotEq, Eq, Eof]);
	test_ok("< << <= <== > >> >= >==", vec![
			Less, Shl, LessEq, LessEq, Assign,
			Greater, Shr, GreaterEq, GreaterEq, Assign, Eof]);
	test_err("!", LexErrorKind::InvalidChar, (1, 1));
}

#[test]
fn identifiers() {
	test_ok("this is a test", vec![id("this"), id("is"), id("a"), id("test"), Eof]);
	test_ok("abc(",           vec![id("abc"), LParen, Eof]);
	test_ok("$___$",          vec![id("$___$"), Eof]);
	test_ok("_abcABC123$",    vec![id("_abcABC123$"), Eof]);
	test_ok("tést これ",      vec![id("tést"), id("これ"), Eof]);
}

#[test]
fn keywords() {
	test_ok("if else for in fn let while break int bool string",
		vec![If, Else, For, In, Fn, Let, While, Break, Int, Bool, String, Eof]);

	test_ok("return and or not true false struct new",
		vec![Return, And, Or, Not, True, False, Struct, New, Eof]);

	test_ok("IF ELSE", vec![id("IF"), id("ELSE"), Eof]);
	test_ok("forth",   vec![id("forth"), Eof]);
}

#[test]
fn string_literals() {
	test_ok(r#""hello, world!""#,  vec![strlit("hello, world!"), Eof]);
	test_ok(r#""\\ \r \t \n \"""#, vec![strlit("\\ \r \t \n \""), Eof]);
	test_err(r#""oh no"#, LexErrorKind::UnclosedString, (1, 1));
	test_err(r#""oh no this is
		also bad""#, LexErrorKind::UnclosedString, (1, 1));
	test_err(r#""uh oh\z""#, LexErrorKind::UnknownEscape, (1, 7));
}

#[test]
fn good_numbers() {
	test_ok("123",    vec![IntLit(123,    Base::Dec), Eof]);
	test_ok("1_2_3",  vec![IntLit(123,    Base::Dec), Eof]);
	test_ok("0___0",  vec![IntLit(0___0,  Base::Dec), Eof]); // 0___0
	test_ok("0x123",  vec![IntLit(0x123,  Base::Hex), Eof]);
	test_ok("0b1001", vec![IntLit(0b1001, Base::Bin), Eof]);
}

#[test]
fn bad_numbers() {
	test_err("123a",        LexErrorKind::InvalidChar,   (1, 4));
	test_err("0x",          LexErrorKind::IncompleteInt, (1, 3));
	test_err("0x ",         LexErrorKind::IncompleteInt, (1, 3));
	test_err("0xz",         LexErrorKind::IncompleteInt, (1, 3));
	test_err("0x____",      LexErrorKind::IncompleteInt, (1, 3));
	test_err("0x1z",        LexErrorKind::InvalidChar,   (1, 4));
	test_err("0b",          LexErrorKind::IncompleteInt, (1, 3));
	test_err("0b ",         LexErrorKind::IncompleteInt, (1, 3));
	test_err("0b2",         LexErrorKind::IncompleteInt, (1, 3));
	test_err("0b12",        LexErrorKind::InvalidChar,   (1, 4));
	test_err("0x100000000", LexErrorKind::IntOutOfRange, (1, 1));
	test_err("99999999999", LexErrorKind::IntOutOfRange, (1, 1));
}

#[test]
fn all_together() {
	test_ok(r##"
		fn main() {
			let x = 0xBEEF; // code
			let y = "hello";

			if x <= 0xFEED { // me, seymour!
				println(y);
			}
		}"##, vec![
		Fn, id("main"), LParen, RParen, LBrace,
			Let, id("x"), Assign, IntLit(0xBEEF, Base::Hex), Semi,
			Let, id("y"), Assign, strlit("hello"), Semi,

			If, id("x"), LessEq, IntLit(0xFEED, Base::Hex), LBrace,
				id("println"), LParen, id("y"), RParen, Semi,
			RBrace,
			RBrace, Eof
		]
	);
}

// ------------------------------------------------------------------------------------
// Test harness functions
// ------------------------------------------------------------------------------------

const TEST_FILENAME: &str = "<string>";

fn test_impl(input: &str, expected_res: Result<Vec<TokenKind>, LexError>) {
	let src = Source::from_str(input, TEST_FILENAME);

	match (expected_res, Lexer::lex(&src)) {
		(Ok(expected), Ok(actual)) => {
			let token_kinds = actual.iter().map(|t| t.kind.clone()).collect::<Vec<_>>();

			if token_kinds.iter().filter(|&t| *t == Eof).count() != 1 {
				panic!("\non '{}', your code somehow produced multiple Eof tokens: '{:?}'",
					input, actual);
			}

			if *token_kinds.last().unwrap() != Eof {
				panic!("\non '{}', the last token should be Eof, but it isn't: '{:?}'",
					input, actual);
			}

			if token_kinds != expected {
				panic!("\non '{}', expected tokens:\n\t{:?}\nbut your code gave these tokens:\n\t{:?}",
					input, expected, actual);
			}
		}
		(Err(expected), Ok(actual)) => {
			panic!("\non '{}', expected error:\n\t{:?}\nbut your code succeeded and gave tokens '{:?}'",
				input, expected, actual);
		}
		(Ok(expected), Err(actual)) => {
			panic!("\non '{}', expected tokens '{:?}', but your code gave an error:\n\t{:?}",
				input, expected, actual);
		}
		(Err(expected), Err(actual)) => {
			if actual != expected {
				panic!("\non '{}', expected error:\n\t{:?}\nbut your code gave this error:\n\t{:?}",
					input, expected, actual);
			}
		}
	}
}

fn test_ok(input: &str, expected: Vec<TokenKind>) {
	test_impl(input, Ok(expected));
}

fn test_err(input: &str, kind: LexErrorKind, (line, col): (usize, usize)) {
	let loc = Location::new(TEST_FILENAME, line, col);
	test_impl(input, Err(LexError { loc, kind }));
}

fn id(s: &str) -> TokenKind {
	TokenKind::Id(s.into())
}

fn strlit(s: &str) -> TokenKind {
	TokenKind::StrLit(s.into())
}