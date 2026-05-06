#include "vm.hpp"

VM::VM()
{
    m_Program = new uint8_t[MAX_PROG_SIZE];   
    m_Registers = new uint32_t[MAX_REGISTERS];
    m_Inputs = new uint32_t[MAX_INPUT_SIZE];
    m_CurrentInstruction = 0;
    m_ActiveProgramSize = 0;
    m_NumInputs = 0;
    m_PC = 0;
}

void VM::Reset()
{
    m_CurrentInstruction = 0;
    m_ActiveProgramSize = 0;
    m_NumInputs = 0;
    m_PC = 0;
    while(m_Stack.size())
        m_Stack.pop();
}




VM::~VM()
{
    delete[] m_Inputs;
    delete[] m_Program;
    delete[] m_Registers;
}

void VM::Compile(std::vector<InputNode *> input_nodes)
{
    verifyInputNodes = input_nodes.size();
    if (input_nodes.size() >= MAX_INPUT_SIZE)
        return;

    std::unordered_map<int, RegNode*> node_map;
    std::vector<RTLNode*> currentRegNodes; // or outpuyt node

    std::queue<RTLNode*> frontier;

        ////printf("here\n");
        int x = 0;
        for (auto& inode: input_nodes) // give input nodes values
        {
            //m_NumInputs++;
            frontier.push(inode);
        }
       // ////printf("frontier size %d\n", frontier.size());
        bool done = false;

    bool left = false;
        std::unordered_set<RTLNode*> visited;
    while (!done)
    {
        currentRegNodes.clear();
        
        while (!frontier.empty() && !done)
        {
            auto node = frontier.front();
            frontier.pop();
            if (visited.count(node))
                continue;

            switch(node->nodetag)
            {
                case NODETAG::OUTPUT_NODE:
                {
                    currentRegNodes.push_back(static_cast<RegNode*>(node));
                    done = true;  break;
                }
                case NODETAG::REG_NODE:
                {
                    currentRegNodes.push_back(static_cast<RegNode*>(node));
                    visited.insert(node);
                    frontier.push(node->GetParent());
                    break;
                }
                default:
                {
                    visited.insert(node);
                    frontier.push(node->GetParent());
                    break;
                }
            }
        }

        if (left)
        {
            for (int i = 0; i < currentRegNodes.size(); i++)
            {
                currentRegNodes[i]->Compile(this);
            }
            left = false;
        }
        else
        {
            for (int i = currentRegNodes.size()-1; i >= 0; i--)
            {
                currentRegNodes[i]->Compile(this);
            }
            left = true;
        }


    }
}

void VM::CompileRegNode(RegNode *reg)
{
    reg->Compile(this); // this will just leave the reg value on the stack
}

int VM::ExecuteProgram()
{
    if (verifyInputNodes >= MAX_INPUT_SIZE)
        return 0;
    //printf("Program Size %d\n", m_ActiveProgramSize);

    OPCODE current_op;
    bool output_ready = false;
    uint32_t ret = 0;
    while(!output_ready)
    {
        current_op = IncProgramCounter();
        switch(current_op)
        {
            case OPCODE::PUSH_INPUT:    {uint8_t input_num = ReadProgramOperand(); PushStack(GetInput(input_num, true)); break;}
            case OPCODE::POP_RET:       {ret = PopStack(); output_ready = true; break;}
            case OPCODE::PUSH_REG:      {uint8_t reg_num = ReadProgramOperand();  PushStack(ReadReg(reg_num)); break;}
            case OPCODE::STORE_REG:     {uint8_t reg_num = ReadProgramOperand(); uint32_t val = PopStack(); StoreReg(reg_num, val); break;}
            case OPCODE::BIT_AND:       {uint32_t b = PopStack(); uint32_t a = PopStack(); PushStack(a & b); break;}
            case OPCODE::BIT_OR:        {uint32_t b = PopStack(); uint32_t a = PopStack(); PushStack(a | b); break;}
            case OPCODE::BIT_XOR:       {uint32_t b = PopStack(); uint32_t a = PopStack(); PushStack(a ^ b); break;}
            case OPCODE::DIV:           {uint32_t b = PopStack(); uint32_t a = PopStack(); PushStack(a / b); break;}
            case OPCODE::MINUS:         {uint32_t b = PopStack(); uint32_t a = PopStack(); PushStack(b - a); break;}
            case OPCODE::PLUS:          {uint32_t b = PopStack(); uint32_t a = PopStack(); PushStack(a + b); break;}
            case OPCODE::TIMES:         {uint32_t b = PopStack(); uint32_t a = PopStack(); PushStack(a * b); break;}
            case OPCODE::SHIFT_LEFT:    {uint32_t b = PopStack(); uint32_t a = PopStack(); PushStack(a << b); break;}
            case OPCODE::SHIFT_RIGHT:   {uint32_t b = PopStack(); uint32_t a = PopStack(); PushStack(a >> b); break;}
            case OPCODE::BIT_NOT:       {uint32_t a = PopStack(); PushStack(~a); break;}
            default:
            {
                ReportError("Invalid opcode.");
                return -1;
            }
        }
    }
    m_PC = 0;
    return (int)ret;
}



int VM::ExecuteProgramDebug()
{
    OPCODE current_op;
    bool output_ready = false;
    uint32_t ret = 0;
    m_CurrentInstruction = 0;
    m_PC = 0;
    while (!m_Stack.empty()) {
        m_Stack.pop();
    }

    while(!output_ready)
    {
        current_op = IncProgramCounter();
           // printf("m_PC %d\n", m_PC);
        switch(current_op)
        {
            case OPCODE::PUSH_INPUT:
            {
                uint8_t input_num = ReadProgramOperand();
                uint32_t val = GetInput(input_num, false);
                printf("PUSH_INPUT %u -> %u\n", input_num, val);
                PushStack(val);
                break;
            }

            case OPCODE::POP_RET:
            {
                ret = PopStack();
                printf("POP_RET -> %u\n", ret);
                output_ready = true;
                break;
            }

            case OPCODE::PUSH_REG:
            {
                uint8_t reg_num = ReadProgramOperand();
                uint32_t val = ReadReg(reg_num);
                printf("PUSH_REG %u -> %u\n", reg_num, val);
                PushStack(val);
                break;
            }

            case OPCODE::STORE_REG:
            {
                uint8_t reg_num = ReadProgramOperand();
                uint32_t val = PopStack();
                printf("STORE_REG %u <- %u\n", reg_num, val);
                StoreReg(reg_num, val);
                break;
            }

            case OPCODE::BIT_AND:
            {
                uint32_t b = PopStack();
                uint32_t a = PopStack();
                uint32_t out = a & b;
                printf("%u & %u = %u\n", a, b, out);
                PushStack(out);
                break;
            }

            case OPCODE::BIT_OR:
            {
                uint32_t b = PopStack();
                uint32_t a = PopStack();
                uint32_t out = a | b;
                printf("%u | %u = %u\n", a, b, out);
                PushStack(out);
                break;
            }

            case OPCODE::BIT_XOR:
            {
                uint32_t b = PopStack();
                uint32_t a = PopStack();
                uint32_t out = a ^ b;
                printf("%u ^ %u = %u\n", a, b, out);
                PushStack(out);
                break;
            }

            case OPCODE::DIV:
            {
                uint32_t b = PopStack();
                uint32_t a = PopStack();
                uint32_t out = a / b;
                printf("%u / %u = %u\n", a, b, out);
                PushStack(out);
                break;
            }

            case OPCODE::MINUS:
            {
                uint32_t b = PopStack();
                uint32_t a = PopStack();
                uint32_t out = a - b;
                printf("%u - %u = %u\n", a, b, out);
                PushStack(out);
                break;
            }

            case OPCODE::PLUS:
            {
                uint32_t b = PopStack();
                uint32_t a = PopStack();
                uint32_t out = a + b;
                printf("%u + %u = %u\n", a, b, out);
                PushStack(out);
                break;
            }

            case OPCODE::TIMES:
            {
                uint32_t b = PopStack();
                uint32_t a = PopStack();
                uint32_t out = a * b;
                printf("%u * %u = %u\n", a, b, out);
                PushStack(out);
                break;
            }

            case OPCODE::SHIFT_LEFT:
            {
                uint32_t b = PopStack();
                uint32_t a = PopStack();
                uint32_t out = a << b;
                printf("%u << %u = %u\n", a, b, out);
                PushStack(out);
                break;
            }

            case OPCODE::SHIFT_RIGHT:
            {
                uint32_t b = PopStack();
                uint32_t a = PopStack();
                uint32_t out = a >> b;
                printf("%u >> %u = %u\n", a, b, out);
                PushStack(out);
                break;
            }

            case OPCODE::BIT_NOT:
            {
                uint32_t a = PopStack();
                uint32_t out = ~a;
                printf("~%u = %u\n", a, out);
                PushStack(out);
                break;
            }

            default:
            {
                ReportError("Invalid opcode.");
                return -1;
            }
        }
    }

    printf("HERE\n");
    return (int)ret;
}







OPCODE VM::IncProgramCounter()
{
    assert(m_PC < m_ActiveProgramSize);
    return static_cast<OPCODE>(m_Program[m_PC++]);
}

uint8_t VM::ReadProgramOperand()
{
    return m_Program[m_PC++];
}

uint32_t VM::ReadReg(uint8_t reg)
{
#ifdef DEBUG
    assert(reg < m_ActiveRegisters);
#endif
    return m_Registers[reg];
}

void VM::StoreReg(uint8_t reg, uint32_t val)
{
 #ifdef DEBUG
    assert(reg < m_ActiveRegisters);
#endif
    m_Registers[reg] = val;
}

void VM::PushStack(uint32_t val)
{
    m_Stack.push(val);
#ifdef DEBUG
    assert(m_Stack.size() <= MAX_STACK_SIZE);
#endif
}

uint32_t VM::PopStack()
{
#ifdef DEBUG
    assert(m_Stack.size() > 0);
#endif
    auto top = m_Stack.top();
    m_Stack.pop();
    return top;
}

void VM::SetInput(uint8_t input, uint32_t val)
{
    //    printf("SET INPUT %d\n", input);
#ifdef DEBUG
    assert(input <= MAX_INPUT_SIZE);
#endif
    m_Inputs[input] = val;
}

uint32_t VM::GetInput(uint8_t input, bool debug)
{
       
#ifdef DEBUG
    if (!(input < MAX_INPUT_SIZE) && debug)
    {
        ExecuteProgramDebug();
        assert(input <= MAX_INPUT_SIZE);
    }
#endif
    if (!(input < MAX_INPUT_SIZE))
    {
        return 0;
    }

    return m_Inputs[input];
}

void VM::ReportError(const char *msg)
{
    ////printf("[VM::ERROR]: %s\n", msg);
    exit(-1);
}

void VM::SetInputs(const std::vector<uint32_t> &ivals)
{
#ifdef DEBUG
    assert(ivals.size() <= m_NumInputs);
#endif
    if (ivals.size() > MAX_INPUT_SIZE)
        return;
    for (int i = 0; i < ivals.size(); i++)
    {
        m_Inputs[i] = ivals[i];
    }
}

void VM::PrintProgram()
{
   // printf("PrintProgram()! Program Size: %d\n", m_ActiveProgramSize);
    int i = 0;
    while(i < m_ActiveProgramSize)
    {
        OPCODE op = static_cast<OPCODE>(m_Program[i++]);
        std::string op_str = opcode_to_string(op);
        
        switch(op)
        {
            case OPCODE::PUSH_REG: {op_str + " " +  std::to_string(m_Program[i++]); break;}
            case OPCODE::STORE_REG: {op_str + " " +  std::to_string(m_Program[i++]); break;}
            case OPCODE::PUSH_INPUT: {op_str + " " +  std::to_string(m_Program[i++]); break;}
            default: break;
        } 

        std::cout << op_str << "\n";
    }

}

void VM::PushInputInst(uint8_t input)
{
   // printf("INPUT %d\n", input);
    m_Program[m_ActiveProgramSize++] = static_cast<uint8_t>(OPCODE::PUSH_INPUT);
    m_Program[m_ActiveProgramSize++] = input;
}

void VM::StoreRegInst(uint8_t reg)
{
    m_Program[m_ActiveProgramSize++] = static_cast<uint8_t>(OPCODE::STORE_REG);
    m_Program[m_ActiveProgramSize++] = reg;
}

void VM::PushRegInst(uint8_t reg)
{
    m_Program[m_ActiveProgramSize++] = static_cast<uint8_t>(OPCODE::PUSH_REG);
    m_Program[m_ActiveProgramSize++] = reg;
}

void VM::BinOpInst(OPCODE op)
{
    m_Program[m_ActiveProgramSize++] = static_cast<uint8_t>(op);
}

void VM::OutputInst()
{
    m_Program[m_ActiveProgramSize++] = static_cast<uint8_t>(OPCODE::POP_RET);
}
