//pub static mut MEMORY : &'static [u8; 1 << 16] = &[0u8; 1 << 16];
use std::{fs};

pub const mem_size : usize = 0xffff;

#[derive(Debug, Copy, Clone)]
pub struct Reg6502 {
    pub a_reg : u8,
    pub x_reg : u8,
    pub y_reg : u8,
    pub status_reg : u8,
    pub prog_counter : u16,
    pub stack_ptr : u8
}

impl Reg6502 {
    pub fn print_regs(&self) {
        println!("A: {:#02x}\nX: {:#02x}\nY: {:#02x}\nSTATUS: {:#02x}\nPROG: {:#02x}\nSTACK: {:#02x}",
            self.a_reg,
            self.x_reg,
            self.y_reg,
            self.status_reg,
            self.prog_counter,
            self.stack_ptr
        );
    }

    pub fn LDA(&mut self, val : u8) {
        self.a_reg = val;
    }

    pub fn LDX(&mut self, val : u8) {
        self.x_reg = val;
    }

    pub fn LDY(&mut self, val : u8) {
        self.y_reg = val;
    }
}

pub fn init_memory() -> [u8; mem_size] {
    return [0u8; mem_size]
}

pub fn print_mem(MEMORY : [u8; mem_size]) {
    let mut adr : u16 = 0u16;
    print!("{:04x}: ",adr);
    for (ind, i) in MEMORY.iter().enumerate() {
        if ind % 16 == 15 {
            if adr < 0xfff0 {adr += 16u16};
            print!("{:02x}\n{:04x}: ", i, adr);
        }
        else {
            print!("{:02x} ", i);
        }
    }
}

fn read_file(path : &str) -> Vec<u8> {
    return fs::read(path)
            .expect("[-] Error reading file");
}

//Allocate memory from file
pub fn alloc_rom(path : &str, memory : &mut [u8; mem_size], reg : Reg6502) {
    let opcodes_read : Vec<u8> = read_file(path);

    for (index, i) in opcodes_read.iter().enumerate() {
        memory[index + reg.prog_counter as usize] = *i;
    }
}