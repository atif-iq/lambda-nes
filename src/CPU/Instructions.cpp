#include "CPU.h"

namespace Lambda{
    /*---Legal Instructions------------------------------------------*/
    void CPU::ADC(){
        uint16_t result = A + read8(rw_Addr) + getFlag(PSFlag::CARRY);

        setFlag(PSFlag::ZERO, (result&0x00FF)==0);
        setFlag(PSFlag::CARRY, (result)>255);
        setFlag(PSFlag::NEGATIVE, result&0x0080);
        setFlag(PSFlag::OVERFLOW, (~(A^result) & (A^read8(rw_Addr)) & 0x0080));

        A = result & 0x00FF;
    } 	

    void CPU::AND(){
        A = A&read8(rw_Addr);
        setFlag(PSFlag::ZERO, A==0);
        setFlag(PSFlag::NEGATIVE, A&0x80);
    }

    void CPU::ASL(){
        uint8_t value = read8(rw_Addr);
        //Accumulator mode
        if(m_currentOpcode == 0x000A){
            value = A;
            A<<=1;
        }
        else{
            write8(rw_Addr, value<<1);
        }
        /* Get's the 8th bit (bit 7) and sets the flag */
        setFlag(PSFlag::CARRY, value&0x80);
        value<<=1;
        setFlag(PSFlag::ZERO, value==0);
        setFlag(PSFlag::NEGATIVE, value&0x80);
    }

    void CPU::BCC(){
        if(!getFlag(PSFlag::CARRY)){
            ++m_cycles;
            if((PC&0xFF00) != ((PC+branchOffset)&0xFF00)){
                ++m_cycles;
            }
            PC+=branchOffset;
        }
    } 	

    void CPU::BCS(){
        if(getFlag(PSFlag::CARRY)){
            ++m_cycles;
            if((PC&0xFF00) != ((PC+branchOffset)&0xFF00)){
                ++m_cycles;
            }
            PC+=branchOffset;
        }
    } 	

    void CPU::BEQ(){
        if(getFlag(PSFlag::ZERO)){
            ++m_cycles;
            if((PC&0xFF00) != ((PC+branchOffset)&0xFF00)){
                ++m_cycles;
            }
            PC+=branchOffset;
        }
    }

    void CPU::BIT(){
        uint8_t value = read8(rw_Addr);
        uint8_t result = A&value;
        setFlag(PSFlag::ZERO, result==0);
        setFlag(PSFlag::OVERFLOW, value&0x40);
        setFlag(PSFlag::NEGATIVE, value&0x80);
    } 	

    void CPU::BMI(){
        if(getFlag(PSFlag::NEGATIVE)){
            ++m_cycles;
            if((PC&0xFF00) != ((PC+branchOffset)&0xFF00)){
                ++m_cycles;
            }
            PC+=branchOffset;
        }
    } 	 

    void CPU::BNE(){
        if(!getFlag(PSFlag::ZERO)){
            ++m_cycles;
            if((PC&0xFF00) != ((PC+branchOffset)&0xFF00)){
                ++m_cycles;
            }
            PC+=branchOffset;
        }
    } 	

    void CPU::BPL(){
        if(!getFlag(PSFlag::NEGATIVE)){
            ++m_cycles;
            if((PC&0xFF00) != ((PC+branchOffset)&0xFF00)){
                ++m_cycles;
            }
            PC+=branchOffset;
        }
    } 	

    void CPU::BRK(){
        PC++;
	
        write8(STACK_BASE+SP, (PC>>8)&0x00FF);
        SP--;
        write8(STACK_BASE+SP, PC&0x00FF);
        SP--;
        write8(STACK_BASE+SP, PS|0x0010);
        SP--;
        setFlag(PSFlag::INT_DIS, true);
    
        PC = (read8(0xFFFE)) | (read8(0xFFFF) << 8);    //IRQ vector
    } 	

    void CPU::BVC(){
        if(!getFlag(PSFlag::OVERFLOW)){
            ++m_cycles;
            if((PC&0xFF00) != ((PC+branchOffset)&0xFF00)){
                ++m_cycles;
            }
            PC+=branchOffset;
        }
    } 	

    void CPU::BVS(){
        if(getFlag(PSFlag::OVERFLOW)){
            ++m_cycles;
            if((PC&0xFF00) != ((PC+branchOffset)&0xFF00)){
                ++m_cycles;
            }
            PC+=branchOffset;
        }
    } 	

    void CPU::CLC(){
        setFlag(PSFlag::CARRY, false);
    }    

    void CPU::CLD(){
        setFlag(PSFlag::DECIMAL, false);
    } 	

    void CPU::CLI(){
        setFlag(PSFlag::INT_DIS, false);
    } 	

    void CPU::CLV(){
        setFlag(PSFlag::OVERFLOW, false);
    } 	
    
    void CPU::CMP(){
        uint8_t value = read8(rw_Addr);
        setFlag(PSFlag::CARRY, A>=value);
        setFlag(PSFlag::ZERO, A==value);
        setFlag(PSFlag::NEGATIVE, (A-value)&0x80);
    }
    
    void CPU::CPX(){
        uint8_t value = read8(rw_Addr);
        setFlag(PSFlag::CARRY, IX>=value);
        setFlag(PSFlag::ZERO, IX==value);
        setFlag(PSFlag::NEGATIVE, (IX-value)&0x80);
    } 	
    
    void CPU::CPY(){
        uint8_t value = read8(rw_Addr);
        setFlag(PSFlag::CARRY, IY>=value);
        setFlag(PSFlag::ZERO, IY==value);
        setFlag(PSFlag::NEGATIVE, (IY-value)&0x80);
    }    
    
    void CPU::DEC(){
        uint8_t value = read8(rw_Addr)-1;
        write8(rw_Addr, value);
        setFlag(PSFlag::ZERO, value==0);
        setFlag(PSFlag::NEGATIVE, value&0x80);
    } 	
    
    void CPU::DEX(){
        IX-=1;
        setFlag(PSFlag::ZERO, IX==0);
        setFlag(PSFlag::NEGATIVE, IX&0x80);
    } 	
    
    void CPU::DEY(){
        IY-=1;
        setFlag(PSFlag::ZERO, IY==0);
        setFlag(PSFlag::NEGATIVE, IY&0x80);
    } 	
    
    void CPU::EOR(){
        A^=read8(rw_Addr);
        setFlag(PSFlag::ZERO, A==0);
        setFlag(PSFlag::NEGATIVE, A&0x80);
    }
    
    void CPU::INC(){
        uint8_t value = read8(rw_Addr)+1;
        write8(rw_Addr, value);
        setFlag(PSFlag::ZERO, value==0);
        setFlag(PSFlag::NEGATIVE, value&0x80);
    } 	
    
    void CPU::INX(){
        IX+=1;
        setFlag(PSFlag::ZERO, IX==0);
        setFlag(PSFlag::NEGATIVE, IX&0x80);
    } 	 
    
    void CPU::INY(){
        IY+=1;
        setFlag(PSFlag::ZERO, IY==0);
        setFlag(PSFlag::NEGATIVE, IY&0x80);
    } 	
    
    void CPU::JMP(){
        PC = rw_Addr;
    }   
    
    void CPU::JSR(){
        PC-=1;  //As per 6502 convention
        write8(STACK_BASE+SP, (PC&0xFF00)>>8);
        SP--;
        write8(STACK_BASE+SP, PC&0x00FF);
        SP--;
        PC = rw_Addr;
    } 	
    
    void CPU::LDA(){
        A = read8(rw_Addr);
        setFlag(PSFlag::ZERO, A==0);
        setFlag(PSFlag::NEGATIVE, A&0x80);
    }

    void CPU::LDX(){
        IX = read8(rw_Addr);
        setFlag(PSFlag::ZERO, IX==0);
        setFlag(PSFlag::NEGATIVE, IX&0x80);
    } 	
    
    void CPU::LDY(){
        IY = read8(rw_Addr);
        setFlag(PSFlag::ZERO, IY==0);
        setFlag(PSFlag::NEGATIVE, IY&0x80);
    } 	 
    
    void CPU::LSR(){
        uint8_t value = read8(rw_Addr);
        //Accumulator mode
        if(m_currentOpcode == 0x004A){
            value = A;
            A>>=1;
        }
        else{
            write8(rw_Addr, value>>1);
        }
        /* Get's the 1st bit (bit 0) and sets the flag */
        setFlag(PSFlag::CARRY, value&0x0001);
        value>>=1;
        setFlag(PSFlag::ZERO, value==0);
        setFlag(PSFlag::NEGATIVE, value&0x80);
    } 	
    
    void CPU::ORA(){
        A|=read8(rw_Addr);
        setFlag(PSFlag::ZERO, A==0);
        setFlag(PSFlag::NEGATIVE, A&0x80);
    } 	
    
    void CPU::PHA(){
        write8(STACK_BASE+SP, A);
        SP--;
    } 	 
    
    void CPU::PHP(){
        write8(STACK_BASE+SP, PS|0x30);
        SP--;
    } 	
    
    void CPU::PLA(){
        SP++;
        A = read8(STACK_BASE+SP);
        setFlag(PSFlag::ZERO, A==0);
        setFlag(PSFlag::NEGATIVE, A&0x80);
    } 	
    
    void CPU::PLP(){
        SP++;
        PS = read8(STACK_BASE+SP);
    } 	
    
    void CPU::ROL(){
        uint8_t value = read8(rw_Addr);
        bool oldFlag = getFlag(PSFlag::CARRY);
        /* No modifications to value under the conditional branches */
        if(m_currentOpcode == 0x002A){
            value = A;
            A<<=1;
            A|=(oldFlag);
        }
        else{
            write8(rw_Addr, (value<<1)|oldFlag);
        }
        //Extracting the carry bit from the initial(unaltered) value
        setFlag(PSFlag::CARRY, value&0x80);
        //Doing the same operations back on value to figure out what happened to A or the value stored in memory
        value<<=1;
        value|=oldFlag;
        setFlag(PSFlag::ZERO, value==0);
        setFlag(PSFlag::NEGATIVE, value&0x80);
    } 	
    
    void CPU::NOP(){
        //Does nothing HeHeHeHeHe
    }   
    
    void CPU::ROR(){
        uint8_t value = read8(rw_Addr);
        bool oldFlag = getFlag(PSFlag::CARRY);
        if(m_currentOpcode == 0x006A){
            value = A;
            A>>=1;
            A|=(oldFlag<<7);
        }
        else{
            write8(rw_Addr, (value>>1)|(oldFlag<<7));
        }
        setFlag(PSFlag::CARRY, value&0x0001);
        value>>=1;
        value|=(oldFlag<<7);
        setFlag(PSFlag::ZERO, value==0);
        setFlag(PSFlag::NEGATIVE, value&0x80);
    } 	
    
    void CPU::RTI(){
        SP++;
        PS = read8(STACK_BASE+SP);
        SP++;
        /* rw_Addr acts as dummy variable here */
        rw_Addr = read8(STACK_BASE+SP);
        SP++;
        rw_Addr = rw_Addr|(read8(STACK_BASE+SP)<<8);
        PC = rw_Addr;
    }
    
    void CPU::RTS(){
        SP++;
        /* rw_Addr acts as dummy variable here */
        rw_Addr = read8(STACK_BASE+SP);
        SP++;
        rw_Addr = rw_Addr|(read8(STACK_BASE+SP)<<8);
        PC = rw_Addr+1;
    } 	
    
    void CPU::SBC(){
        uint16_t value = read8(rw_Addr) ^ 0x00FF;

        uint8_t temp = A + value + getFlag(PSFlag::CARRY);
        setFlag(PSFlag::CARRY, temp&0xFF00);
        setFlag(PSFlag::ZERO, ((temp&0x00FF)==0));
        setFlag(PSFlag::OVERFLOW, (temp^(uint16_t)A) & (temp^value) & 0x0080);
        setFlag(PSFlag::NEGATIVE, temp&0x0080);
        A=temp & 0x00FF;
    } 	 
    
    void CPU::SEC(){
        setFlag(PSFlag::CARRY, true);
    } 	
    
    void CPU::SED(){
        setFlag(PSFlag::DECIMAL, true);
    } 	
    
    void CPU::SEI(){
        setFlag(PSFlag::INT_DIS, true);
    } 	
    
    void CPU::STA(){
        write8(rw_Addr, A);
    }
    
    void CPU::STX(){
        write8(rw_Addr, IX);
    } 	
    
    void CPU::STY(){
        write8(rw_Addr, IY);
    } 	 
    
    void CPU::TAX(){
        IX = A;
        setFlag(PSFlag::ZERO, IX==0);
        setFlag(PSFlag::NEGATIVE, IX&0x80);
    } 	
    
    void CPU::TAY(){
        IY = A;
        setFlag(PSFlag::ZERO, IY==0);
        setFlag(PSFlag::NEGATIVE, IY&0x80);
    } 	
    
    void CPU::TSX(){
        IX = SP;
        setFlag(PSFlag::ZERO, IX==0);
        setFlag(PSFlag::NEGATIVE, IX&0x80);
    } 	
    
    void CPU::TXA(){
        A = IX;
        setFlag(PSFlag::ZERO, A==0);
        setFlag(PSFlag::NEGATIVE, A&0x80);
    } 	
    
    void CPU::TXS(){
        SP = IX;
    } 	
    
    void CPU::TYA(){
        A = IY;
        setFlag(PSFlag::ZERO, A==0);
        setFlag(PSFlag::NEGATIVE, A&0x80);
    }

    void CPU::XXX(){

    }
}