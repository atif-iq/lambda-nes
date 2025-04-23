#include "BUS.h"

namespace Lambda{

    class GUI;

    BUS::BUS(){
        m_components.reserve(5);
        LAMBDA_LOG(LogLevel::INFO, "Bus initialised successfully");
    }

    void BUS::write8(uint16_t addr, uint8_t value){
        bool written = false;
        for(auto& component: m_components){
            if(component->isResponsive(addr)){
                component->write8(addr, value);
                written = true;
                break;
            }
        }
        if(!written){
            LAMBDA_LOG(LogLevel::CRITICAL, "Trying to write to unknown address 0x{:#04X}", addr);
        }
    }
    
    uint8_t BUS::read8(uint16_t addr){
        for(auto& component: m_components){
            if(component->isResponsive(addr)){
                return component->read8(addr);
            }
        }
        LAMBDA_LOG(LogLevel::CRITICAL, "Trying to read from unknown address 0x{:#04X}", addr);
        return -1;
    }
    
    void BUS::attachComponent(std::unique_ptr<Component> component){
        m_components.emplace_back(std::move(component));
    }

    BUS::~BUS(){
        LAMBDA_LOG(LogLevel::INFO, "Bus terminated successfully");
    }
}