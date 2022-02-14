
use std::fmt::{ Display, Formatter, Result as FmtResult };

use crate::*;

#[derive(Debug, PartialEq, Eq)]
pub enum LexErrorKind {
	InvalidChar,
	UnclosedString,
	UnknownEscape,
	IntOutOfRange,
	IncompleteInt,
}

impl Display for LexErrorKind {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		use LexErrorKind::*;

		write!(f, "{}", match self {
			InvalidChar    => "invalid character",
			UnclosedString => "unclosed string literal",
			UnknownEscape  => "unknown escape character",
			IntOutOfRange  => "integer literal out of 32-bit range",
			IncompleteInt  => "incomplete integer literal",
		})
	}
}

#[derive(Debug, PartialEq, Eq)]
pub struct LexError {
	pub loc:  Location,
	pub kind: LexErrorKind,
}

impl LexError {
	// Constructors for creating error values with less typing.
	pub fn invalid_char(loc: Location) -> LexError {
		LexError { loc, kind: LexErrorKind::InvalidChar }
	}

	pub fn unclosed_string(loc: Location) -> LexError {
		LexError { loc, kind: LexErrorKind::UnclosedString }
	}

	pub fn unknown_escape(loc: Location) -> LexError {
		LexError { loc, kind: LexErrorKind::UnknownEscape }
	}

	pub fn int_range(loc: Location) -> LexError {
		LexError { loc, kind: LexErrorKind::IntOutOfRange }
	}

	pub fn incomplete_int(loc: Location) -> LexError {
		LexError { loc, kind: LexErrorKind::IncompleteInt }
	}
}

impl Display for LexError {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		write!(f, "{}: {}", self.loc, self.kind)
	}
}

impl std::error::Error for LexError {}

pub type LexResult<T> = Result<T, LexError>;