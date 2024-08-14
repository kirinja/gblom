#include "cpu.h"

int op_ld_u8(uint16_t &pc, unsigned char &reg, uint8_t u8, int pc_steps = 2, int cycles = 2) {
    reg = u8;
    pc += pc_steps;
    return cycles;
};

int op_ld_u16(uint16_t &pc, unsigned char &regHi, unsigned char &regLo, uint8_t u8Hi, uint8_t u8Lo, int pc_steps = 3, int cycles = 3) {
    regHi = u8Hi;
    regLo = u8Lo;
    pc += pc_steps;
    return cycles;
};

int op_ld_to_adr(uint16_t &pc, uint16_t adr, unsigned char val, MMU &mmu, int pc_steps = 1, int cycles = 1) {
    mmu.writeToMemory(adr, val);
    pc += pc_steps;
    return cycles;
};

int op_inc(uint16_t &pc, unsigned char &reg, Flags &flags, int pc_steps = 1, int cycles = 1) {
    flags.N = 0;
    flags.H = (reg & 0x0F) == 0x0F;
    reg += 1;
    flags.Z = reg == 0;

    pc += pc_steps;
    return cycles;
};

int op_dec() { return 0; };
int op_rlca() { return 0; };
int op_add() { return 0; };
int op_rrca() { return 0; };
int op_stop() { return 0; };
int op_rla() { return 0; };

int op_jr(uint16_t &pc, int8_t i8, unsigned char flag, int8_t enabled) {
    if (flag == enabled) {
        pc += 2 + i8;
        return 3;
    }
    else pc += 2;
    return 2;
};

int op_rra() { return 0; };
int op_daa() { return 0; };
int op_cpl() { return 0; };
int op_scf() { return 0; };
int op_ccf() { return 0; };
int op_halt() { return 0; };
int op_adc() { return 0; };
int op_sub() { return 0; };
int op_sbc() { return 0; };
int op_and() { return 0; };
int op_xor() { return 0; };
int op_or() { return 0; };
int op_cp() { return 0; };
int op_ret() { return 0; };
int op_pop() { return 0; };
int op_jp() { return 0; };
int op_call() { return 0; };
int op_push() { return 0; };
int op_rst() { return 0; };
int op_reti() { return 0; };
int op_di() { return 0; };
int op_ei() { return 0; };


CPU::CPU()
{
    PC = 0;
    SP = 0;
    mmu.loadROM("/Users/nbl/Projects/SDL/dmg_boot.bin");
}

CPU::~CPU()
{
}

void CPU::run()
{
    while (1) {
        stepCPU(PC, SP, registers, flags);
    }
}

int CPU::stepCPU(uint16_t &pc, uint16_t &sp, Registers &registers, Flags &flags)
{
    const uint16_t opcode = mmu.readFromMemory(pc);
    switch (opcode) {
        // NOP
        case 0x00:
            pc++;
            return 1;
            break;

        // JR NZ,i8
        case 0x20:
            return op_jr(pc, mmu.readFromMemory(pc + 1), flags.Z, 0);
            break;

        // LD C,u8
        case 0x0e:
            return op_ld_u8(pc, registers.C, mmu.readFromMemory(pc + 1), 2, 2);
            break;

        // LD A,u8
        case 0x3e:
            return op_ld_u8(pc, registers.A, mmu.readFromMemory(pc + 1), 2, 2);
            break;

        // LD (FF00+C),A
        case 0xe2:
            return op_ld_to_adr(pc, 0xFF00 + registers.C, registers.A, mmu, 1, 2);
            break;

        // INC C
        case 0x0c:
            return op_inc(pc, registers.C, flags, 1, 1);
            break;

        // LD (HL),A
        case 0x77:
            return op_ld_to_adr(pc, (registers.H << 8) | registers.L, registers.A, mmu, 1, 2);
            break;

        // LD (FF00+u8),A
        case 0xe0:
            return op_ld_to_adr(pc, 0xFF00 + mmu.readFromMemory(pc + 1), registers.A, mmu, 2, 3);
            break;

        // LD DE,u16
        case 0x11:
            return op_ld_u16(pc, registers.D, registers.E, mmu.readFromMemory(pc + 2), mmu.readFromMemory(pc + 1), 3, 3);
            break;

        // LD SP,u16
        case 0x31: 
            sp = mmu.readFromMemory(pc + 1) | (mmu.readFromMemory(pc + 2) << 8);
            pc += 3;
            return 3;
            break;
        

        // LD (HL-),A
        case 0x32: {
            uint16_t val = (registers.H << 8) | registers.L;
            // printf("0x%04x\n", val);
            // printf("0x%02x\n", registers.A);
            mmu.writeToMemory(val, registers.A);

            val--;
            registers.H = val >> 8 & 0xFF;
            registers.L = val & 0xFF;

            pc++;
            return 2;
            break;
        }

        // XOR A,A
        case 0xaf:
            if (flags.Z) registers.A = 0;
            else registers.A = registers.A ^ registers.A;
            pc++;
            return 1;
            break;

        // LD HL,u16
        case 0x21:
            registers.H = mmu.readFromMemory(pc + 2);
            registers.L = mmu.readFromMemory(pc + 1);
            pc += 3;
            return 3;
            break;

        // LD L,B
        case 0x68:
            registers.B = registers.L;
            pc++;
            return 1;
            break;

        case 0xcb:
            return readCBPrefixed(pc, sp, registers, flags);
            break;
        
        default:
            printf("Unsupported opcode 0x%02x at 0x%04x\n\n\n", opcode, pc);
            std::exit(EXIT_FAILURE);
            pc++;
            return 0;
            break;
    }
    return 0;
}

int CPU::readCBPrefixed(u_int16_t &pc, u_int16_t &sp, Registers &registers, Flags &flags)
{
    const uint16_t cb_opcode = mmu.readFromMemory(pc + 1);
    switch (cb_opcode) {

        // BIT 7,H
        case 0x7c:
            flags.Z = ~(registers.H >> 7) & 0x1;
            flags.N = 0;
            flags.H = 1;
            // registers.H = registers.H >> 1;
            pc += 2;
            return 2;
            break;

        default:
            printf("Unsupported opcode 0x%02x at 0x%04x\n\n\n", cb_opcode, pc + 1);
            std::exit(EXIT_FAILURE);
            pc++;
            return 0;
            break;
    }
    return 0;
}

