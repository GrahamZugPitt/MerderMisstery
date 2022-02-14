
use crate::*;

use std::fmt::{ Display, Formatter, Result as FmtResult };

#[derive(Debug, PartialEq, Eq)]
pub enum CompileErrorKind {
	Lex(LexErrorKind),
	Parse(ParseErrorKind),
	Sem(SemErrorKind),
}

#[derive(Debug, PartialEq, Eq)]
pub struct CompileError {
	pub loc:  Location,
	pub kind: CompileErrorKind,
}

impl CompileError {
	pub(crate) fn from_parse_error(e: ParseError, t: &[Token], src: &Source) -> Self {
		let offs = t[e.token_idx].span.start;

		Self {
			loc: src.location_from_offset(offs),
			kind: CompileErrorKind::Parse(e.kind),
		}
	}

	pub(crate) fn from_sem_error(e: SemError) -> Self {
		Self {
			// TODO: this. lol. I fucked up and forgot to add location info to the AST.
			loc: Location::new("<???>", 0, 0),
			kind: CompileErrorKind::Sem(e.kind),
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
			Sem(e)   => e.to_string(),
		})
	}
}

impl std::error::Error for CompileError {}

pub type CompileResult<T> = Result<T, CompileError>;