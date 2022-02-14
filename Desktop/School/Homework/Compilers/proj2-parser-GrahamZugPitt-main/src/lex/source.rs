
use std::fmt::{ Display, Formatter, Result as FmtResult };

/// Represents a piece of source code. Includes the filename (if any) and
/// the raw source code.
#[derive(Clone)]
pub struct Source {
	filename: String,
	code:     Vec<char>,
}

impl Source {
	/// Constructor: create a `Source` by reading from a file.
	pub fn from_file(filename: &str) -> std::io::Result<Self> {
		let code = std::fs::read_to_string(filename)?;
		Ok(Self::from_str(&code, filename))
	}

	/// Constructor: create a `Source` from a string. Its file name field will be `filename`, but
	/// no file is read.
	pub fn from_str(code: &str, filename: &str) -> Self {
		Self {
			filename: filename.into(),
			code:     code.chars().collect(),
		}
	}

	/// Gets the filename as a string.
	pub fn filename(&self) -> &str {
		&self.filename
	}

	/// Gets the raw source code as a slice of chars. You could then index this with
	/// say, [`Token::span`] to recover the original characters the token was made from.
	pub fn code(&self) -> &[char] {
		&self.code
	}

	/// Iterator over the source code characters.
	pub fn iter(&self) -> SourceIter<'_> {
		SourceIter::new(&self.filename, &self.code)
	}

	/// Given an offset into this object's `code`, create a [`Location`] that represents the
	/// line and column.
	///
	/// Panics if `offs` is not <= this object's code length.
	pub fn location_from_offset(&self, offs: usize) -> Location {
		let (line, col) = compute_line_and_column(&self.code, offs);
		Location::new(&self.filename, line, col)
	}
}

fn compute_line_and_column(code: &[char], offs: usize) -> (usize, usize) {
	assert!(offs <= code.len(), "invalid source offset");

	// just look at the stuff before this offset.
	let code_before = &code[..offs];

	// how many newline characters are there before the offset?
	let num_newlines = code_before.iter().filter(|c| **c == '\n').count();

	// line numbers start from 1.
	let line = num_newlines + 1;

	// now find the newline character before this position (if any)...
	let start_of_line = match code_before.iter().rposition(|c| *c == '\n') {
		// +1 is safe because newline_pos is guaranteed to be at most len - 1.
		// so newline_pos + 1 can be at most len, and slicing [len..] just gives
		// an empty string.
		Some(newline_pos) => newline_pos + 1,
		None              => 0,
	};

	// ...and let's look at just the code on this line.
	let code_before = &code_before[start_of_line ..];

	// now we count characters, but tabs count for 4.
	let col = code_before.iter().fold(1, |count, ch| {
		match ch {
			'\t' => count + 4,
			_    => count + 1,
		}
	});

	(line, col)
}

/// Iterator over a [`Source`] object's characters. Keeps track
/// of line and column and can produce a [`Location`].
pub struct SourceIter<'src> {
	code:     &'src [char],
	filename: &'src str,
	line:     usize,
	col:      usize,
	pos:      usize,
}

impl<'src> SourceIter<'src> {
	fn new(filename: &'src str, code: &'src [char]) -> Self {
		Self {
			code,
			filename,
			line: 1,
			col: 1,
			pos: 0,
		}
	}

	/// Gets a [`Location`] representing the current position in the source.
	pub fn loc(&self) -> Location {
		Location::new(self.filename, self.line, self.col)
	}

	/// Gets the current position in the source.
	pub fn pos(&self) -> usize {
		self.pos
	}

	/// True if there are more characters left to read.
	pub fn has_more(&self) -> bool {
		self.pos < self.code.len()
	}

	/// Gets the character at the current position, or `'\0'` if the current position
	/// is past the end of the source.
	pub fn cur(&self) -> char {
		*self.code.get(self.pos).unwrap_or(&'\0')
	}

	/// Gets the character at the *next* position, or `'\0'` if the next position is
	/// past the end of the source.
	pub fn peek(&self) -> char {
		*self.code.get(self.pos + 1).unwrap_or(&'\0')
	}

	/// Moves to the next position and updates the line and column info.
	///
	/// Panics if the position is already past the end of the source.
	pub fn next(&mut self) {
		assert!(self.pos < self.code.len());

		if self.code[self.pos] == '\n' {
			self.line += 1;
			self.col = 1;
		} else if self.code[self.pos] == '\t' {
			self.col += 4;
		} else {
			self.col += 1;
		}

		self.pos += 1;
	}

	/// Moves the position forward by 2 characters.
	pub fn next_2(&mut self) {
		self.next();
		self.next();
	}
}

/// Represents a location within a piece of source code. Includes the filename,
/// line, and column.
#[derive(Debug, PartialEq, Eq, Clone)]
pub struct Location {
	filename: String,
	line:     usize,
	col:      usize,
}

impl Location {
	// pub(crate) means that only code within the compiler can instantiate this.
	// code outside the compiler can only get a Location from methods that return one.
	pub(crate) fn new(filename: &str, line: usize, col: usize) -> Self {
		Self { filename: filename.into(), line, col }
	}

	/// Getter for the filename.
	pub fn filename(&self) -> &str {
		&self.filename
	}

	/// Getter for the line.
	pub fn line(&self) -> usize {
		self.line
	}

	/// Getter for the column.
	pub fn col(&self) -> usize {
		self.col
	}
}

impl Display for Location {
	fn fmt(&self, f: &mut Formatter) -> FmtResult {
		write!(f, "{} ({}:{})", self.filename, self.line, self.col)
	}
}

#[cfg(test)]
mod tests {
	use super::*;

	const SRC: &'static str =
r"hello there
	this is a test of things
		there are multiple tabs now
    this line is indented with spaces";

	const TEST_POSITIONS: &[(usize, (usize, usize))] = &[
		(0,   (1, 1)),
		(1,   (1, 2)),
		(11,  (1, 12)),
		(12,  (2, 1)),
		(13,  (2, 5)),
		(17,  (2, 9)),
		(37,  (2, 29)),
		(38,  (3, 1)),
		(40,  (3, 9)),
		(67,  (3, 36)),
		(68,  (4, 1)),
		(72,  (4, 5)),
		(105, (4, 38)),
	];

	#[test]
	fn line_and_column() {
		let src = SRC.chars().collect::<Vec<_>>();

		for (offs, (line, col)) in TEST_POSITIONS {
			assert_eq!(compute_line_and_column(&src, *offs), (*line, *col));
		}
	}

	#[test]
	fn locations() {
		let src = Source::from_str(SRC, "test.trs");

		for (offs, (line, col)) in TEST_POSITIONS {
			let loc = src.location_from_offset(*offs);
			let expected = format!("test.trs ({}:{})", *line, *col);
			assert_eq!(loc.to_string(), expected);
		}
	}
}