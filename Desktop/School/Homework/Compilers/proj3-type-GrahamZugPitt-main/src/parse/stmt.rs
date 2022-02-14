
use colored::Colorize;
use crate::*;

impl Parser<'_> {
	pub(crate) fn parse_stmt(&mut self) -> ParseResult<Box<Stmt>> {
		println!("{}", "Hey, this is the SEMANTIC project.".red());
		println!("{}", "If you completed the parser in project 2, drop".red());
		println!("{}", "your completed exp.rs, decl.rs, and stmt.rs into".red());
		println!("{}", "the src/parse directory to use it.".red());

		Err(ParseError::extra_tokens(self.cur_loc(), self.cur()))
	}
}