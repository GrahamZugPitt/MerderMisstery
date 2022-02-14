
use crate::parse::ast::*;
use crate::lex::token::*;

#[derive(Debug, PartialEq, Eq, PartialOrd, Ord)]
pub(crate) enum Precedence {
	None,   // for things which aren't operators
	Or,     // logical or
	And,    // logical and
	Cmp,    // == != < <= > >=
	Shift,  // << >>
	Add,
	Mul,
}

impl Precedence {
	pub(crate) const MIN : Precedence = Precedence::Or;
}

impl TokenKind {
	pub(crate) fn precedence(&self) -> Precedence {
		use TokenKind::*;

		match self {
			Or                      => Precedence::Or,
			And                     => Precedence::And,
			Eq | NotEq | Less | LessEq |
			Greater | GreaterEq     => Precedence::Cmp,
			Shl | Shr               => Precedence::Shift,
			Plus  | Minus           => Precedence::Add,
			Times | Divide | Modulo => Precedence::Mul,

			// all other tokens have "no" precedence. this is how the expression
			// parser knows when to stop parsing.
			_                       => Precedence::None,
		}
	}

	pub(crate) fn to_binop(&self) -> BinOp {
		use TokenKind::*;
		match self {
			Plus      => BinOp::Add,
			Minus     => BinOp::Sub,
			Times     => BinOp::Mul,
			Divide    => BinOp::Div,
			Modulo    => BinOp::Mod,
			Eq        => BinOp::Eq,
			NotEq     => BinOp::NotEq,
			Less      => BinOp::Less,
			LessEq    => BinOp::LessEq,
			Greater   => BinOp::Greater,
			GreaterEq => BinOp::GreaterEq,
			Shl       => BinOp::Shl,
			Shr       => BinOp::Shr,
			And       => BinOp::And,
			Or        => BinOp::Or,
			_         => panic!("to_binop() called on a {:?} token", self),
		}
	}
}