pub const MEMORY : &'static [u8; 1 << 16] = &[0u8; 1 << 16];

pub struct Reg6502 {
    pub a_reg : u8,
    pub x_reg : u8,
    pub y_reg : u8,
    pub status_reg : u8,
    pub prog_counter : u16,
    pub stack_ptr : u8
}

pub fn print_mem() {
    for (ind, i) in MEMORY.iter().enumerate() {
        if ind % 16 == 15 {println!("{}", i);}
        else {print!("{} ", i);}
    }
}

pub fn print_regs(reg : Reg6502) {
    println!("A: {:#02x}\nX: {:#02x}\nY: {:#02x}\nSTATUS: {:#02x}\nPROG: {:#02x}\nSTACK: {:#02x}",
        reg.a_reg,
        reg.x_reg,
        reg.y_reg,
        reg.status_reg,
        reg.prog_counter,
        reg.stack_ptr
    );
}