use colored::Colorize;
use rustyline::{ Editor, KeyEvent, KeyCode, Modifiers, Cmd, error::ReadlineError };

use parser_proj::*;

fn main() {
	let args = std::env::args().collect::<Vec<_>>();
	let stmt_mode = !args[1..].contains(&String::from("--decl"));

	let mut rl = Editor::<()>::new();
	rl.bind_sequence(KeyEvent(KeyCode::Tab, Modifiers::NONE),   Cmd::Insert(1, "\t".into()));
	rl.bind_sequence(KeyEvent(KeyCode::Down, Modifiers::SHIFT), Cmd::Insert(1, "\n".into()));

	println!("{}",
		"--------------------------------------------------------------------------".bright_blue());
	println!("{}",
		"Type some code. Shift+Down inserts a newline; Enter submits; Ctrl+C quits.".bright_blue());

	if stmt_mode {
		println!("{}",
			"STATEMENT MODE: enter statements. for decl mode, run like this:".bright_green());
		println!("  cargo run --bin parse_interactive -- --decl");
	} else {
		println!("{}", "DECL MODE: enter declarations. ".bright_green());
	}

	println!("{}", "Note: you must have your lexer working for this to work.".red());
	println!("{}",
		"--------------------------------------------------------------------------".bright_blue());

	loop {
		let line = rl.readline(">> ");
		match line {
			Ok(line) => {
				rl.add_history_entry(line.as_str());
				show_ast(&line, stmt_mode);
			},
			Err(ReadlineError::Interrupted) => break,
			Err(ReadlineError::Eof) => break,
			Err(err) => {
				println!("Error: {:?}", err);
				break
			}
		}
	}

	println!("byeeeeee!");
}

fn show_ast(line: &str, stmt_mode: bool) {
	if stmt_mode {
		match Compiler::lex_and_parse_stmt(&Source::from_str(line, "<stdin>")) {
			Ok(ast) => {
				println!("{}", "AST:".green());
				println!("{}", ast);
			}
			Err(e) => println!("{} {}", "error:".red(), e),
		}
	} else {
		match Compiler::lex_and_parse(&Source::from_str(line, "<stdin>")) {
			Ok(ast) => {
				println!("{}", "AST:".green());
				println!("{}", ast);
			}
			Err(e) => println!("{} {}", "error:".red(), e),
		}
	}
}