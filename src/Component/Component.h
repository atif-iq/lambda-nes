#pragma once
#include <cstdint>
#include <utility>

namespace Lambda{
    //Interface for all the components that shall be attached to the BUS
    class Component{
    public:
        virtual bool isResponsive(uint16_t addr) = 0;
        virtual void write8(uint16_t addr, uint8_t value) = 0;
        virtual uint8_t read8(uint16_t addr) = 0;
        ~Component()= default;
    private:
    };
}