#include "RAM.h"
#include <cstdint>

namespace Lambda {
    RAM::RAM(){
        m_memory = new uint8_t[0xFFFF+1];
        memset(m_memory, 0, sizeof(uint8_t) * (0xFFFF+1));
    }

    bool RAM::isResponsive(uint16_t addr){
        return (addr>=0 && addr<=0xFFFF);
    }

    void RAM::write8(uint16_t addr, uint8_t value){
        m_memory[addr] = value;
    }

    uint8_t RAM::read8(uint16_t addr){
        return m_memory[addr];
    }

    RAM::~RAM(){
        delete[] m_memory;
    }
}