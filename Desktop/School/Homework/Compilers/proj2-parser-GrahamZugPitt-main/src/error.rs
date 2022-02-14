
use crate::*;

use std::fmt::{ Display, Formatter, Result as FmtResult };

#[derive(Debug, PartialEq, Eq)]
pub enum CompileErrorKind {
	Lex(LexErrorKind),
	Parse(ParseErrorKind),
	// Sem(SemErrorKind),
}

#[derive(Debug, PartialEq, Eq)]
pub struct CompileError {
	pub loc:  Location,
	pub kind: CompileErrorKind,
}

impl CompileError {
	pub(crate) fn from_parse_error(e: ParseError, src: &Source) -> Self {
		Self {
			loc: src.location_from_offset(e.token_idx),
			kind: CompileErrorKind::Parse(e.kind),
		}
	}
}

impl From<LexError> for CompileError {
	fn from(e: LexError) -> Self {
		Self {
			loc:  e.loc,
			kind: CompileErrorKind::Lex(e.kind)
		}
	}
}

impl Display for CompileError {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use CompileErrorKind::*;

		write!(f, "{}: {}", self.loc, match &self.kind {
			Lex(e)   => e.to_string(),
			Parse(e) => e.to_string(),
		})
	}
}

impl std::error::Error for CompileError {}

pub type CompileResult<T> = Result<T, CompileError>;