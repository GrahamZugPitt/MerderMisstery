// #![allow(dead_code)]

#![deny(unused_must_use)] // makes forgetting ? a hard error

use serde::{ Serialize, Deserialize };

pub mod error;
pub mod lex;
pub mod parse;
pub mod sem;
// pub mod ir;
// pub mod opt;
pub mod cg;

pub use error::*;
pub use lex::*;
pub use parse::*;
pub use sem::*;
// pub use ir::*;
// pub use opt::*;
pub use cg::*;

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

	pub fn freeze(filename: &str, prog: Program, nc: NameCtx, tc: TypeCtx) {
		FrozenProgram { prog, nc, tc }.write_to_file(filename);
	}

	pub fn unfreeze(filename: &str) -> (Program, NameCtx, TypeCtx) {
		let prog = FrozenProgram::read_from_file(filename);
		(prog.prog, prog.nc, prog.tc)
	}

	pub fn direct_codegen(prog: &Program, nc: &NameCtx, tc: &TypeCtx) -> (TextSeg, DataSeg) {
		DirectCg::run(prog, nc, tc)
	}
}

#[derive(Serialize, Deserialize)]
pub struct FrozenProgram {
	prog: Program,
	nc:   NameCtx,
	tc:   TypeCtx,
}

impl FrozenProgram {
	pub fn write_to_file(self, filename: &str) {
		let serialized = ron::to_string(&self).unwrap();
		match std::fs::write(filename, serialized) {
			Ok(()) => {}
			Err(e) => {
				eprintln!("could not serialize to file '{}': {}", filename, e);
				std::process::exit(1);
			}
		}
	}

	pub fn read_from_file(filename: &str) -> Self {
		let serialized = match std::fs::read_to_string(filename) {
			Ok(s) => s,
			Err(e) => {
				eprintln!("could not read from file '{}': {}", filename, e);
				std::process::exit(1)
			}
		};

		match ron::from_str::<Self>(&serialized) {
			Ok(fp) => fp,
			Err(e) => {
				eprintln!("error deserializing file '{}': {}", filename, e);
				std::process::exit(1)
			}
		}
	}
}