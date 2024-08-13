#include "cpu.h"

int op_ld() {
    return 1;
};
int op_inc() { return 0; };
int op_dec() { return 0; };
int op_rlca() { return 0; };
int op_add() { return 0; };
int op_rrca() { return 0; };
int op_stop() { return 0; };
int op_rla() { return 0; };
int op_jr() { return 0; };
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

        // // JR NZ,i8
        // case 0x20:
        //     // add i8 to NZ and jump to it
        //     return 1;
        //     break;

        // LD SP,u16
        case 0x31: 
            sp = mmu.readFromMemory(pc + 1) | (mmu.readFromMemory(pc + 2) << 8);
            pc += 3;
            return 1;
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
            return 1;
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
            return 1;
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
            return 1;
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

