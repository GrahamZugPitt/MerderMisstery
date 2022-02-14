use colored::Colorize;
use rustyline::{ Editor, KeyEvent, KeyCode, Modifiers, Cmd, error::ReadlineError };

use lexer_proj::*;

fn main() {
	let mut rl = Editor::<()>::new();
	rl.bind_sequence(KeyEvent(KeyCode::Tab, Modifiers::NONE),   Cmd::Insert(1, "\t".into()));
	rl.bind_sequence(KeyEvent(KeyCode::Down, Modifiers::SHIFT), Cmd::Insert(1, "\n".into()));

	println!("{}",
		"--------------------------------------------------------------------------".bright_blue());
	println!("{}",
		"Type some code. Shift+Down inserts a newline; Enter submits; Ctrl+C quits.".bright_blue());
	println!("{}",
		"--------------------------------------------------------------------------".bright_blue());

	loop {
		let line = rl.readline(">> ");
		match line {
			Ok(line) => {
				rl.add_history_entry(line.as_str());
				show_tokens(&line);
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

fn show_tokens(line: &str) {
	match Compiler::lex(&Source::from_str(line, "<stdin>")) {
		Ok(tokens) => {
			println!("{} ", "Tokens:".green());

			for t in tokens {
				println!("   {:?}", t);
			}
		}
		Err(e) => println!("{} {}", "error:".red(), e),
	}

	println!();
}