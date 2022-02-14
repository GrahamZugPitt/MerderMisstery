
use colored::Colorize;
use crate::*;

impl Parser<'_> {
	pub(crate) fn parse_decl(&mut self) -> ParseResult<Box<Decl>> {
		println!("{}", "Hey, this is the CODEGEN project.".red());
		println!("{}", "If you completed the parser in project 2, drop".red());
		println!("{}", "your completed exp.rs, decl.rs, and stmt.rs into".red());
		println!("{}", "the src/parse directory to use it.".red());

		Err(ParseError::extra_tokens(self.cur_loc(), self.cur()))
	}

	#[allow(dead_code)]
	pub(crate) fn parse_type(&mut self) -> ParseResult<Box<AstType>> {
		match self.cur() {
			TokenKind::LParen => {
				self.next();
				self.expect(TokenKind::RParen)?;
				Ok(AstType::new_void())
			}

			TokenKind::Bool => {
				self.next();
				Ok(AstType::new_bool())
			}

			TokenKind::Int => {
				self.next();
				Ok(AstType::new_int())
			}

			TokenKind::String => {
				self.next();
				Ok(AstType::new_string())
			}

			TokenKind::Fn => {
				self.next();
				self.expect(TokenKind::LParen)?;
				let mut args = Vec::<Box<AstType>>::new();

				if self.cur() != TokenKind::RParen {
					args.push(self.parse_type()?);

					while self.cur() == TokenKind::Comma {
						self.next();
						args.push(self.parse_type()?);
					}
				}

				self.expect(TokenKind::RParen)?;
				self.expect(TokenKind::Colon)?;

				let ret = self.parse_type()?;
				Ok(AstType::new_func(args, ret))
			}

			TokenKind::Id(n) => {
				self.next();
				Ok(AstType::new_struct(Ident::new(&n)))
			}

			_ => Err(ParseError::expected_type(self.cur_loc(), self.cur())),
		}
	}
}