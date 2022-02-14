
use std::path::{ PathBuf };
use codegen_proj::*;

fn main() {
	let args = parse_args();
	let src = read_source(&args.in_filename);
	let (prog, nc, tc) = compile(&src);
	Compiler::freeze(&args.out_filename, prog, nc, tc);
	println!("frozen program written to {}.", args.out_filename);
}

struct Args {
	in_filename:  String,
	out_filename: String,
}

fn parse_args() -> Args {
	let args = std::env::args().collect::<Vec<_>>();

	if args.len() < 2 {
		print_usage_and_exit();
	}

	let in_path = match PathBuf::from(args.last().unwrap()).canonicalize() {
		Ok(p) => p,
		Err(e) => {
			eprintln!("bad input file name: {}", e);
			std::process::exit(1)
		}
	};

	if in_path.extension().map(|s| s.to_str().unwrap()) != Some("trs") {
		eprintln!("input must be a *.trs file");
		std::process::exit(1)
	}

	Args {
		in_filename:  in_path.to_str().unwrap().into(),
		out_filename: in_path.with_extension("ron").to_str().unwrap().into(),
	}
}

fn print_usage_and_exit() {
	println!(r"usage:
    cargo run --bin freeze_program -- filename.trs

    lexes, parses, and semantics 'filename.trs' and dumps the resulting
    data structures to 'filename.ron' in the same directory.");

	std::process::exit(1)
}

fn read_source(filename: &str) -> Source {
	match Source::from_file(filename) {
		Ok(ret) => ret,
		Err(e) => {
			eprintln!("could not open '{}': {}", filename, e);
			std::process::exit(1)
		}
	}
}

fn compile(src: &Source) -> (Program, NameCtx, TypeCtx) {
	match compile_impl(src) {
		Ok(ret) => ret,
		Err(e) => {
			eprintln!("Compilation error: {}", e);
			std::process::exit(1)
		}
	}
}

fn compile_impl(src: &Source) -> CompileResult<(Program, NameCtx, TypeCtx)> {
	let prog     = Compiler::lex_and_parse(&src)?;
	let (nc, tc) = Compiler::semantic(&prog)?;
	Ok((prog, nc, tc))
}