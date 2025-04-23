#include "CPU.h"

namespace Lambda{
    /*---Addressing Modes---------------------------------------------*/
    void CPU::IMP(){
        //Does nothing
    }

    void CPU::ACC(){
        //Handled manually inside the opcode.
    }

    void CPU::IMM(){
        rw_Addr = PC++;
    }

    void CPU::ZP (){
        rw_Addr = read8(PC++);
    }

    void CPU::ZPX(){
        rw_Addr = (read8(PC++) + IX) & 0x00FF;
    }

    void CPU::ZPY(){
        rw_Addr = (read8(PC++) + IY) & 0x00FF;
    }

    void CPU::REL(){
        branchOffset = static_cast<int8_t>(read8(PC++));
    }

    void CPU::ABS(){
        rw_Addr = read8(PC) | (read8(PC+1)<<8);
        PC+=2;
    }

    void CPU::ABX(){
        rw_Addr = (read8(PC) | (read8(PC+1)<<8)) + IX;
        if((rw_Addr&0xFF00) != ((rw_Addr-IX)&0xFF00)){
            ++m_cycles;
        }
        PC+=2;
    }

    void CPU::ABY(){
        rw_Addr = (read8(PC) | (read8(PC+1)<<8)) + IY;
        if((rw_Addr&0xFF00) != ((rw_Addr-IY)&0xFF00)){
            ++m_cycles;
        }
        PC+=2;
    }

    void CPU::IND(){
        uint16_t ptr = (read8(PC) | (read8(PC+1)<<8));

        /* Page Boundary Bug */
        if((ptr&0x00FF) == 0x00FF){
            rw_Addr = read8(ptr) | (read8(ptr&0xFF00)<<8);
        }
        else{
            rw_Addr = read8(ptr) | (read8(ptr+1)<<8);
        }
        PC+=2;
    }

    void CPU::IDX(){
        uint8_t ptr = (read8(PC++) + IX) & 0x00FF;
        rw_Addr = read8(ptr) | (read8(ptr+1)<<8);
    }

    void CPU::IDY(){
        uint8_t ptr = read8(PC++) & 0x00FF;
        rw_Addr = read8(ptr) | (read8(ptr+1)<<8);
        rw_Addr += IY;

        if((rw_Addr&0xFF00) != ((rw_Addr-IY)&0xFF00)){
            ++m_cycles;
        }
    }
}