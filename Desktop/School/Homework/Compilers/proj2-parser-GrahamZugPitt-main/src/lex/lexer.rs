
use crate::*;

pub(crate) struct Lexer<'src> {
	iter:   SourceIter<'src>,
	tokens: Vec<Token>,
}

// Helper functions for testing characters.
fn is_dec_digit_start(c: char) -> bool { c.is_ascii_digit() }
fn is_hex_digit_start(c: char) -> bool { c.is_ascii_hexdigit() }
fn is_bin_digit_start(c: char) -> bool { c == '0' || c == '1' }

fn is_digit_cont(c: char, base: Base) -> bool {
	match base {
		Base::Dec => is_dec_digit_start(c) || c == '_',
		Base::Hex => is_hex_digit_start(c) || c == '_',
		Base::Bin => is_bin_digit_start(c) || c == '_',
	}
}

fn is_ident_start(c: char) -> bool { c.is_alphabetic() || c == '$' || c == '_' }
fn is_ident_cont (c: char) -> bool { is_ident_start(c) || c.is_ascii_digit() }

impl<'src> Lexer<'src> {
	pub(crate) fn lex(src: &Source) -> LexResult<Vec<Token>> {
		let mut l = Lexer::new(src);
		l.lex_all()?;
		Ok(l.tokens)
	}

	fn new(src: &'src Source) -> Lexer<'src> {
		Lexer {
			iter:   src.iter(),
			tokens: Vec::new(),
		}
	}

	fn cur(&self) -> char  { self.iter.cur() }
	fn peek(&self) -> char { self.iter.peek() }
	fn next(&mut self)     { self.iter.next() }
	fn next_2(&mut self)   { self.iter.next_2() }

	fn lex_all(&mut self) -> LexResult<()> {
		let mut is_eof = false;

		while !is_eof {
			self.eat_whitespace_and_comments();

			let start  = self.iter.pos();
			let kind   = self.next_token()?;
			let end    = self.iter.pos();
			is_eof     = kind == TokenKind::Eof;

			assert!(end > start || is_eof, "next_token() is stuck!");
			self.tokens.push(Token { span: start .. end, kind });
		}

		Ok(())
	}

	fn eat_whitespace_and_comments(&mut self) {
		loop {
			match self.cur() {
				// whitespace
				' ' | '\t' | '\n' => { self.next(); }

				// comments
				'/' if self.peek() == '/' => {
					loop {
						match self.cur() {
							'\n' | '\0' => break,
							_    => self.next(),
						}
					}
				}

				// something tokeny?
				_ => break,
			}
		}
	}

	fn next_token(&mut self) -> LexResult<TokenKind> {
		match self.cur() {
			// EOF
			'\0' => Ok(TokenKind::Eof),

			'+' => {self.next(); Ok(TokenKind::Plus) }

			'-' => {self.next(); Ok(TokenKind::Minus) }

			'*' => {self.next(); Ok(TokenKind::Times) }

			'/' => {self.next(); Ok(TokenKind::Divide) }

			'%' => {self.next(); Ok(TokenKind::Modulo) }

			'(' => {self.next(); Ok(TokenKind::LParen) }

			')' => {self.next(); Ok(TokenKind::RParen) }

			'{' => {self.next(); Ok(TokenKind::LBrace) }

			'}' => {self.next(); Ok(TokenKind::RBrace) }

			'[' => {self.next(); Ok(TokenKind::LBracket) }

			']' => {self.next(); Ok(TokenKind::RBracket) }

			';' => {self.next(); Ok(TokenKind::Semi) }

			':' => {self.next(); Ok(TokenKind::Colon) }

			',' => {self.next(); Ok(TokenKind::Comma) }

			'.' => {self.next(); Ok(TokenKind::Dot) }
			
			'!' => {if self.peek() != '='  			 
				{Err(LexError::invalid_char(self.iter.loc()))}
				else
				{self.next_2(); Ok(TokenKind::NotEq)} 
				}
			'=' => {match self.peek() {
					'=' => {self.next_2(); Ok(TokenKind::Eq) }

					_ => {self.next(); Ok(TokenKind::Assign) }
					}
				}
			'<' => {match self.peek() {
					'=' => {self.next_2(); Ok(TokenKind::LessEq) }

					'<' => {self.next_2(); Ok(TokenKind::Shl) }

					_ => {self.next(); Ok(TokenKind::Less) }
					}
				}
			'>' => {match self.peek() {
					'=' => {self.next_2(); Ok(TokenKind::GreaterEq) }

					'>' => {self.next_2(); Ok(TokenKind::Shr) }

					_ => {self.next(); Ok(TokenKind::Greater) }
					}
				}
			c if is_ident_start(c) => self.lex_ident(),
			
			c if is_dec_digit_start(c) => self.lex_integer(),

			'"' => self.lex_string(),
			

			// TODO: every other kind of token
		

			// anything else is invalid.
			_ => Err(LexError::invalid_char(self.iter.loc()))
		}
	}

	fn lex_ident(&mut self) -> LexResult<TokenKind> {
		let mut s = String::new();
		while is_ident_cont(self.peek()){
			s.push(self.cur());
			self.next();
		}
		s.push(self.cur());
		self.next();
		Ok(match s.as_str(){
			"if" => {TokenKind::If}

			"else" => {TokenKind::Else}

			"for" => {TokenKind::For}

			"in" => {TokenKind::In}

			"fn" => {TokenKind::Fn}

			"let" => {TokenKind::Let}

			"while" => {TokenKind::While}

			"break" => {TokenKind::Break}

			"return" => {TokenKind::Return}

			"int" => {TokenKind::Int}

			"bool" => {TokenKind::Bool}

			"string" => {TokenKind::String}

			"and" => {TokenKind::And}

			"or" => {TokenKind::Or}

			"not" => {TokenKind::Not}

			"true" => {TokenKind::True}

			"false" => {TokenKind::False}

			"struct" => {TokenKind::Struct}

			"new" => {TokenKind::New}

			_ => TokenKind::Id(s)
		})
	}

	fn lex_string(&mut self) -> LexResult<TokenKind> {
		let first_char = self.iter.loc();
		let mut s = String::new();
		self.next();
		while self.cur() != '"'{
			match self.cur(){
				'\0' | '\n' => return Err(LexError::unclosed_string(first_char)),

				'\\' => {match self.peek(){
						'\\' => s.push('\\'),
	
						'\"' => s.push('"'),
	
						't' => s.push('\t'),
	
						'r' => s.push('\r'),
	
						'n' => s.push('\n'),

						_ => return Err(LexError::unknown_escape(self.iter.loc()))
						}
					self.next();
					},
				
				_ => s.push(self.cur()),
			}
			self.next();
		}
		self.next();
		Ok(TokenKind::StrLit(s))
	}

	fn lex_integer(&mut self) -> LexResult<TokenKind> {
		let first_char = self.iter.loc();
		let mut s = String::new();
			match self.cur(){
				'0' => {
					match self.peek(){

						'x' | 'X' => {self.next_2();	
								if !is_hex_digit_start(self.cur()){
									return Err(LexError::incomplete_int(self.iter.loc()));
									}	
								while is_hex_digit_start(self.cur()) || self.cur() == '_'{ //decimal case
							
								if self.cur() != '_' {
									s.push(self.cur());
									}
								self.next();
								}
								if self.cur().is_alphabetic() && !is_hex_digit_start(self.cur()) { 
									return Err(LexError::invalid_char(self.iter.loc()));
								}
								let final_int;
								match i64::from_str_radix(&s, 16){
									Err(_) => {return Err(LexError::int_range(first_char));}
									_ => final_int = i64::from_str_radix(&s, 16).unwrap()
								}
								if final_int > u32::MAX as i64{
									return Err(LexError::int_range(first_char));
								}
									return Ok(TokenKind::IntLit(final_int, token::Base::Hex));
								}

						'b' | 'B' => {self.next_2();	
								if !is_bin_digit_start(self.cur()){
									return Err(LexError::incomplete_int(self.iter.loc()));
									}							
								while is_bin_digit_start(self.cur()) || self.cur() == '_'{ //decimal case
							
								if self.cur() != '_' {
									s.push(self.cur());
									}
								self.next();
								}
								if self.cur().is_alphabetic() && !is_hex_digit_start(self.cur()) { 
									return Err(LexError::invalid_char(self.iter.loc()));
								}
								if is_dec_digit_start(self.cur()) && !is_bin_digit_start(self.cur()) { 
									return Err(LexError::invalid_char(self.iter.loc()));
								}
								let final_int;
								match i64::from_str_radix(&s, 2){
									Err(_) => {return Err(LexError::int_range(first_char));}
									_ => final_int = i64::from_str_radix(&s, 2).unwrap()
								}
								if final_int > u32::MAX as i64{
									return Err(LexError::int_range(first_char));
								}
									return Ok(TokenKind::IntLit(final_int, token::Base::Bin));
								}

						

						c if c.is_alphabetic() => {self.next(); return Err(LexError::invalid_char(self.iter.loc()));}

						_ => {while is_dec_digit_start(self.cur()) || self.cur() == '_'{ //decimal case
							
							if self.cur() != '_' {
								s.push(self.cur());
								}
							self.next();
							}
							if self.cur().is_alphabetic(){ 
								return Err(LexError::invalid_char(self.iter.loc()));
							}
							let final_int;
							match i64::from_str_radix(&s, 10){
								Err(_) => {return Err(LexError::int_range(first_char));}
								_ => final_int = i64::from_str_radix(&s, 10).unwrap()
							}
							if final_int > u32::MAX as i64{
								return Err(LexError::int_range(first_char));
							}
								return Ok(TokenKind::IntLit(final_int, token::Base::Dec));
							}
						}
					}
					

				'1' ..= '9' => {while is_dec_digit_start(self.cur()) || self.cur() == '_'{ //decimal case
							
							if self.cur() != '_' {
								s.push(self.cur());
								}
							self.next();
							}
							if self.cur().is_alphabetic(){ 
								return Err(LexError::invalid_char(self.iter.loc()));
							}
							let final_int;
							match i64::from_str_radix(&s, 10){
								Err(_) => {return Err(LexError::int_range(first_char));}
								_ => final_int = i64::from_str_radix(&s, 10).unwrap()
							}
							if final_int > u32::MAX as i64{
								return Err(LexError::int_range(first_char));
							}
								return Ok(TokenKind::IntLit(final_int, token::Base::Dec));
							}
				_ => {return Err(LexError::invalid_char(self.iter.loc()));}
			}
		
	}
}
