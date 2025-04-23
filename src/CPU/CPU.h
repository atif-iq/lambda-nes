#pragma once
#include <cstdint>
#include <functional>
#include <fstream>
#include "../BUS/BUS.h"
#include "../RAM/RAM.h"

class GUI;

#define STACK_BASE 0x0100

namespace Lambda{
    class CPU{
    public:
        CPU();
        void fetch_decode_execute();
        void reset();
        bool running(); //Returns the state of emulation - paused or running
        void write8(uint16_t addr, uint8_t value);
        uint8_t read8(uint16_t addr);
        ~CPU();
        
        friend class GUI;   //To give GUI acess to everything for debugging
        private:
        
        /*---INSTRUCTION SET----------------------------------------------*/
        struct Instruction{
            public:
            Instruction(std::string n = "???", std::string addr="???", std::function<void()> func = nullptr, std::function<void()> addrFunc = nullptr, uint8_t cycles = 2){
                name = n;
                addrName = addr;
                base_cycles = cycles;
                function = func;
                addrFunction = addrFunc;
            }

            /*---For Debugging Purposes--------------------------------------*/
            std::string name;
            std::string addrName;
            /*---------------------------------------------------------------*/
            
            std::function<void()> function;
            std::function<void()> addrFunction;
            uint8_t base_cycles;
            private:
        };

        Instruction* m_instructions;

        /*---Legal Instructions------------------------------------------*/
        void ADC(); 	void AND(); 	void ASL(); 	void BCC(); 	void BCS(); 	void BEQ();
        void BIT(); 	void BMI(); 	void BNE(); 	void BPL(); 	void BRK(); 	void BVC(); 	
        void BVS(); 	void CLC();     void CLD(); 	void CLI(); 	void CLV(); 	void CMP();
        void CPX(); 	void CPY();     void DEC(); 	void DEX(); 	void DEY(); 	void EOR();
        void INC(); 	void INX(); 	void INY(); 	void JMP();     void JSR(); 	void LDA();
        void ORA(); 	void PHA(); 	void PHP(); 	void PLA(); 	void PLP(); 	void ROL(); 	
        void LDX(); 	void LDY(); 	void LSR(); 	void NOP();     void ROR(); 	void RTI();
        void RTS(); 	void SBC(); 	void SEC(); 	void SED(); 	void SEI(); 	void STA();
        void STX(); 	void STY(); 	void TAX(); 	void TAY(); 	void TSX(); 	void TXA(); 	
        void TXS(); 	void TYA();     void XXX();
        
        /*---Addressing Modes---------------------------------------------*/
        void IMP();     void ACC();     void IMM();     void ZP();      void ZPX();     void ZPY();
        void REL();     void ABS();     void ABX();     void ABY();     void IND();     void IDX();    
        void IDY();
        
        /*---REGISTERS----------------------------------------------------*/
        uint16_t PC;
        uint8_t  SP;
        uint8_t  IX;
        uint8_t  IY;
        uint8_t  A ;
        uint8_t  PS;

        /*---MISC / UTILS-------------------------------------------------*/
        enum class PSFlag: uint8_t{
            CARRY   = 0,
            ZERO    = 1,
            INT_DIS = 2,
            DECIMAL = 3,
            BREAK   = 4,
            UNUSED  = 5,
            OVERFLOW= 6,
            NEGATIVE= 7,
        };

        //Setting and clearing a specific Program Status bit
        void setFlag(PSFlag flag, bool value);
        //Gettng a specific Program Status bit
        bool getFlag(PSFlag flag);
        
        void loadROM(const std::string& fname);


        bool emulation_paused;
        std::string m_romName;
        uint16_t oldPC;         //The actual PC changes due to manipulation of the PC by branching or changing instructions, so a constant dummy called oldPC is kept  
        uint32_t m_cycles;      //Total cycles executed
        uint16_t rw_Addr;       //Helper variable for addressing modes, basically all opcodes use this when they need an address from where something is to be read from or written to. Check AddressingModes.cpp for details on how it's implemented
        int8_t branchOffset;    //Helper variable (almost same as above) majorly used by branching commands on how many bytes to jump
        uint8_t m_currentOpcode;//Helper varible

        BUS* m_bus; //BUS... It Works...
    };
}