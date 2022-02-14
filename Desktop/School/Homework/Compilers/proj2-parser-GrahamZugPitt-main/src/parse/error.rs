
use std::fmt::{ Display, Formatter, Result as FmtResult };

use crate::*;

#[derive(Debug, PartialEq, Eq)]
pub enum ParseErrorKind {
	ExtraTokens(TokenKind),
	UnexpectedEof,
	BadExpression(TokenKind),
	BadStatement(TokenKind),
	ExpectedToken { exp: TokenKind, act: TokenKind },
	ExpectedName(TokenKind),
	ExpectedDecl(TokenKind),
	ExpectedType(TokenKind),
	NoReturn(String),
}

impl Display for ParseErrorKind {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use ParseErrorKind::*;

		match self {
			ExtraTokens(t)    => write!(f, "extra stuff at end of input: '{}' etc.", t),
			UnexpectedEof     => write!(f, "unexpected end-of-input."),
			BadExpression(t)  => write!(f, "expected expression, not '{}'.", t),
			BadStatement(t)   => write!(f, "expected statement, not '{}'.", t),
			ExpectedToken { exp, act } => write!(f, "expected '{}' token, not '{}'.", exp, act),
			ExpectedName(act) => write!(f, "expected a name, not '{}'.", act),
			ExpectedDecl(act) => write!(f, "expected 'let' or 'fn', not '{}'.", act),
			ExpectedType(act) => write!(f, "expected a type, not '{}'.", act),
			NoReturn(name)    => write!(f, "no 'return' at end of non-void function '{}'.", name),
		}
	}
}

#[derive(Debug, PartialEq, Eq)]
pub struct ParseError {
	pub token_idx: usize,
	pub kind: ParseErrorKind,
}

impl ParseError {
	pub fn extra_tokens(token_idx: usize, t: TokenKind) -> Self {
		Self { token_idx, kind: ParseErrorKind::ExtraTokens(t) }
	}

	pub fn unexpected_eof(token_idx: usize) -> Self {
		Self { token_idx, kind: ParseErrorKind::UnexpectedEof }
	}

	pub fn bad_expression(token_idx: usize, t: TokenKind) -> Self {
		Self { token_idx, kind: ParseErrorKind::BadExpression(t) }
	}

	pub fn bad_statement(token_idx: usize, t: TokenKind) -> Self {
		Self { token_idx, kind: ParseErrorKind::BadStatement(t) }
	}

	pub fn expected_token(token_idx: usize, exp: TokenKind, act: TokenKind) -> Self {
		Self { token_idx, kind: ParseErrorKind::ExpectedToken { exp, act } }
	}

	pub fn expected_name(token_idx: usize, t: TokenKind) -> Self {
		Self { token_idx, kind: ParseErrorKind::ExpectedName(t) }
	}

	pub fn expected_decl(token_idx: usize, t: TokenKind) -> Self {
		Self { token_idx, kind: ParseErrorKind::ExpectedDecl(t) }
	}

	pub fn expected_type(token_idx: usize, t: TokenKind) -> Self {
		Self { token_idx, kind: ParseErrorKind::ExpectedType(t) }
	}

	pub fn no_return(token_idx: usize, name: String) -> Self {
		Self { token_idx, kind: ParseErrorKind::NoReturn(name) }
	}

}

impl Display for ParseError {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		write!(f, "(token #{}): {}", self.token_idx, self.kind)
	}
}

impl std::error::Error for ParseError {}

pub type ParseResult<T> = Result<T, ParseError>;