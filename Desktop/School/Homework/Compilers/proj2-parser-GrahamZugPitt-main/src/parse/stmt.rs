
use crate::*;


impl Parser<'_> {
	// Stmt: BlockStmt | IfStmt | WhileStmt | ForStmt | ExpStmt | AssignStmt | ReturnStmt | LetStmt
	pub(crate) fn parse_stmt(&mut self) -> ParseResult<Box<Stmt>> {
		match self.cur(){

			TokenKind::LBrace => {self.parse_block()}

			TokenKind::Return => {self.parse_return_statement()}

			TokenKind::While => {self.parse_while_statement()}

			TokenKind::For => {self.parse_for_statement()}
			
			TokenKind::If => {self.next(); self.parse_if_statement()}

			TokenKind::Let => {self.parse_let_statement()}
			
		
			_ => {self.parse_expression_and_assignment()}
		}		
	}

	fn parse_let_statement(&mut self) -> ParseResult<Box<Stmt>>{
		let decl = self.parse_var_decl()?;
		Ok(Stmt::new_let(decl))
	}

	fn parse_for_statement(&mut self) -> ParseResult<Box<Stmt>>{
		self.next();
		let name = self.parse_ident()?;
		self.expect(TokenKind::In)?;
		let exp = self.parse_exp()?;
		self.expect(TokenKind::Comma)?;
		Ok(Stmt::new_for(VarDecl::new(name, exp), self.parse_exp()?, self.parse_block()?))
		
		
	}

	fn parse_while_statement(&mut self) -> ParseResult<Box<Stmt>>{
		self.next();
		let exp = self.parse_exp()?;
		let block = self.parse_block()?;
		Ok(Stmt::new_while(exp,block))
	}

	fn parse_if_statement(&mut self) -> ParseResult<Box<Stmt>>{
		let exp = self.parse_exp()?;
		let block = self.parse_block()?;
		let _else = self.expect(TokenKind::Else);
		match _else{
			Ok(okay) => {Ok(Stmt::new_if(exp,block,Some(self.parse_else_statement()?)))}

			Err(error) => {Ok(Stmt::new_if(exp,block,None))}
		}
	}
	
	fn parse_else_statement(&mut self) -> ParseResult<Box<Stmt>>{
		if self.cur() == TokenKind::LBrace {
			Ok(self.parse_block()?)
		} else {
			self.expect(TokenKind::If)?;
			Ok(self.parse_if_statement()?)
		} 
	}

	fn parse_return_statement(&mut self) -> ParseResult<Box<Stmt>>{
		self.next();
		let semi = self.expect(TokenKind::Semi);
		match semi{
			Ok(okay) => {Ok(Stmt::new_return(None))}

			Err(error) => {let exp = self.parse_exp()?; self.expect(TokenKind::Semi)?; Ok(Stmt::new_return(Some(exp)))}
		}
		
	}

	fn check_end_of_block(&mut self) -> ParseResult<()>{
		if self.cur() == TokenKind::RBrace{
			return Ok(())
		}
		return Err(ParseError::expected_token(self.cur_loc(), TokenKind::RBrace, self.cur()))
	}

	pub fn parse_block(&mut self) -> ParseResult<Box<Stmt>>{
    		self.expect(TokenKind::LBrace)?;

    		let mut args = Vec::new();

   	 	while self.cur() != TokenKind::RBrace && self.cur() != TokenKind::Eof{
       			args.push(self.parse_stmt()?);
				if args[args.len() - 1].returns(){
					break;
				}
        }
    

    	self.expect(TokenKind::RBrace)?;
    	Ok(Stmt::new_block(args))
	}

	fn parse_expression_and_assignment(&mut self) -> ParseResult<Box<Stmt>>{
		let exp = self.parse_exp()?;
		let assign = self.expect(TokenKind::Assign);
		match assign{
			Ok(ass) => {let exp_2 = self.parse_exp()?; let ret = Stmt::new_assign(exp, exp_2); self.expect(TokenKind::Semi)?; Ok(ret)}

			Err(error) => {self.expect(TokenKind::Semi)?; Ok(Stmt::new_exp(exp))}
		}
	}

	fn parse_expression_exclusively(&mut self) -> ParseResult<Box<Stmt>>{
		let exp = self.parse_exp()?;
		self.expect(TokenKind::Semi)?; 
		Ok(Stmt::new_exp(exp))
	}
}
