#![deny(unused_must_use)] // makes forgetting ? a hard error

pub mod error;
pub mod lex;
pub mod parse;
pub mod sem;
// pub mod ir;
// pub mod opt;
// pub mod cg;

pub use error::*;
pub use lex::*;
pub use parse::*;
pub use sem::*;
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
			Err(e)  => Err(CompileError::from_parse_error(e, &tokens, src)),
		}
	}

	pub fn lex_and_parse_stmt(src: &Source) -> CompileResult<Box<Stmt>> {
		let tokens = Self::lex(src)?;

		match Parser::parse_one_stmt(&tokens) {
			Ok(ast) => Ok(ast),
			Err(e)  => Err(CompileError::from_parse_error(e, &tokens, src)),
		}
	}

	pub fn lex_and_parse_exp(src: &Source) -> CompileResult<Box<Exp>> {
		let tokens = Self::lex(src)?;

		match Parser::parse_one_exp(&tokens) {
			Ok(ast) => Ok(ast),
			Err(e)  => Err(CompileError::from_parse_error(e, &tokens, src)),
		}
	}

	pub fn sem_symbols(prog: &Program) -> CompileResult<NameCtx> {
		match SymbolsPass::run(prog) {
			Ok(ctx) => Ok(ctx),
			Err(e)  => Err(CompileError::from_sem_error(e)),
		}
	}

	pub fn sem_typecheck(prog: &Program, nc: &NameCtx) -> CompileResult<TypeCtx> {
		match TypecheckPass::run(prog, nc) {
			Ok(ctx) => Ok(ctx),
			Err(e)  => Err(CompileError::from_sem_error(e)),
		}
	}

	pub fn semantic(prog: &Program) -> CompileResult<(NameCtx, TypeCtx)> {
		let nc = Self::sem_symbols(prog)?;
		let tc = Self::sem_typecheck(prog, &nc)?;
		Ok((nc, tc))
	}
}