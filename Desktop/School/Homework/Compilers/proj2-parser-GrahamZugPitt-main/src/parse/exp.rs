
use crate::*;

pub type ParseExpResult = ParseResult<Box<Exp>>;

impl Parser<'_> {
	// Exp: Term (BinOp Term)*
	pub(crate) fn parse_exp(&mut self) -> ParseExpResult {
		let lhs = self.parse_term()?;
		self.parse_binops(lhs, Precedence::MIN)
	}

	// the (BinOp Term)* portion of Exp.
	fn parse_binops(&mut self, mut lhs: Box<Exp>, min_precedence: Precedence) -> ParseExpResult {
		while self.cur().precedence() >= min_precedence {
			let op = self.cur();
			self.next();

			let mut rhs = self.parse_term()?;

			while self.cur().precedence() > op.precedence() {
				rhs = self.parse_binops(rhs, self.cur().precedence())?;
			}

			lhs = Exp::new_binary(lhs, op.to_binop(), rhs);
		}

		Ok(lhs)
	}

	// Term: UnOp* Primary Postfix*
	fn parse_term(&mut self) -> ParseExpResult {
		match self.cur(){
			TokenKind::Minus => {
				self.next();
				let operand = self.parse_term()?;
				Ok(Exp::new_unary(UnOp::Neg,operand))
			}
			TokenKind::Not => {
				self.next();
				let operand = self.parse_term()?;
				Ok(Exp::new_unary(UnOp::Not,operand))
			}
			_ => {

				let pri = self.parse_primary()?;
				self.parse_postfix(pri)
			}
		}
	}

	fn parse_primary(&mut self) -> ParseExpResult {
		match self.cur(){

			TokenKind::Id(name) => {self.next(); Ok(Exp::new_id(&name))}

			TokenKind::IntLit(int_lit, _) => {self.next(); Ok(Exp::new_int_lit(int_lit))}

			TokenKind::StrLit(str_lit) => {self.next(); Ok(Exp::new_str_lit(&str_lit))}

			TokenKind::True => {self.next(); Ok(Exp::new_bool_lit(true))}

			TokenKind::False => {self.next(); Ok(Exp::new_bool_lit(false))}

			TokenKind::LParen => {
					self.next(); 
					let ret = self.parse_exp()?; 
					self.expect(TokenKind::RParen)?;
					Ok(ret)
			}
			TokenKind::New => {
				self.expect(TokenKind::New)?; 
				let name = self.parse_ident()?;
				let args = self.parse_arg_list();
				match args{
					Ok(arg) => {Ok(Exp::new_ctor(AstType::new_struct(name), arg))}
					Err(error) => {Err(error)}
				}	
			}
			_ => Err(ParseError::bad_expression(self.cur_loc(), self.cur())),
		}
	}



	fn parse_postfix(&mut self, mut lhs: Box<Exp>) -> ParseExpResult {
		loop {
			match self.cur() {

			TokenKind::Dot => {
					self.next();
					match self.cur(){
						TokenKind::Id(name) => {let args = self.parse_ident(); lhs = Exp::new_field(lhs,args.unwrap())}
						_ => {return Err(ParseError::expected_name(self.cur_loc(), self.cur()))}
						}
					}
			TokenKind::LParen => {
				let args = self.parse_arg_list()?;
				lhs = Exp::new_func_call(lhs,args);
			} 
		

			_ => break,
			}

		}
		
		Ok(lhs)
		
	}

	fn parse_arg_list(&mut self) -> ParseResult<Vec<Box<Exp>>> {
	// '('
    		self.expect(TokenKind::LParen)?;

    		let mut args = Vec::new();

    	// this 'if' handles the '?' bit of the grammar rule
   	 	if self.cur() != TokenKind::RParen {
    		// Exp
       			args.push(self.parse_exp()?);

        // (',' Exp)*
        		while self.cur() == TokenKind::Comma {
            		self.next();
            		args.push(self.parse_exp()?);
        }
    }

    // ')'
    	self.expect(TokenKind::RParen)?;
    	Ok(args)
}
}
