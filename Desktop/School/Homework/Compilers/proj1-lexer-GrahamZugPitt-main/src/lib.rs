#![allow(dead_code)]

pub mod error;
pub mod lex;

pub use error::*;
pub use lex::*;

// the tiniest compiler.
pub struct Compiler;

impl Compiler {
	pub fn lex(src: &Source) -> CompileResult<Vec<Token>> {
		Ok(Lexer::lex(src)?)
	}
}