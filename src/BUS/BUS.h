#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "../Component/Component.h"
#include "../Logger/Logger.h"

namespace Lambda{
    class BUS{
    public:
        BUS();
        void write8(uint16_t addr, uint8_t value);
        uint8_t read8(uint16_t addr);
        std::vector<std::shared_ptr<Component>> getComponents(){ return m_components; }
        void attachComponent(std::unique_ptr<Component> component);
        ~BUS();
        friend class GUI;
    private:  
        std::vector<std::shared_ptr<Component>> m_components;
    };
}