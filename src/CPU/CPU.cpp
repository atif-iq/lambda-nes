#include "CPU.h"
#include <fstream>
#include <ios>

/*
 *  Magic Numbers (Special numbers used in hexadecimal or binary formats for operation(s) directly)
 *  0x00FF -> A bit mask for getting lower 8 bits
 *  0x0080 -> A bit mask for getting negative flag 
 */

namespace Lambda{
    CPU::CPU(){
        m_bus = new BUS();
        m_bus->attachComponent(std::make_unique<RAM>());
        reset();

        /* The wholly mamoth of a task! Filling the god damn instruction set*/
        m_instructions = new Instruction[256];
        for (int i = 0; i < 256; i++) {
            m_instructions[i] = Instruction("???", "???", std::bind(&CPU::XXX, this), std::bind(&CPU::IMP, this), 2);
        }

        m_instructions[0x00] = Instruction("BRK", "IMP", std::bind(&CPU::BRK, this), std::bind(&CPU::IMP, this), 7);
        m_instructions[0x01] = Instruction("ORA", "IDX", std::bind(&CPU::ORA, this), std::bind(&CPU::IDX, this), 6);
        m_instructions[0x05] = Instruction("ORA", "ZP ", std::bind(&CPU::ORA, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0x06] = Instruction("ASL", "ZP ", std::bind(&CPU::ASL, this), std::bind(&CPU::ZP , this), 5);
        m_instructions[0x08] = Instruction("PHP", "IMP", std::bind(&CPU::PHP, this), std::bind(&CPU::IMP, this), 3);
        m_instructions[0x09] = Instruction("ORA", "IMM", std::bind(&CPU::ORA, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0x0A] = Instruction("ASL", "IMP", std::bind(&CPU::ASL, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x0D] = Instruction("ORA", "ABS", std::bind(&CPU::ORA, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0x0E] = Instruction("ASL", "ABS", std::bind(&CPU::ASL, this), std::bind(&CPU::ABS, this), 6);
        m_instructions[0x10] = Instruction("BPL", "REL", std::bind(&CPU::BPL, this), std::bind(&CPU::REL, this), 2);
        m_instructions[0x11] = Instruction("ORA", "IDY", std::bind(&CPU::ORA, this), std::bind(&CPU::IDY, this), 5);
        m_instructions[0x15] = Instruction("ORA", "ZPX", std::bind(&CPU::ORA, this), std::bind(&CPU::ZPX, this), 4);
        m_instructions[0x16] = Instruction("ASL", "ZPX", std::bind(&CPU::ASL, this), std::bind(&CPU::ZPX, this), 6);
        m_instructions[0x18] = Instruction("CLC", "IMP", std::bind(&CPU::CLC, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x19] = Instruction("ORA", "ABY", std::bind(&CPU::ORA, this), std::bind(&CPU::ABY, this), 4);
        m_instructions[0x1D] = Instruction("ORA", "ABX", std::bind(&CPU::ORA, this), std::bind(&CPU::ABX, this), 4);
        m_instructions[0x1E] = Instruction("ASL", "ABX", std::bind(&CPU::ASL, this), std::bind(&CPU::ABX, this), 7);
        m_instructions[0x20] = Instruction("JSR", "ABS", std::bind(&CPU::JSR, this), std::bind(&CPU::ABS, this), 6);
        m_instructions[0x21] = Instruction("AND", "IDX", std::bind(&CPU::AND, this), std::bind(&CPU::IDX, this), 6);
        m_instructions[0x24] = Instruction("BIT", "ZP ", std::bind(&CPU::BIT, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0x25] = Instruction("AND", "ZP ", std::bind(&CPU::AND, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0x26] = Instruction("ROL", "ZP ", std::bind(&CPU::ROL, this), std::bind(&CPU::ZP , this), 5);
        m_instructions[0x28] = Instruction("PLP", "IMP", std::bind(&CPU::PLP, this), std::bind(&CPU::IMP, this), 4);
        m_instructions[0x29] = Instruction("AND", "IMM", std::bind(&CPU::AND, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0x2A] = Instruction("ROL", "IMP", std::bind(&CPU::ROL, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x2C] = Instruction("BIT", "ABS", std::bind(&CPU::BIT, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0x2D] = Instruction("AND", "ABS", std::bind(&CPU::AND, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0x2E] = Instruction("ROL", "ABS", std::bind(&CPU::ROL, this), std::bind(&CPU::ABS, this), 6);
        m_instructions[0x30] = Instruction("BMI", "REL", std::bind(&CPU::BMI, this), std::bind(&CPU::REL, this), 2);
        m_instructions[0x31] = Instruction("AND", "IDY", std::bind(&CPU::AND, this), std::bind(&CPU::IDY, this), 5);
        m_instructions[0x35] = Instruction("AND", "ZPX", std::bind(&CPU::AND, this), std::bind(&CPU::ZPX, this), 4);
        m_instructions[0x36] = Instruction("ROL", "ZPX", std::bind(&CPU::ROL, this), std::bind(&CPU::ZPX, this), 6);
        m_instructions[0x38] = Instruction("SEC", "IMP", std::bind(&CPU::SEC, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x39] = Instruction("AND", "ABY", std::bind(&CPU::AND, this), std::bind(&CPU::ABY, this), 4);
        m_instructions[0x3D] = Instruction("AND", "ABX", std::bind(&CPU::AND, this), std::bind(&CPU::ABX, this), 4);
        m_instructions[0x3E] = Instruction("ROL", "ABX", std::bind(&CPU::ROL, this), std::bind(&CPU::ABX, this), 7);
        m_instructions[0x40] = Instruction("RTI", "IMP", std::bind(&CPU::RTI, this), std::bind(&CPU::IMP, this), 6);
        m_instructions[0x41] = Instruction("EOR", "IDX", std::bind(&CPU::EOR, this), std::bind(&CPU::IDX, this), 6);
        m_instructions[0x45] = Instruction("EOR", "ZP ", std::bind(&CPU::EOR, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0x46] = Instruction("LSR", "ZP ", std::bind(&CPU::LSR, this), std::bind(&CPU::ZP , this), 5);
        m_instructions[0x48] = Instruction("PHA", "IMP", std::bind(&CPU::PHA, this), std::bind(&CPU::IMP, this), 3);
        m_instructions[0x49] = Instruction("EOR", "IMM", std::bind(&CPU::EOR, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0x4A] = Instruction("LSR", "IMP", std::bind(&CPU::LSR, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x4C] = Instruction("JMP", "ABS", std::bind(&CPU::JMP, this), std::bind(&CPU::ABS, this), 3);
        m_instructions[0x4D] = Instruction("EOR", "ABS", std::bind(&CPU::EOR, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0x4E] = Instruction("LSR", "ABS", std::bind(&CPU::LSR, this), std::bind(&CPU::ABS, this), 6);
        m_instructions[0x50] = Instruction("BVC", "REL", std::bind(&CPU::BVC, this), std::bind(&CPU::REL, this), 2);
        m_instructions[0x51] = Instruction("EOR", "IDY", std::bind(&CPU::EOR, this), std::bind(&CPU::IDY, this), 5);
        m_instructions[0x55] = Instruction("EOR", "ZPX", std::bind(&CPU::EOR, this), std::bind(&CPU::ZPX, this), 4);
        m_instructions[0x56] = Instruction("LSR", "ZPX", std::bind(&CPU::LSR, this), std::bind(&CPU::ZPX, this), 6);
        m_instructions[0x58] = Instruction("CLI", "IMP", std::bind(&CPU::CLI, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x59] = Instruction("EOR", "ABY", std::bind(&CPU::EOR, this), std::bind(&CPU::ABY, this), 4);
        m_instructions[0x5D] = Instruction("EOR", "ABX", std::bind(&CPU::EOR, this), std::bind(&CPU::ABX, this), 4);
        m_instructions[0x5E] = Instruction("LSR", "ABX", std::bind(&CPU::LSR, this), std::bind(&CPU::ABX, this), 7);
        m_instructions[0x60] = Instruction("RTS", "IMP", std::bind(&CPU::RTS, this), std::bind(&CPU::IMP, this), 6);
        m_instructions[0x61] = Instruction("ADC", "IDX", std::bind(&CPU::ADC, this), std::bind(&CPU::IDX, this), 6);
        m_instructions[0x65] = Instruction("ADC", "ZP ", std::bind(&CPU::ADC, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0x66] = Instruction("ROR", "ZP ", std::bind(&CPU::ROR, this), std::bind(&CPU::ZP , this), 5);
        m_instructions[0x68] = Instruction("PLA", "IMP", std::bind(&CPU::PLA, this), std::bind(&CPU::IMP, this), 4);
        m_instructions[0x69] = Instruction("ADC", "IMM", std::bind(&CPU::ADC, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0x6A] = Instruction("ROR", "IMP", std::bind(&CPU::ROR, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x6C] = Instruction("JMP", "IND", std::bind(&CPU::JMP, this), std::bind(&CPU::IND, this), 5);
        m_instructions[0x6D] = Instruction("ADC", "ABS", std::bind(&CPU::ADC, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0x6E] = Instruction("ROR", "ABS", std::bind(&CPU::ROR, this), std::bind(&CPU::ABS, this), 6);
        m_instructions[0x70] = Instruction("BVS", "REL", std::bind(&CPU::BVS, this), std::bind(&CPU::REL, this), 2);
        m_instructions[0x71] = Instruction("ADC", "IDY", std::bind(&CPU::ADC, this), std::bind(&CPU::IDY, this), 5);
        m_instructions[0x75] = Instruction("ADC", "ZPX", std::bind(&CPU::ADC, this), std::bind(&CPU::ZPX, this), 4);
        m_instructions[0x76] = Instruction("ROR", "ZPX", std::bind(&CPU::ROR, this), std::bind(&CPU::ZPX, this), 6);
        m_instructions[0x78] = Instruction("SEI", "IMP", std::bind(&CPU::SEI, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x79] = Instruction("ADC", "ABY", std::bind(&CPU::ADC, this), std::bind(&CPU::ABY, this), 4);
        m_instructions[0x7D] = Instruction("ADC", "ABX", std::bind(&CPU::ADC, this), std::bind(&CPU::ABX, this), 4);
        m_instructions[0x7E] = Instruction("ROR", "ABX", std::bind(&CPU::ROR, this), std::bind(&CPU::ABX, this), 7);
        m_instructions[0x81] = Instruction("STA", "IDX", std::bind(&CPU::STA, this), std::bind(&CPU::IDX, this), 6);
        m_instructions[0x84] = Instruction("STY", "ZP ", std::bind(&CPU::STY, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0x85] = Instruction("STA", "ZP ", std::bind(&CPU::STA, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0x86] = Instruction("STX", "ZP ", std::bind(&CPU::STX, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0x88] = Instruction("DEY", "IMP", std::bind(&CPU::DEY, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x8A] = Instruction("TXA", "IMP", std::bind(&CPU::TXA, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x8C] = Instruction("STY", "ABS", std::bind(&CPU::STY, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0x8D] = Instruction("STA", "ABS", std::bind(&CPU::STA, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0x8E] = Instruction("STX", "ABS", std::bind(&CPU::STX, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0x90] = Instruction("BCC", "REL", std::bind(&CPU::BCC, this), std::bind(&CPU::REL, this), 2);
        m_instructions[0x91] = Instruction("STA", "IDY", std::bind(&CPU::STA, this), std::bind(&CPU::IDY, this), 6);
        m_instructions[0x94] = Instruction("STY", "ZPX", std::bind(&CPU::STY, this), std::bind(&CPU::ZPX, this), 4);
        m_instructions[0x95] = Instruction("STA", "ZPX", std::bind(&CPU::STA, this), std::bind(&CPU::ZPX, this), 4);
        m_instructions[0x96] = Instruction("STX", "ZPY", std::bind(&CPU::STX, this), std::bind(&CPU::ZPY, this), 4);
        m_instructions[0x98] = Instruction("TYA", "IMP", std::bind(&CPU::TYA, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x99] = Instruction("STA", "ABY", std::bind(&CPU::STA, this), std::bind(&CPU::ABY, this), 5);
        m_instructions[0x9A] = Instruction("TXS", "IMP", std::bind(&CPU::TXS, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0x9D] = Instruction("STA", "ABX", std::bind(&CPU::STA, this), std::bind(&CPU::ABX, this), 5);
        m_instructions[0xA0] = Instruction("LDY", "IMM", std::bind(&CPU::LDY, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0xA1] = Instruction("LDA", "IDX", std::bind(&CPU::LDA, this), std::bind(&CPU::IDX, this), 6);
        m_instructions[0xA2] = Instruction("LDX", "IMM", std::bind(&CPU::LDX, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0xA4] = Instruction("LDY", "ZP ", std::bind(&CPU::LDY, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0xA5] = Instruction("LDA", "ZP ", std::bind(&CPU::LDA, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0xA6] = Instruction("LDX", "ZP ", std::bind(&CPU::LDX, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0xA8] = Instruction("TAY", "IMP", std::bind(&CPU::TAY, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0xA9] = Instruction("LDA", "IMM", std::bind(&CPU::LDA, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0xAA] = Instruction("TAX", "IMP", std::bind(&CPU::TAX, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0xAC] = Instruction("LDY", "ABS", std::bind(&CPU::LDY, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0xAD] = Instruction("LDA", "ABS", std::bind(&CPU::LDA, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0xAE] = Instruction("LDX", "ABS", std::bind(&CPU::LDX, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0xB0] = Instruction("BCS", "REL", std::bind(&CPU::BCS, this), std::bind(&CPU::REL, this), 2);
        m_instructions[0xB1] = Instruction("LDA", "IDY", std::bind(&CPU::LDA, this), std::bind(&CPU::IDY, this), 5);
        m_instructions[0xB4] = Instruction("LDY", "ZPX", std::bind(&CPU::LDY, this), std::bind(&CPU::ZPX, this), 4);
        m_instructions[0xB5] = Instruction("LDA", "ZPX", std::bind(&CPU::LDA, this), std::bind(&CPU::ZPX, this), 4);
        m_instructions[0xB6] = Instruction("LDX", "ZPY", std::bind(&CPU::LDX, this), std::bind(&CPU::ZPY, this), 4);
        m_instructions[0xB8] = Instruction("CLV", "IMP", std::bind(&CPU::CLV, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0xB9] = Instruction("LDA", "ABY", std::bind(&CPU::LDA, this), std::bind(&CPU::ABY, this), 4);
        m_instructions[0xBA] = Instruction("TSX", "IMP", std::bind(&CPU::TSX, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0xBC] = Instruction("LDY", "ABX", std::bind(&CPU::LDY, this), std::bind(&CPU::ABX, this), 4);
        m_instructions[0xBD] = Instruction("LDA", "ABX", std::bind(&CPU::LDA, this), std::bind(&CPU::ABX, this), 4);
        m_instructions[0xBE] = Instruction("LDX", "ABY", std::bind(&CPU::LDX, this), std::bind(&CPU::ABY, this), 4);
        m_instructions[0xC0] = Instruction("CPY", "IMM", std::bind(&CPU::CPY, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0xC1] = Instruction("CMP", "IDX", std::bind(&CPU::CMP, this), std::bind(&CPU::IDX, this), 6);
        m_instructions[0xC4] = Instruction("CPY", "ZP ", std::bind(&CPU::CPY, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0xC5] = Instruction("CMP", "ZP ", std::bind(&CPU::CMP, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0xC6] = Instruction("DEC", "ZP ", std::bind(&CPU::DEC, this), std::bind(&CPU::ZP , this), 5);
        m_instructions[0xC8] = Instruction("INY", "IMP", std::bind(&CPU::INY, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0xC9] = Instruction("CMP", "IMM", std::bind(&CPU::CMP, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0xCA] = Instruction("DEX", "IMP", std::bind(&CPU::DEX, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0xCC] = Instruction("CPY", "ABS", std::bind(&CPU::CPY, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0xCD] = Instruction("CMP", "ABS", std::bind(&CPU::CMP, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0xCE] = Instruction("DEC", "ABS", std::bind(&CPU::DEC, this), std::bind(&CPU::ABS, this), 6);
        m_instructions[0xD0] = Instruction("BNE", "REL", std::bind(&CPU::BNE, this), std::bind(&CPU::REL, this), 2);
        m_instructions[0xD1] = Instruction("CMP", "IDY", std::bind(&CPU::CMP, this), std::bind(&CPU::IDY, this), 5);
        m_instructions[0xD5] = Instruction("CMP", "ZPX", std::bind(&CPU::CMP, this), std::bind(&CPU::ZPX, this), 4);
        m_instructions[0xD6] = Instruction("DEC", "ZPX", std::bind(&CPU::DEC, this), std::bind(&CPU::ZPX, this), 6);
        m_instructions[0xD8] = Instruction("CLD", "IMP", std::bind(&CPU::CLD, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0xD9] = Instruction("CMP", "ABY", std::bind(&CPU::CMP, this), std::bind(&CPU::ABY, this), 4);
        m_instructions[0xDD] = Instruction("CMP", "ABX", std::bind(&CPU::CMP, this), std::bind(&CPU::ABX, this), 4);
        m_instructions[0xDE] = Instruction("DEC", "ABX", std::bind(&CPU::DEC, this), std::bind(&CPU::ABX, this), 7);
        m_instructions[0xE0] = Instruction("CPX", "IMM", std::bind(&CPU::CPX, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0xE1] = Instruction("SBC", "IDX", std::bind(&CPU::SBC, this), std::bind(&CPU::IDX, this), 6);
        m_instructions[0xE4] = Instruction("CPX", "ZP ", std::bind(&CPU::CPX, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0xE5] = Instruction("SBC", "ZP ", std::bind(&CPU::SBC, this), std::bind(&CPU::ZP , this), 3);
        m_instructions[0xE6] = Instruction("INC", "ZP ", std::bind(&CPU::INC, this), std::bind(&CPU::ZP , this), 5);
        m_instructions[0xE8] = Instruction("INX", "IMP", std::bind(&CPU::INX, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0xE9] = Instruction("SBC", "IMM", std::bind(&CPU::SBC, this), std::bind(&CPU::IMM, this), 2);
        m_instructions[0xEA] = Instruction("NOP", "IMP", std::bind(&CPU::NOP, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0xEC] = Instruction("CPX", "ABS", std::bind(&CPU::CPX, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0xED] = Instruction("SBC", "ABS", std::bind(&CPU::SBC, this), std::bind(&CPU::ABS, this), 4);
        m_instructions[0xEE] = Instruction("INC", "ABS", std::bind(&CPU::INC, this), std::bind(&CPU::ABS, this), 6);      
        m_instructions[0xF0] = Instruction("BEQ", "REL", std::bind(&CPU::BEQ, this), std::bind(&CPU::REL, this), 2);
        m_instructions[0xF1] = Instruction("SBC", "IDY", std::bind(&CPU::SBC, this), std::bind(&CPU::IDY, this), 5);
        m_instructions[0xF5] = Instruction("SBC", "ZPX", std::bind(&CPU::SBC, this), std::bind(&CPU::ZPX, this), 4);
        m_instructions[0xF6] = Instruction("INC", "ZPX", std::bind(&CPU::INC, this), std::bind(&CPU::ZPX, this), 6);
        m_instructions[0xF8] = Instruction("SED", "IMP", std::bind(&CPU::SED, this), std::bind(&CPU::IMP, this), 2);
        m_instructions[0xF9] = Instruction("SBC", "ABY", std::bind(&CPU::SBC, this), std::bind(&CPU::ABY, this), 4);
        m_instructions[0xFD] = Instruction("SBC", "ABX", std::bind(&CPU::SBC, this), std::bind(&CPU::ABX, this), 4);
        m_instructions[0xFE] = Instruction("INC", "ABX", std::bind(&CPU::INC, this), std::bind(&CPU::ABX, this), 7);
        emulation_paused = true;    //Begins with a paused state
    }

    void CPU::fetch_decode_execute(){
        oldPC = PC;
        m_currentOpcode = read8(PC++);
        m_instructions[m_currentOpcode].addrFunction();        
        m_instructions[m_currentOpcode].function(); 
        m_cycles += m_instructions[m_currentOpcode].base_cycles;
    }

    void CPU::reset(){
        PC = 0x0400;    //For testing the Klaus Dorman test
        oldPC = 0x0400;
        m_currentOpcode = read8(PC);
        SP = 0x00FD;    //Init code pushes 3 values to the , 0x00FF - 3 = 0x00FD
        IX = 0x0000;    
        IY = 0x0000;
        A  = 0x0000;
        PS = 0b00100100;

        m_cycles = 7;
        loadROM("test.bin");
    }

    uint8_t CPU::read8(uint16_t addr){
        return m_bus->read8(addr);
    }

    void CPU::write8(uint16_t addr, uint8_t value){
        m_bus->write8(addr, value);
    }

    void CPU::loadROM(const std::string& fname){
        std::ifstream file(fname, std::ios_base::binary | std::ios_base::app);
        if(!file){
            LAMBDA_LOG(LogLevel::CRITICAL, "Failed loading rom file {}", fname);
        }
        uint8_t byte;
        //The begin address as of now it's in testing phase that is why it exists
        uint16_t address = 0x0000;
        while (file.read(reinterpret_cast<char*>(&byte), 1)){
            m_bus->write8(address, byte);
            ++address;
        }
        m_romName = fname;
    }

    bool CPU::running(){
        return (!emulation_paused);
    }

    void CPU::setFlag(PSFlag flag, bool value){
    
        if(value){
            PS |= (1<<static_cast<uint8_t>(flag));
        }

        else{
            PS &= (~(1<<static_cast<uint8_t>(flag)));
        }
    }

    bool CPU::getFlag(PSFlag flag){
        return (PS&(1<<static_cast<uint8_t>(flag)));
    }
    
    CPU::~CPU(){
        delete m_bus;
    }
    
}