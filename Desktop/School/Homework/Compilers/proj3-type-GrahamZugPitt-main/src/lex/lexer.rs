
use colored::Colorize;
use crate::*;

pub(crate) struct Lexer {
}

impl Lexer {
	pub(crate) fn lex(src: &Source) -> LexResult<Vec<Token>> {
		println!("{}", "Hey, this is the SEMANTIC project.".red());
		println!("{}", "If you completed the lexer in project 1, drop your completed".red());
		println!("{}", "lexer.rs into the src/lex directory to use it.".red());

		Err(LexError::invalid_char(src.location_from_offset(0)))
	}
}