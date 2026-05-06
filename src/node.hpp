#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <cassert>
class VM;
class RTLModulePrint;

enum NODETAG
{
    RTL_NODE,
    REG_NODE,
    INPUT_NODE,
    OUTPUT_NODE,
    PLUS_NODE,
    TIMES_NODE,
    SHIFTL_NODE,
    SHIFTR_NODE,
    BITOR_NODE,
    BITXOR_NODE,
    BITAND_NODE,
    BITNOT_NODE,
    MINUS_NODE
};

class RTLNode
{
public:
    RTLNode(NODETAG tag) : nodetag(tag), parent(nullptr), val(0), inputs_received(0), inputs_needed(0), cost(0) {}
    virtual ~RTLNode() 
    {

    }
    virtual void PropagateVal() {
        return;
    }

    virtual RTLNode* GetParent()
    {
        return parent;
    }

    virtual bool ReceivedInputs()
    {
       // printf("(%d) %d == %d\n", nodetag, inputs_received, inputs_needed);
        return inputs_received == inputs_needed;
    }


    virtual void Compile(VM* vm)
    {
        return;
    }

    virtual std::string PrintExpr() = 0;

    virtual std::string Print(RTLModulePrint* mod_print) = 0;


    RTLNode* parent;
    int val;
    int inputs_received;
    int inputs_needed;
    NODETAG nodetag;
    uint32_t cost;

private:
    
};


class BinaryNode : public RTLNode
{
public:
    BinaryNode(NODETAG tag)
        : RTLNode(tag), leftChild(nullptr), rightChild(nullptr)
    {
        inputs_needed = 2;
    }
    virtual std::string PrintExpr() {
        std::string op;
        switch (nodetag) {
            case NODETAG::BITAND_NODE:op = "&";break;
            case NODETAG::BITOR_NODE:op = "|";break;
            case NODETAG::BITXOR_NODE:op = "^";break;
            case NODETAG::MINUS_NODE:op = "-";break;
            case NODETAG::PLUS_NODE:op = "+";break;
            case NODETAG::SHIFTL_NODE:op = "<<";break;
            case NODETAG::SHIFTR_NODE:op = ">>";break;
            case NODETAG::TIMES_NODE:op = "*";break;
            default:
                assert(false);
        }
        return "(" + leftChild->PrintExpr() + op + rightChild->PrintExpr() + ")";
    }
    virtual void Compile(VM* vm) override;
    virtual std::string Print(RTLModulePrint* mod_print) override;


    RTLNode* leftChild;
    RTLNode* rightChild;

};



// Nodes with two children
class PlusNode : public BinaryNode 
{
public:
    PlusNode() : BinaryNode(PLUS_NODE) {inputs_needed = 2;}

    void PropagateVal() override {val = leftChild->val + rightChild->val; inputs_received = 0; parent->inputs_received++; };
};

class MinusNode : public BinaryNode 
{
public:
    MinusNode() : BinaryNode(MINUS_NODE) {inputs_needed = 2;}

    void PropagateVal() override {val = leftChild->val - rightChild->val; inputs_received = 0; parent->inputs_received++; };
};



class TimesNode : public BinaryNode
{
public:
    TimesNode() : BinaryNode(TIMES_NODE) {inputs_needed = 2;}

    void PropagateVal() override {val = leftChild->val * rightChild->val; inputs_received = 0; parent->inputs_received++; };
};

class ShiftLeftNode : public BinaryNode
{
public:
    ShiftLeftNode() : BinaryNode(SHIFTL_NODE) {inputs_needed = 2;}



    void PropagateVal() override {val = leftChild->val << rightChild->val; inputs_received = 0; parent->inputs_received++; };
};

class ShiftRightNode : public BinaryNode
{
public:
    ShiftRightNode() : BinaryNode(SHIFTR_NODE) {inputs_needed = 2;}

    void PropagateVal() override {val = leftChild->val >> rightChild->val; inputs_received = 0; parent->inputs_received++; };
};

class BitwiseORNode : public BinaryNode
{
public:
    BitwiseORNode() : BinaryNode(BITOR_NODE) {inputs_needed = 2;}


    void PropagateVal() override {val = leftChild->val | rightChild->val; inputs_received = 0; parent->inputs_received++; };
};

class BitWiseXORNode : public BinaryNode
{
public:
    BitWiseXORNode() : BinaryNode(BITXOR_NODE) {inputs_needed = 2;}
    void PropagateVal() override {val = leftChild->val ^ rightChild->val; inputs_received = 0; parent->inputs_received++; };
};

class BitwiseANDNode : public BinaryNode
{
public:
    BitwiseANDNode() : BinaryNode(BITAND_NODE) {inputs_needed = 2;}
    void PropagateVal() override {val = leftChild->val & rightChild->val; inputs_received = 0; parent->inputs_received++; };
};

class BitWiseNOTNode : public RTLNode
{
public:
    BitWiseNOTNode() : RTLNode(BITNOT_NODE), Child(nullptr)
    {
        inputs_needed = 1;
    }

        virtual std::string PrintExpr() { return "~" + Child->PrintExpr();}
    void PropagateVal() override {val = ~Child->val; inputs_received = 0; parent->inputs_received++; };
    virtual void Compile(VM* vm) override;
    virtual std::string Print(RTLModulePrint* mod_print) override;
    RTLNode* Child;

};


class InputNode : public RTLNode
{
public:
    InputNode() : RTLNode(INPUT_NODE)
    {
        inputs_needed = 0;
    }
    void PropagateVal() override {parent->inputs_received++; return;};
    virtual void Compile(VM* vm) override;
    virtual std::string Print(RTLModulePrint* mod_print) override;

    virtual std::string PrintExpr() { return  std::string("in");}

    void SetVal(int value) {val = value; }

};


class OutputNode : public RTLNode
{
public:
    OutputNode() : RTLNode(OUTPUT_NODE), Child(nullptr)
    {
        inputs_needed = 1;
    }

    void PropagateVal() override {val = Child->val; inputs_received = 0;};
    virtual void Compile(VM* vm) override;

    virtual std::string PrintExpr() { return "Out(" + Child->PrintExpr() + ")";}
    void Search();
    virtual std::string Print(RTLModulePrint* mod_print) override;
    RTLNode* Child;

};


// Node with only one child
class RegNode : public RTLNode
{
public:
    RegNode() : RTLNode(REG_NODE), Child(nullptr)
    {
        parent = nullptr; // explicitly null for single-child nodes
        inputs_needed = 1;
        compiled = false;
    }

    virtual std::string PrintExpr() { return "Reg(" + Child->PrintExpr() + ")";}

    virtual void Compile(VM* vm) override;
    void PropagateVal() override {val = Child->val; inputs_received = 0; parent->inputs_received++; };
    virtual std::string Print(RTLModulePrint* mod_print) override;
    RTLNode* Child;
    bool compiled;
};









class Verifier
{
public:
   Verifier(int max_input_fan_out);
    ~Verifier();

    void setInputNodes(std::vector<InputNode*> input_nodes) { m_InputNodes = input_nodes; };

    void Setup(const std::vector<std::vector<int>> &input, const std::vector<int> &output, std::vector<InputNode*> input_nodes);

    bool Verify(int i);
    bool VerifyVM();
    
    bool VerifyGPU();
    bool VMVerifier(int i);
    bool VMVerifierSpecificPermutation(int i, int j);
    bool VerifySpecificCombo(int input_idx, int combo);

    void BackTrackPermuteInputs(\
            std::vector<int>& input_vec, int input_count, std::vector<std::vector<int>>& ret, std::vector<int>& curr, std::unordered_map<int, int>& count);

    void SetMaxInputFanout(int fanout) {max_input_fanout = fanout;}



    void Reset();
        // Stats
    int input_perm_check;

private:

    VM* m_VM;
    int max_input_fanout;

    std::unordered_map<int, std::vector<std::vector<int>>> input_combination_indexes;

    //std::vector<std::vector<int>> input_combination_indexes;

    std::vector<InputNode*> m_InputNodes;
    std::vector<std::vector<int>> m_Input;
    std::vector<int> m_Output;
};









#endif


