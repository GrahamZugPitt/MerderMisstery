
use crate::*;

pub(crate) struct Parser<'t> {
	tokens: &'t [Token],
	i:      usize,
}

impl<'t> Parser<'t> {
	// Program: Decl*
	pub(crate) fn parse_program(tokens: &'t [Token]) -> ParseResult<Program> {
		let mut p = Self::new(tokens);

		let mut items = Vec::new();
		while p.cur() != TokenKind::Eof {
			items.push(p.parse_decl()?);
		}

		p.expect(TokenKind::Eof)?;

		Ok(Program { items })
	}

	// Convenience methods for testing the parser.
	pub(crate) fn parse_one_stmt(tokens: &'t [Token]) -> ParseResult<Box<Stmt>> {
		let mut p = Self::new(tokens);
		let ret = p.parse_stmt()?;
		p.expect(TokenKind::Eof)?;
		Ok(ret)
	}

	#[cfg(test)]
	pub(crate) fn parse_one_exp(tokens: &'t [Token]) -> ParseResult<Box<Exp>> {
		let mut p = Self::new(tokens);
		let ret = p.parse_exp()?;
		p.expect(TokenKind::Eof)?;
		Ok(ret)
	}

	#[cfg(test)]
	pub(crate) fn parse_one_decl(tokens: &'t [Token]) -> ParseResult<Box<Decl>> {
		let mut p = Self::new(tokens);
		let ret = p.parse_decl()?;
		p.expect(TokenKind::Eof)?;
		Ok(ret)
	}

	#[cfg(test)]
	pub(crate) fn parse_one_type(tokens: &'t [Token]) -> ParseResult<Box<AstType>> {
		let mut p = Self::new(tokens);
		let ret = p.parse_type()?;
		p.expect(TokenKind::Eof)?;
		Ok(ret)
	}

	fn new(tokens: &'t [Token]) -> Self {
		Parser { tokens, i: 0 }
	}

	// Helper methods.
	pub(crate) fn cur(&self) -> TokenKind {
		if self.i < self.tokens.len() {
			self.tokens[self.i].kind.clone()
		} else {
			TokenKind::Eof
		}
	}

	pub(crate) fn cur_loc(&self) -> usize {
		self.i
	}

	pub(crate) fn next(&mut self) {
		self.i += 1
	}

	pub(crate) fn expect(&mut self, expected: TokenKind) -> ParseResult<()> {
		let cur = self.cur();

		if cur != expected {
			if cur == TokenKind::Eof {
				return Err(ParseError::unexpected_eof(self.cur_loc()));
			} else if expected == TokenKind::Eof {
				return Err(ParseError::extra_tokens(self.cur_loc(), cur))
			} else {
				return Err(ParseError::expected_token(self.cur_loc(), expected, cur));
			}
		}

		if expected != TokenKind::Eof {
			self.next();
		}

		Ok(())
	}

	pub(crate) fn parse_ident(&mut self) -> ParseResult<Ident> {
		match self.cur() {
			TokenKind::Id(name) => { self.next(); Ok(Ident::new(&name)) }
			_ => Err(ParseError::expected_name(self.cur_loc(), self.cur())),
		}
	}
}