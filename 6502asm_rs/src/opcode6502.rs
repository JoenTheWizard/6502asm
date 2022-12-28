//pub static mut MEMORY : &'static [u8; 1 << 16] = &[0u8; 1 << 16];
use std::{fs, num::ParseIntError};

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

pub enum StatusFlags {
    C = (1 << 0), //Carry bit
    Z = (1 << 1), //Zero
    I = (1 << 2), //Disable Interrupts
    D = (1 << 3), //Decimal mode
    B = (1 << 4), //Break
    U = (1 << 5), //Unused
    V = (1 << 6), //Overflow
    N = (1 << 7), //Negative
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
            if adr < 0xfffe {adr += 16u16};
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

fn pc_count(reg : Reg6502) -> usize {
    reg.prog_counter as usize
}

//Handle the ROM
//I might just use the C function as a static lib that I implemented 
pub fn handle_opcodes(mem : [u8; mem_size], reg : &mut Reg6502) {
    while reg.prog_counter < mem_size as u16 - 1 {
        match mem[pc_count(*reg)] {
            0xA9 => {
                reg.prog_counter += 1;
                reg.LDA(mem[pc_count(*reg)]);
            },
            0x00 => {
                reg.prog_counter += 1;
                break;
            }
            _ => {}
        }
        reg.prog_counter += 1;
    }
}

fn interp_hex(s: &str) -> Result<Vec<u8>, ParseIntError> {
    (0..s.len())
        .step_by(2)
        .map(|i| u8::from_str_radix(&s[i..i + 2], 16))
        .collect()
}

fn interp_opcode_(op : &str) -> u8 {
    let interp : Vec<char> = op.chars().collect();

    if interp[0] == '#' && interp[1] == '$' {
        let ff = interp_hex(&op[2..]).expect("[-] Error with interpreting opcode");
        return ff[0];
    }

    0u8 
}

//Interpret the intructions
pub fn interp_instruc(path : &str) -> Box<[u8]> {
    let s_ : String = fs::read_to_string(path).unwrap();
    let mut arr : Vec<u8> = Vec::with_capacity(0xf9ff); 

    for l_ in s_.lines() {
        let mut q_ : Vec<&str> = l_.split(" ").collect();
        match q_[0] {
            "LDA" => {
                arr.push(0xA9);

                //Extra error handling
                let sec_arg  = match q_.get_mut(1) {
                    Some(q) => { *q },
                    None => { 
                        println!("[-] 6502asm Error. There was an error parsing the opcode"); 
                        break; 
                        ""
                    }
                };

                arr.push(interp_opcode_(sec_arg));
            },
            _ => {}
        }
    }

    //let dynamic_alloc : Box<[u8]> = arr.into_boxed_slice();

    arr.into_boxed_slice()
}
