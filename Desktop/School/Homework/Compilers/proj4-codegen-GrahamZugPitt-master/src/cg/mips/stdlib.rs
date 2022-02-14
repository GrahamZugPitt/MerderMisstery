
use super::arch::*;

pub fn add_stdlib(text: &mut TextSeg, data: &mut DataSeg) {
	// due to the ABI, on entry to each function, sp is pointing to the first argument.
	// the epilogue must pop all arguments from the stack.

	use Inst::*;
	use Reg::*;

	// fn print_i(v: int)
	let mut f = Function::new_global("print_i");
		f.inst(Lw(A0, 0, Sp));   // lw a0, 0(sp)
		f.inst(Li(V0, 1));       // li v0, 1
		f.inst(Syscall);         // syscall
		f.inst(Addi(Sp, Sp, 4)); // addi sp, sp, 4
		f.inst(Jr(Ra));          // jr ra
	text.add(f);

	// fn print_s(v: string)
	let mut f = Function::new_global("print_s");
		f.inst(Lw(A0, 0, Sp));   // lw a0, 0(sp)
		f.inst(Li(V0, 4));       // li v0, 4
		f.inst(Syscall);         // syscall
		f.inst(Addi(Sp, Sp, 4)); // addi sp, sp, 4
		f.inst(Jr(Ra));          // jr ra
	text.add(f);

	// fn print_c(c: int)
	let mut f = Function::new_global("print_c");
		f.inst(Lw(A0, 0, Sp));   // lw a0, 0(sp)
		f.inst(Li(V0, 11));      // li v0, 4
		f.inst(Syscall);         // syscall
		f.inst(Addi(Sp, Sp, 4)); // addi sp, sp, 4
		f.inst(Jr(Ra));          // jr ra
	text.add(f);

	// fn print_b(c: bool)
	let true_str  = data.add_str_lit("true");
	let false_str = data.add_str_lit("false");

	let mut f = Function::new_global("print_b");
		f.inst(La(A0, false_str.clone()));      //  la a0, false_str
		f.inst(Lb(T0, 0, Sp));                  //  lb t0, 0(sp)
		f.inst(Beq(T0, Zero, "_print".into())); //  beq t0, 0, _print
		f.inst(La(A0, true_str.clone()));       //  la a0, true_str
		f.label("_print".into());               // _print:
		f.inst(Li(V0, 4));                      //  li v0, 4
		f.inst(Syscall);                        //  syscall
		f.inst(Addi(Sp, Sp, 4));                //  addi sp, sp, 4
		f.inst(Jr(Ra));                         //  jr ra
	text.add(f);

	// fn println_i(v: int)
	let mut f = Function::new_global("println_i");
		f.inst(Lw(A0, 0, Sp));   // lw a0, 0(sp)
		f.inst(Li(V0, 1));       // li v0, 1
		f.inst(Syscall);         // syscall
		f.inst(Li(A0, 10));      // li a0, 10
		f.inst(Li(V0, 11));      // li v0, 11
		f.inst(Syscall);         // syscall
		f.inst(Addi(Sp, Sp, 4)); // addi sp, sp, 4
		f.inst(Jr(Ra));          // jr ra
	text.add(f);

	// fn println_s(v: string)
	let mut f = Function::new_global("println_s");
		f.inst(Lw(A0, 0, Sp));   // lw a0, 0(sp)
		f.inst(Li(V0, 4));       // li v0, 4
		f.inst(Syscall);         // syscall
		f.inst(Li(A0, 10));      // li a0, 10
		f.inst(Li(V0, 11));      // li v0, 11
		f.inst(Syscall);         // syscall
		f.inst(Addi(Sp, Sp, 4)); // addi sp, sp, 4
		f.inst(Jr(Ra));          // jr ra
	text.add(f);

	// fn println_c(c: int)
	let mut f = Function::new_global("println_c");
		f.inst(Lw(A0, 0, Sp));   // lw a0, 0(sp)
		f.inst(Li(V0, 11));      // li v0, 4
		f.inst(Syscall);         // syscall
		f.inst(Li(A0, 10));      // li a0, 10
		f.inst(Li(V0, 11));      // li v0, 11
		f.inst(Syscall);         // syscall
		f.inst(Addi(Sp, Sp, 4)); // addi sp, sp, 4
		f.inst(Jr(Ra));          // jr ra
	text.add(f);

	// fn println_b(c: bool)
	let mut f = Function::new_global("println_b");
		f.inst(La(A0, false_str));              //  la a0, false_str
		f.inst(Lb(T0, 0, Sp));                  //  lb t0, 0(sp)
		f.inst(Beq(T0, Zero, "_print".into())); //  beq t0, 0, _print
		f.inst(La(A0, true_str));               //  la a0, true_str
		f.label("_print".into());               // _print:
		f.inst(Li(V0, 4));                      //  li v0, 4
		f.inst(Syscall);                        //  syscall
		f.inst(Li(A0, 10));                     //  li a0, 10
		f.inst(Li(V0, 11));                     //  li v0, 11
		f.inst(Syscall);                        //  syscall
		f.inst(Addi(Sp, Sp, 4));                //  addi sp, sp, 4
		f.inst(Jr(Ra));                         //  jr ra
	text.add(f);

	// --------------------------------------------------------------------------------------------
	// Runtime functions

	// fn rt$concat(s1: string, s2: string): string
	let mut f = Function::new_global("rt$concat");
		f.comment("s0 = total length of resulting string");
		f.inst(Li(S0, 0));

		f.comment("length of first string");
		f.inst(Lw(T0, 0, Sp));
		f.label("_strlen1");
			f.inst(Lb(T1, 0, T0));
			f.inst(Beq(T1, Zero, "_end_strlen1".into()));
			f.inst(Addi(S0, S0, 1));
			f.inst(Addi(T0, T0, 1));
			f.inst(J("_strlen1".into()));
		f.label("_end_strlen1");

		f.comment("length of second string");
		f.inst(Lw(T0, 4, Sp));
		f.label("_strlen2");
			f.inst(Lb(T1, 0, T0));
			f.inst(Beq(T1, Zero, "_end_strlen2".into()));
			f.inst(Addi(S0, S0, 1));
			f.inst(Addi(T0, T0, 1));
			f.inst(J("_strlen2".into()));
		f.label("_end_strlen2");

		f.comment("add 1 more for nul terminator");
		f.inst(Addi(S0, S0, 1));

		f.comment("allocate space");
		f.inst(Move(A0, S0));
		f.comment("sbrk");
		f.inst(Li(V0, 9));
		f.inst(Syscall);

		f.comment("now v0 contains destination. leave it alone!!");

		f.comment("s0 is the destination pointer");
		f.inst(Move(S0, V0));

		f.comment("copy first string");
		f.inst(Lw(T0, 0, Sp));

		f.label("_copy1");
			f.inst(Lb(T1, 0, T0));

			f.inst(Beq(T1, Zero, "_end_copy1".into()));
			f.inst(Sb(T1, 0, S0));
			f.inst(Addi(S0, S0, 1));
			f.inst(Addi(T0, T0, 1));
			f.inst(J("_copy1".into()));
		f.label("_end_copy1");

		f.comment("copy second string");
		f.inst(Lw(T0, 4, Sp));
		f.label("_copy2");
			f.inst(Lb(T1, 0, T0));
			f.inst(Beq(T1, Zero, "_end_copy2".into()));
			f.inst(Sb(T1, 0, S0));
			f.inst(Addi(S0, S0, 1));
			f.inst(Addi(T0, T0, 1));
			f.inst(J("_copy2".into()));
		f.label("_end_copy2");

		f.comment("put nul terminator");
		f.inst(Sb(Zero, 0, S0));

		f.comment("and we're done");
		f.inst(Addi(Sp, Sp, 8));
		f.inst(Jr(Ra));
	text.add(f);

	// fn rt$new(size: int): Struct
	let mut f = Function::new_global("rt$new");
		f.inst(Lw(A0, 0, Sp));
		f.comment("sbrk");
		f.inst(Li(V0, 9));
		f.inst(Syscall);

		f.comment("v0 now contains result. don't change it!");

		f.comment("zero out the memory");
		f.inst(Move(T0, V0));
		f.inst(Lw(T1, 0, Sp));
		f.label("_zero");
			f.inst(Sb(Zero, 0, T0));
			f.inst(Addi(T0, T0, 1));
			f.inst(Addi(T1, T1, -1));
			f.inst(Bne(T1, Zero, "_zero".into()));

		f.inst(Addi(Sp, Sp, 4));
		f.inst(Jr(Ra));
	text.add(f);
}

#[cfg(none)]
#[test]
fn tryit() {
	let mut text = TextSeg::new();
	let mut data = DataSeg::new();
	add_stdlib(&mut text, &mut data);
	println!("{}", data);
	println!("{}", text);
}