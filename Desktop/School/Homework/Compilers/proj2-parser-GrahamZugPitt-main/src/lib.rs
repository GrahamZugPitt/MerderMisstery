
pub mod error;
pub mod lex;
pub mod parse;
// pub mod sem;
// pub mod ir;
// pub mod opt;
// pub mod cg;

pub use error::*;
pub use lex::*;
pub use parse::*;
// pub use sem::*;
// pub use ir::*;
// pub use opt::*;
// pub use cg::*;

pub struct Compiler;

impl Compiler {
	pub fn lex(src: &Source) -> CompileResult<Vec<Token>> {
		Ok(Lexer::lex(src)?)
	}

	pub fn lex_and_parse(src: &Source) -> CompileResult<Program> {
		let tokens = Self::lex(src)?;

		match Parser::parse_program(&tokens) {
			Ok(ast) => Ok(ast),
			Err(e)  => Err(CompileError::from_parse_error(e, src)),
		}
	}

	pub fn lex_and_parse_stmt(src: &Source) -> CompileResult<Box<Stmt>> {
		let tokens = Self::lex(src)?;

		match Parser::parse_one_stmt(&tokens) {
			Ok(ast) => Ok(ast),
			Err(e)  => Err(CompileError::from_parse_error(e, src)),
		}
	}
}