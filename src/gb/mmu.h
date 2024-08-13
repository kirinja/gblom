#include <cstdint>

class MMU
{
public:
    MMU();
    ~MMU();
    void writeToMemory(uint16_t adr, unsigned char val);
    uint16_t readFromMemory(uint16_t adr);
    uint16_t loadROM(const char* filepath);
private:
    unsigned char memory[0xFA000];      // memory
    // unsigned char memory[0x100];      // memory
    unsigned char rom[0xFA000];			//	cartridge
    unsigned char romtype = 0x00;
    unsigned char *ram[8];	
};
