#ifndef VM_HPP
#define VM_HPP
#include <stdint.h>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <string>
#include <iostream>

#include "node.hpp"




#define MAX_REGISTERS 16
#define MAX_PROG_SIZE 512
#define MAX_STACK_SIZE 64
#define MAX_INPUT_SIZE 8

enum class OPCODE : uint8_t {
    PUSH_INPUT,
    POP_RET,
    PUSH_REG,
    STORE_REG,
    BIT_AND,
    BIT_OR,
    BIT_XOR,
    PLUS,
    MINUS,
    TIMES,
    DIV,
    SHIFT_LEFT,
    SHIFT_RIGHT
};



inline std::string opcode_to_string(OPCODE op) {
    switch (op) {
        case OPCODE::PUSH_INPUT:   return "PUSH_INPUT";
        case OPCODE::POP_RET:      return "POP_RET";
        case OPCODE::PUSH_REG:     return "PUSH_REG";
        case OPCODE::STORE_REG:    return "STORE_REG";

        case OPCODE::BIT_AND:      return "BIT_AND";
        case OPCODE::BIT_OR:       return "BIT_OR";
        case OPCODE::BIT_XOR:      return "BIT_XOR";

        case OPCODE::PLUS:         return "PLUS";
        case OPCODE::MINUS:        return "MINUS";
        case OPCODE::TIMES:        return "TIMES";
        case OPCODE::DIV:          return "DIV";

        case OPCODE::SHIFT_LEFT:   return "SHIFT_LEFT";
        case OPCODE::SHIFT_RIGHT:  return "SHIFT_RIGHT";
    }

    return "UNKNOWN_OPCODE";
}


inline OPCODE nodetag_to_opcode(NODETAG tag) {
    switch (tag) {
        case NODETAG::BITAND_NODE:
            return OPCODE::BIT_AND;

        case NODETAG::BITOR_NODE:
            return OPCODE::BIT_OR;

        case NODETAG::BITXOR_NODE:
            return OPCODE::BIT_XOR;

        case NODETAG::PLUS_NODE:
            return OPCODE::PLUS;

        case NODETAG::MINUS_NODE:
            return OPCODE::MINUS;

        case NODETAG::TIMES_NODE:
            return OPCODE::TIMES;

        case NODETAG::SHIFTL_NODE:
            return OPCODE::SHIFT_LEFT;

        case NODETAG::SHIFTR_NODE:
            return OPCODE::SHIFT_RIGHT;

        default:
        {
            printf("nodetag_to_opcode() received bad tag %d\n", tag);
            exit(-1);
        }
    }
}


class VM
{
public:
    VM();
    ~VM();
    void Compile(std::vector<InputNode *> input_nodes);
    void CompileRegNode(RegNode* reg);
    int ExecuteProgram();




    OPCODE IncProgramCounter();
    uint8_t ReadProgramOperand();
    uint32_t ReadReg(uint8_t reg);
    void StoreReg(uint8_t reg, uint32_t val);
    void PushStack(uint32_t val);
    uint32_t PopStack();

    void SetInput(uint8_t input, uint32_t val);
    uint32_t GetInput(uint8_t input);

    void ReportError(const char* msg);

    void SetInputs(const std::vector<uint32_t>& ivals);
    void PrintProgram();




    // Instruction Generators
    void PushInputInst(uint8_t input);
    void StoreRegInst(uint8_t reg);
    void PushRegInst(uint8_t reg);
    void BinOpInst(OPCODE op);
    void OutputInst();


    // Compile Info
    int m_CurrentInstruction;
    uint8_t m_NumInputs;
    uint32_t m_ActiveRegisters;
    uint32_t m_ActiveProgramSize;

    



private:


    // Runtime info





    std::stack<uint32_t> m_Stack;
    uint16_t m_PC;
    uint8_t* m_Program;
    uint32_t* m_Inputs;
    uint32_t m_ProgramCounter;
    uint32_t* m_Registers;


};











#endif