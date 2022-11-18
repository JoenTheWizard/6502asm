mod opcode6502;

//Just a toy implementation with Rust
//The actual implementation is the C version

fn main() {
    let mut registers = opcode6502::Reg6502 {
        a_reg : 0,
        x_reg : 0,
        y_reg : 0,
        status_reg : 0,
        prog_counter : 0x600,
        stack_ptr : 0
    };

    opcode6502::print_regs(registers);

    //opcode6502::print_mem();
}
