
use colored::Colorize;
use crate::*;

pub type ParseExpResult = ParseResult<Box<Exp>>;

impl Parser<'_> {
	pub(crate) fn parse_exp(&mut self) -> ParseExpResult {
		println!("{}", "Hey, this is the CODEGEN project.".red());
		println!("{}", "If you completed the parser in project 2, drop".red());
		println!("{}", "your completed exp.rs, decl.rs, and stmt.rs into".red());
		println!("{}", "the src/parse directory to use it.".red());

		Err(ParseError::extra_tokens(self.cur_loc(), self.cur()))
	}
}