mod opcode6502;

//Just a toy implementation with Rust
//The actual implementation is the C version

fn main() {
    let mut registers : opcode6502::Reg6502 = opcode6502::Reg6502 {
        a_reg : 0,
        x_reg : 0,
        y_reg : 0,
        status_reg : 0,
        prog_counter : 0x600,
        stack_ptr : 0
    };
    let mut memory : [u8; opcode6502::mem_size] = opcode6502::init_memory();

    //Print the registers
    registers.print_regs();

    //Initialize the A register with '3'
    //Initialize Y register with '58'
    registers.LDA(3u8);
    registers.LDY(0x3au8);
    dbg!(registers);

    //Allocate the ROM from bin file
    opcode6502::alloc_rom("simple6502.bin",
                &mut memory, registers);

    //Print the memory for debugging
    opcode6502::print_mem(memory);

}
