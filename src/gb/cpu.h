#include <cstdint>
#include "flags.h"
#include "registers.h"
#include <string>
#include "mmu.h"

class CPU {
    public:
        CPU();
        ~CPU();
        void run();
        int stepCPU(uint16_t &pc, uint16_t &sp, Registers &registers, Flags &flags);
    private:
        int readCBPrefixed(u_int16_t &pc, u_int16_t &sp, Registers &registers, Flags &flags);
        // unsigned char memory[0xFA000];
        // unsigned char memory[512];
        uint16_t PC;
        uint16_t SP;
        Registers registers;
        Flags flags;
        MMU mmu;

        // int op_bit();

};