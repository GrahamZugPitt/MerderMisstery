
use crate::*;

impl Parser<'_> {
	// Decl: VarDecl | FuncDecl | StructDecl
	pub(crate) fn parse_decl(&mut self) -> ParseResult<Box<Decl>> {
		match self.cur() {
			TokenKind::Let    => Ok(Decl::new_var(self.parse_var_decl()?)),
			TokenKind::Fn     => Ok(Decl::new_func(self.parse_func_decl(None)?)),
			TokenKind::Struct => Ok(Decl::new_struct(self.parse_struct_decl()?)),
			_ =>                 Err(ParseError::expected_decl(self.cur_loc(), self.cur())),
		}
	}

	// VarDecl: 'let' Id '=' Exp ';'
	pub(crate) fn parse_var_decl(&mut self) -> ParseResult<VarDecl> {
		self.expect(TokenKind::Let)?;
		let name = self.parse_ident()?;
		self.expect(TokenKind::Assign)?;
		let init = self.parse_exp()?;
		self.expect(TokenKind::Semi)?;
		Ok(VarDecl{name, init})
	}

	// FuncDecl: 'fn' Id '(' FuncArgs? ')' (':' Type)? BlockStmt
	fn parse_func_decl(&mut self, struct_: Option<&Ident>) -> ParseResult<FuncDecl> {
		let start_loc = self.cur_loc();
		let this = self.struct_name_to_this(struct_);
		self.expect(TokenKind::Fn)?;	
		let name = self.parse_ident()?;
		self.expect(TokenKind::LParen)?;
		let mut args = Vec::new();
		let mut first = true;
		while let TokenKind::Id(..) = self.cur(){
       			let name = self.parse_ident()?;
				self.expect(TokenKind::Colon)?;
				let ty = self.parse_type()?;
				args.push(FuncArg{name, ty});
				if self.cur() != TokenKind::RParen{
					self.expect(TokenKind::Comma)?;
					if let TokenKind::Id(..) = self.cur() {
					} else {
						return Err(ParseError::expected_name(self.cur_loc(), self.cur()));
					}
				}			
		}
		self.expect(TokenKind::RParen)?;
		if self.cur() == TokenKind::Colon{
			self.next();
			let return_type = self.parse_type()?;
			let code = self.parse_block()?;
			if *return_type != AstType::Void && !code.returns() {
				return Err(ParseError::no_return(start_loc, name.name.clone()));
			}	
			return Ok(FuncDecl{name, this, args, return_type, code});
		}
		let return_type = AstType::new_void();
		let code = self.parse_block()?;
		return Ok(FuncDecl{name, this, args, return_type, code});
	}

	// StructDecl: 'struct' Id (':' Id)? '{' FieldDecls FuncDecl* '}'
	//FieldDecls: FieldDecl (',' FieldDecl)* ','?
	//FieldDecl:  Id ':' Type
	fn parse_struct_decl(&mut self) -> ParseResult<StructDecl> {
		self.next();
		let name = self.parse_ident()?;
		if self.cur() == TokenKind::Colon {
			self.next();
			let base = Some(Box::new(AstType::Struct(self.parse_ident()?)));
			self.expect(TokenKind::LBrace)?;	
			let mut fields = Vec::new();
			let mut methods = Vec::new();
			while let TokenKind::Id(..) = self.cur(){
				let name = self.parse_ident()?;
				self.expect(TokenKind::Colon)?;
				let ty = self.parse_type()?;
				fields.push(Field{name, ty});
				if self.cur() != TokenKind::RBrace{
					self.expect(TokenKind::Comma)?;
					if let TokenKind::Id(..) = self.cur(){
					} else {
						if self.cur() != TokenKind::RBrace && self.cur() != TokenKind::Fn {
							return Err(ParseError::expected_name(self.cur_loc(), self.cur()));
						}
					}
				}
			}
	
			while self.cur() == TokenKind::Fn {
				let method = self.parse_func_decl(Some(&name))?;
				methods.push(method);
			}
			self.expect(TokenKind::RBrace)?;
			return Ok(StructDecl{name, base, fields, methods});			
		}
		self.expect(TokenKind::LBrace)?;
		let mut fields = Vec::new();
		let mut methods = Vec::new();
		while let TokenKind::Id(..) = self.cur(){
			let name = self.parse_ident()?;
			self.expect(TokenKind::Colon)?;
			let ty = self.parse_type()?;
			fields.push(Field{name, ty});
			if self.cur() != TokenKind::RBrace{
				self.expect(TokenKind::Comma)?;
				if let TokenKind::Id(..) = self.cur(){
				} else {
					if self.cur() != TokenKind::RBrace {
						return Err(ParseError::expected_name(self.cur_loc(), self.cur()));
					}
				}
			}
		}

		while self.cur() == TokenKind::Fn {
			let method = self.parse_func_decl(Some(&name))?;
			methods.push(method);
		}
		self.expect(TokenKind::RBrace)?;
		return Ok(StructDecl{name, base : None, fields, methods});

	}

	// given an optional identifier, constructs a FuncArg for it named 'this' and
	// using that identifier as the type of 'this'. (if given None, returns None.)
	fn struct_name_to_this(&self, struct_: Option<&Ident>) -> Option<FuncArg> {
		struct_.map(|struct_name| {
			FuncArg {
				name: Ident::new("this"),
				ty:   AstType::new_struct(struct_name.dup())
			}
		})
	}

	// Type:       VoidType | 'bool' | 'int' | 'string' | FuncType | StructType
	// VoidType:   '(' ')'
	// FuncType:   'fn' '(' (Type (',' Type)*)? ')' ':' Type
	// StructType: Id
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