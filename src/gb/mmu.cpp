#include "mmu.h"
#include <cstdio>

MMU::MMU()
{
}

MMU::~MMU()
{
}

void MMU::writeToMemory(uint16_t adr, unsigned char val)
{
    //	MBC0
	if (romtype == 0x00) {
		
		//	make ROM readonly
		if (adr >= 0x8000)
			memory[adr] = val;
	}
}

uint16_t MMU::readFromMemory(uint16_t adr)
{
    return memory[adr];
}

uint16_t MMU::loadROM(const char *filepath)
{
    // unsigned char memory[0x100];
    FILE* file = fopen(filepath, "rb");
    int pos = 0;
    while (fread(&memory[pos], 1, 1, file)) {
        pos++;
    }
    fclose(file);

    return 0;
}
