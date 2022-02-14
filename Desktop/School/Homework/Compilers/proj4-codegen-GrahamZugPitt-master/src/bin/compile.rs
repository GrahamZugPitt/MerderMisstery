
use std::fmt::{ Write };
use std::path::{ PathBuf };
use codegen_proj::*;

// the MARS jar file to run when using -r.
// due to the codegen output formatting, you must use one of my custom
// versions of MARS as they allow you to elide $ on registers and use
// function-local labels, which the stock version does not.
const MARS_JAR: &str = "Mars_2211_0822.jar";

fn main() {
	let args = parse_args();

	let (text, data) = if args.is_frozen {
		unfreeze_and_codegen(&args.in_filename)
	} else {
		compile(&read_source(&args.in_filename))
	};

	if args.print {
		println!("{}", data);
		println!("{}", text);
	} else {
		write_asm(&args.out_filename, text, data);

		if args.run {
			run_mars(&args.out_filename);
		}
	}
}

struct Args {
	in_filename:  String,
	is_frozen:    bool,
	out_filename: String,
	print:        bool,
	run:          bool,
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

	let is_frozen = match in_path.extension().map(|s| s.to_str().unwrap()) {
		Some("trs") => false,
		Some("ron") => true,
		_ => {
			eprintln!("input file must be *.trs or *.ron");
			std::process::exit(1)
		}
	};

	let options = &args[1 .. args.len() - 1];
	let run = options.contains(&"-r".into());
	let print = !run && options.contains(&"-p".into());

	Args {
		in_filename:  in_path.to_str().unwrap().into(),
		is_frozen,
		out_filename: in_path.with_extension("asm").to_str().unwrap().into(),
		print,
		run,
	}
}

fn print_usage_and_exit() {
	println!(r"usage:
    cargo run --bin mips_cg_test -- [options] filename.trs

    compiles 'filename.trs' to 'filename.asm' in the same directory.

    options:
        -p   print output asm to console instead of to file
        -r   run resulting program in MARS (overrides -p)");

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

fn write_asm(filename: &str, text: TextSeg, data: DataSeg) {
	let mut out_str = String::new();
	writeln!(&mut out_str, "{}", data).unwrap();
	writeln!(&mut out_str, "{}", text).unwrap();

	match std::fs::write(filename, out_str) {
		Ok(()) => {}
		Err(e) => {
			eprintln!("could not write '{}': {}", filename, e);
			std::process::exit(1)
		}
	}
}

fn compile(src: &Source) -> (TextSeg, DataSeg) {
	match compile_impl(src) {
		Ok(ret) => ret,
		Err(e) => {
			eprintln!("Compilation error: {}", e);
			std::process::exit(1)
		}
	}
}

fn compile_impl(src: &Source) -> CompileResult<(TextSeg, DataSeg)> {
	let prog     = Compiler::lex_and_parse(&src)?;
	let (nc, tc) = Compiler::semantic(&prog)?;
	Ok(Compiler::direct_codegen(&prog, &nc, &tc))
}

fn unfreeze_and_codegen(filename: &str) -> (TextSeg, DataSeg) {
	let (prog, nc, tc) = Compiler::unfreeze(filename);
	Compiler::direct_codegen(&prog, &nc, &tc)
}

fn run_mars(asm_filename: &str) {
	println!("------------ running MARS, all following output is from program ------------");

	let res = std::process::Command::new("java")
		// me = display MARS messages to standard err instead of standard out
		// nc = copyright notice will not be displayed
		// sm = start execution at global 'main'
		// we = assembler warnings will be considered errors
		.args(&["-jar", MARS_JAR, "me", "nc", "sm", "we", asm_filename])
		.spawn();

	let mut child = match res {
		Ok(c) => c,
		Err(e) => {
			eprintln!("Could not run MARS! {}", e);
			std::process::exit(1)
		}
	};

	match child.wait() {
		Ok(_status) => {}
		Err(e) => {
			eprintln!("Could not wait for MARS process! {}", e);
			std::process::exit(1)
		}
	}
}