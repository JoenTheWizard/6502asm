mod opcode6502;

//Just a toy implementation with Rust
//The actual implementation is the C version

fn main() {
    //Initialize the registers struct
    //Will contain the information for each of the registers
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
    // registers.LDA(3u8);
    // registers.LDY(0x3au8);
    // dbg!(registers);

    //Allocate the ROM from bin file
    opcode6502::alloc_rom("simple6502.bin",
                &mut memory, registers);

    //Handle the opcodes
    opcode6502::handle_opcodes(memory, &mut registers);

    //Print the memory for debugging
    opcode6502::print_mem(memory);

    //Returns 'Box<[u8]>' smart pointer type (just an alternative to heap allocation)
    //This will return an array of uint8 which will represent the opcodes read from the
    //'asm' file
    let t : &[u8] = &opcode6502::interp_instruc("simple6502.asm");

    dbg!(registers);

    for j in t {
        println!("{}", j);
    }
}
