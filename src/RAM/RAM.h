#pragma once
#include <cstdint>
#include <cstring>
#include "../Component/Component.h"

namespace Lambda{
    class RAM: public Component{
    public:
        RAM();
        bool isResponsive(uint16_t addr) override;
        void write8(uint16_t addr, uint8_t value) override;
        uint8_t read8(uint16_t addr) override;
        ~RAM();
    private:
        uint8_t* m_memory;
    };
}