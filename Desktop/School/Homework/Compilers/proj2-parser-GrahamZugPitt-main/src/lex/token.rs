
use std::fmt::{ Display, Formatter, Result as FmtResult };
use std::ops::{ Range };

#[derive(Debug, PartialEq, Eq, Copy, Clone)]
pub enum Base {
	Dec = 10,
	Bin = 2,
	Hex = 16,
}

#[derive(Debug, PartialEq, Eq, Clone)]
pub enum TokenKind {
	// "end-of-file" - used as a sentinel value at the end of the token stream.
	Eof,

	// keywords
	If,          // if
	Else,        // else
	For,         // for
	In,          // in
	Fn,          // fn
	Let,         // let
	While,       // while
	Break,       // break
	Return,      // return
	Int,         // int
	Bool,        // bool
	String,      // string
	And,         // and
	Or,          // or
	Not,         // not
	True,        // true
	False,       // false
	Struct,      // struct
	New,         // new

	// operators
	Assign,      // =
	Plus,        // +
	Minus,       // -
	Times,       // *
	Divide,      // /
	Modulo,      // %
	Shl,         // <<
	Shr,         // >>
	Less,        // <
	Greater,     // >
	LessEq,      // <=
	GreaterEq,   // >=
	Eq,          // ==
	NotEq,       // !=

	// other symbols
	LParen,      // (
	RParen,      // )
	LBrace,      // {
	RBrace,      // }
	LBracket,    // [
	RBracket,    // ]
	Semi,        // ;
	Colon,       // :
	Comma,       // ,
	Dot,         // .

	// more complex tokens
	Id(String),        // identifiers (names of variables, functions etc)
	IntLit(i64, Base), // integer value + base it was written in
	StrLit(String),    // quoted strings
}

impl Display for TokenKind {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use TokenKind::*;

		match self {
			Eof          => write!(f, "<EOF>"),

			If           => write!(f, "if"),
			Else         => write!(f, "else"),
			For          => write!(f, "for"),
			In           => write!(f, "in"),
			Fn           => write!(f, "fn"),
			Let          => write!(f, "let"),
			While        => write!(f, "while"),
			Break        => write!(f, "break"),
			Return       => write!(f, "return"),
			Int          => write!(f, "int"),
			Bool         => write!(f, "bool"),
			String       => write!(f, "string"),
			And          => write!(f, "and"),
			Or           => write!(f, "or"),
			Not          => write!(f, "not"),
			True         => write!(f, "true"),
			False        => write!(f, "false"),
			Struct       => write!(f, "struct"),
			New          => write!(f, "new"),

			Assign       => write!(f, "="),
			Plus         => write!(f, "+"),
			Minus        => write!(f, "-"),
			Times        => write!(f, "*"),
			Divide       => write!(f, "/"),
			Modulo       => write!(f, "%"),
			Shl          => write!(f, "<<"),
			Shr          => write!(f, ">>"),
			Less         => write!(f, "<"),
			Greater      => write!(f, ">"),
			LessEq       => write!(f, "<="),
			GreaterEq    => write!(f, ">="),
			Eq           => write!(f, "=="),
			NotEq        => write!(f, "!="),

			LParen       => write!(f, "("),
			RParen       => write!(f, ")"),
			LBrace       => write!(f, "{{"),
			RBrace       => write!(f, "}}"),
			LBracket     => write!(f, "["),
			RBracket     => write!(f, "]"),
			Semi         => write!(f, ";"),
			Colon        => write!(f, ":"),
			Comma        => write!(f, ","),
			Dot          => write!(f, "."),

			Id(s)        => write!(f, "{}", s),
			StrLit(s)    => write!(f, "\"{}\"", s),

			IntLit(x, Base::Dec) => write!(f, "{}", x),
			IntLit(x, Base::Hex) => write!(f, "0x{:X}", x),
			IntLit(x, Base::Bin) => write!(f, "0b{:b}", x),
		}
	}
}

#[derive(Debug, PartialEq, Eq, Clone)]
pub struct Token {
	pub span: Range<usize>,
	pub kind: TokenKind,
}

impl Display for Token {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		write!(f, "{}", self.kind)
	}
}