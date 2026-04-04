#ifndef AST_HPP
#define AST_HPP

#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <cassert>


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
    BITNOT_NODE
};

class RTLNode
{
public:
    RTLNode(NODETAG tag) : nodetag(tag), parent(nullptr), val(0), inputs_received(0), inputs_needed(0) {}
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

    RTLNode* parent;
    int val;
    int inputs_received;
    int inputs_needed;
    NODETAG nodetag;

private:
    
};



// Nodes with two children
class PlusNode : public RTLNode
{
public:
    PlusNode() : RTLNode(PLUS_NODE) {inputs_needed = 2;}

    void PropagateVal() override {val = leftChild->val + rightChild->val; printf("add val %d\n", val); inputs_received = 0; parent->inputs_received++; };
    RTLNode* leftChild;
    RTLNode* rightChild;
};

class TimesNode : public RTLNode
{
public:
    TimesNode() : RTLNode(TIMES_NODE) {inputs_needed = 2;}

    void PropagateVal() override {val = leftChild->val * rightChild->val; printf("mult val %d\n", val); inputs_received = 0; parent->inputs_received++; };
    RTLNode* leftChild;
    RTLNode* rightChild;
};

class ShiftLeftNode : public RTLNode
{
public:
    ShiftLeftNode() : RTLNode(SHIFTL_NODE) {inputs_needed = 2;}



    void PropagateVal() override {val = leftChild->val << rightChild->val; inputs_received = 0; parent->inputs_received++; };
    RTLNode* leftChild;
    RTLNode* rightChild;
};

class ShiftRightNode : public RTLNode
{
public:
    ShiftRightNode() : RTLNode(SHIFTR_NODE) {inputs_needed = 2;}

    void PropagateVal() override {val = leftChild->val >> rightChild->val; inputs_received = 0; parent->inputs_received++; };
    RTLNode* leftChild;
    RTLNode* rightChild;
};

class BitwiseORNode : public RTLNode
{
public:
    BitwiseORNode() : RTLNode(BITOR_NODE) {inputs_needed = 2;}


    void PropagateVal() override {val = leftChild->val | rightChild->val; inputs_received = 0; parent->inputs_received++; };
    RTLNode* leftChild;
    RTLNode* rightChild;
};

class BitWiseXORNode : public RTLNode
{
public:
    BitWiseXORNode() : RTLNode(BITXOR_NODE) {inputs_needed = 2;}
    void PropagateVal() override {val = leftChild->val ^ rightChild->val; inputs_received = 0; parent->inputs_received++; };
    RTLNode* leftChild;
    RTLNode* rightChild;
};

class BitwiseANDNode : public RTLNode
{
public:
    BitwiseANDNode() : RTLNode(BITAND_NODE) {inputs_needed = 2;}
    void PropagateVal() override {val = leftChild->val & rightChild->val; inputs_received = 0; parent->inputs_received++; };
    RTLNode* leftChild;
    RTLNode* rightChild;
};

class BitWiseNOTNode : public RTLNode
{
public:
    BitWiseNOTNode() : RTLNode(BITNOT_NODE)
    {
        inputs_needed = 1;
    }


    void PropagateVal() override {val = ~Child->val; inputs_received = 0; parent->inputs_received++; };
    RTLNode* Child;

};


class InputNode : public RTLNode
{
public:
    InputNode() : RTLNode(INPUT_NODE)
    {
        inputs_needed = 0;
    }
    void PropagateVal() override {parent->inputs_received++; printf("input val %d\n", val); return;};

    void SetVal(int value) {val = value; }

};


class OutputNode : public RTLNode
{
public:
    OutputNode() : RTLNode(OUTPUT_NODE)
    {
        inputs_needed = 1;
    }

    void PropagateVal() override {val = Child->val; inputs_received = 0;};

    RTLNode* Child;

};


// Node with only one child
class RegNode : public RTLNode
{
public:
    RegNode() : RTLNode(REG_NODE) 
    {
        parent = nullptr; // explicitly null for single-child nodes
        inputs_needed = 1;
    }

    void PropagateVal() override {val = Child->val; inputs_received = 0; parent->inputs_received++; };

    RTLNode* Child;
};









class Verifier
{
public:
   Verifier(const std::vector<std::vector<int>> &input, const std::vector<int> &output, std::vector<InputNode*> input_nodes);
    ~Verifier();


    bool Verify(int i);
    bool VerifySpecificCombo(int input_idx, int combo);

    void BackTrackPermuteInputs(\
            std::vector<int>& input_vec, int input_count, std::vector<std::vector<int>>& ret, std::vector<int>& curr, std::unordered_map<int, int>& count);
    
    void SetMaxInputFanout(int fanout) {max_input_fanout = fanout;}

private:
    int max_input_fanout;

    std::vector<std::vector<int>> input_combination_indexes;

    std::vector<InputNode*> m_InputNodes;
    std::vector<std::vector<int>> m_Input;
    std::vector<int> m_Output;
};









#endif
