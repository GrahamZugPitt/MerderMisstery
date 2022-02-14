
use crate::*;

use std::fmt::{ Display, Formatter, Result as FmtResult };

#[derive(Debug, PartialEq, Eq)]
pub enum CompileErrorKind {
	Lex(LexErrorKind),
	// FFFFFFUUUUTUUUUUUREEEEEE!!!!!
	// Parse(ParseError),
	// Sem(SemError),
}

#[derive(Debug, PartialEq, Eq)]
pub struct CompileError {
	pub loc:  Location,
	pub kind: CompileErrorKind,
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
			Lex(e) => e.to_string(),
		})
	}
}

impl std::error::Error for CompileError {}

pub type CompileResult<T> = Result<T, CompileError>;