#include "node.hpp"

#include "vm.hpp"
#include <algorithm>
#include <random>
Verifier::~Verifier()
{
}

Verifier::Verifier(const std::vector<std::vector<int>> &input, const std::vector<int> &output, std::vector<InputNode *> input_nodes) : \
 m_Input(input), m_Output(output), m_InputNodes(input_nodes), max_input_fanout(8), input_perm_check(0)

{

    std::vector<int> input_vec; // just use indexes
    for (int i = 0; i < m_Input[0].size(); i++)
        input_vec.push_back(i);


    std::vector<std::vector<int>> ret;
    std::vector<int> curr = {};
    std::unordered_map<int, int> count_map;
    BackTrackPermuteInputs(input_vec, m_InputNodes.size(), input_combination_indexes,  curr, count_map);
    std::mt19937 g(42);  // deterministic (recommended for benchmarking)

std::shuffle(input_combination_indexes.begin(),
             input_combination_indexes.end(),
             g);
    printf("init size %d\n", input_combination_indexes.size());
    m_VM = new VM();
}

bool Verifier::Verify(int i)
{
    
    if (i == m_Input.size())
        return true;

    // we BFS to a RegNode 
    // then we do it again, where the RegNodes are leaves
    // when we hit output node we stop

    /*
        We first find a combination all combinations of inputs that work on
        input example 1.


    
    */

    std::queue<RTLNode*> frontier;
    std::vector<RegNode*> currentRegNodes;


    
    int expected_output = m_Output[i];

    auto input = m_Input[i];


    


        /*
            For each input example,

            We generate all valid combinations for the input nodes

            We tag the value, then search forward from the nodes.
        */

    int combo = 0;
    printf("input_combination_idx_size %d\n", input_combination_indexes.size());
    for (auto& input_combination: input_combination_indexes)
    {
        printf("here\n");
        int x = 0;
        for (auto& inode: m_InputNodes) // give input nodes values
        {
            
            inode->SetVal(input[input_combination[x]]);
            x++;
            frontier.push(inode);
        }
       // printf("frontier size %d\n", frontier.size());
        bool done = false;
        while (!done)
        {
            std::unordered_set<RTLNode*> visited;
            for (auto& rnode: currentRegNodes)
            {
                auto rnode_parent = rnode->GetParent();
                if (visited.count(rnode_parent))
                    continue;
                visited.insert(rnode_parent);
                frontier.push(rnode_parent);
            }
            currentRegNodes.clear();
            
            while (!frontier.empty())
            {
                
                auto node = frontier.front();
                frontier.pop();
                
                
                switch(node->nodetag)
                {
                    case NODETAG::REG_NODE:
                    {
                        assert(node->ReceivedInputs());
                        node->PropagateVal();
                        currentRegNodes.push_back(static_cast<RegNode*>(node));
                        break;
                    }
                    case NODETAG::RTL_NODE: // not properly initialzied
                    {
                        assert(false);
                    }
                    case NODETAG::OUTPUT_NODE: // check against output
                    {
                         
                       // printf("output node\n");
                        assert(node->ReceivedInputs());
                        
                        node->PropagateVal();
                      //  printf("output node\n");
                        if (node->val == expected_output)
                        {
                            if (VerifySpecificCombo(i+1, combo))
                                return true;
                        }
                       // printf("restart %d\n", frontier.size());
                        done = true;
                        break;
                        // if correct, we can either go to next example
                        // or continue enumerating all correct permutations for this example
                    }
                    default:
                    {
                        if (!node->ReceivedInputs())
                        {
                            frontier.push(node);
                            break;
                        }
                        printf("default\n");
                        node->PropagateVal();
                        auto parent = node->GetParent();
                        if (visited.count(parent))
                            break;
                        visited.insert(parent);
                        frontier.push(parent);
                    }
                }
            }
        }
        combo++;
    }
    printf("combo = %d\n", combo);
    return false;
}

bool Verifier::VerifyVM()
{


    m_VM->Compile(m_InputNodes);
    printf("Compiled\n");
       // vm.SetInputs({2,1,1, 1, 1, 1});
    //vm.PrintProgram();


    return VMVerifier(0);
}

bool Verifier::VMVerifier(int i)
{
    if (i >= m_Output.size())
    {
        return true;
    }
    
    for (auto& input_combination: input_combination_indexes)
    {
        std::vector<uint32_t> input_vals;
        printf("here\n");
        int x = 0;
        for (auto& inode: m_InputNodes) // give input nodes values
        {
            input_vals.push_back((m_Input[i][input_combination[x]]));
            x++;
        }
        m_VM->SetInputs(input_vals);
        int res = m_VM->ExecuteProgram();
        if (res == m_Output[i])
        {
            if (VMVerifier(i+1))
                return true;
            
        }
    }
  
    return false;
}

bool Verifier::VerifySpecificCombo(int input_idx, int combo)
{
    
    if (input_idx == m_Input.size())
    {
        input_perm_check = combo;
       
        return true;
    }

    // we BFS to a RegNode 
    // then we do it again, where the RegNodes are leaves
    // when we hit output node we stop

    /*
        We first find a combination all combinations of inputs that work on
        input example 1.


    
    */

    std::queue<RTLNode*> frontier;
    std::vector<RegNode*> currentRegNodes;


    
    int expected_output = m_Output[input_idx];

    auto input = m_Input[input_idx];

        /*
            For each input example,

            We generate all valid combinations for the input nodes

            We tag the value, then search forward from the nodes.
        */



        auto& input_combination = input_combination_indexes[combo];

            int x = 0;
            for (auto& inode: m_InputNodes) // give input nodes values
            {

                
                inode->SetVal(input[input_combination[x]]);
                x++;
                frontier.push(inode);
            }
            printf("frontier size %d\n", frontier.size());



            bool done = false;


            while (!done)
            {
                std::unordered_set<RTLNode*> visited;


                for (auto& rnode: currentRegNodes)
                {
                    auto rnode_parent = rnode->GetParent();
                    if (visited.count(rnode_parent))
                        continue;

                    visited.insert(rnode_parent);
                    frontier.push(rnode_parent);
                }
                currentRegNodes.clear();
                





                while (!frontier.empty())
                {
                    
                    auto node = frontier.front();
                    frontier.pop();
                    

                    
                    switch(node->nodetag)
                    {


                        case NODETAG::REG_NODE:
                        {
                            assert(node->ReceivedInputs());
                            node->PropagateVal();
                            currentRegNodes.push_back(static_cast<RegNode*>(node));
                            break;
                        }
                        case NODETAG::RTL_NODE: // not properly initialzied
                        {
                            assert(false);
                        }
                        case NODETAG::OUTPUT_NODE: // check against output
                        {
                             
                            printf("output node\n");
                            assert(node->ReceivedInputs());
                            
                            node->PropagateVal();
                            printf("output node\n");
                            if (node->val == expected_output)
                            {
                                return VerifySpecificCombo(input_idx+1, combo);
                            }
                            printf("restart combo\n");
                            return false;
                            break;
                            // if correct, we can either go to next example
                            // or continue enumerating all correct permutations for this example
                        }
                        default:
                        {
                            if (!node->ReceivedInputs())
                            {
                                frontier.push(node);
                                break;
                            }
                            //printf("default\n");
                            node->PropagateVal();
                            auto parent = node->GetParent();
                            if (visited.count(parent))
                                break;
                            visited.insert(parent);
                            frontier.push(parent);
                        }
                    }

                }
            }
        
    
        
    return false;
}

/*
    We can remove some overhead by making this with while loop instead of recursive

    it is just easier for now



    We can also save the previous ones for later. Kind of like dynamic programming.

*/
void  Verifier::BackTrackPermuteInputs(std::vector<int>& input_vec, int input_count, std::vector<std::vector<int>>& ret,\
         std::vector<int>& curr, std::unordered_map<int, int>& count)
{

    if (input_count == curr.size())
    {
        ret.push_back(curr);
        return;
    }


    for (auto& input: input_vec)
    {
        if (count[input] < max_input_fanout)
        {
            curr.push_back(input);
            count[input]++;
            BackTrackPermuteInputs(input_vec, input_count, ret, curr, count);
            count[input]--;
            curr.pop_back();
        }
    }

    return;
}

void BinaryNode::Compile(VM *vm)
{
    printf("%s->Compile()   0x%x, 0x%x\n", opcode_to_string(nodetag_to_opcode(nodetag)).c_str(), leftChild, rightChild);
    rightChild->Compile(vm);
    leftChild->Compile(vm);
    

    vm->BinOpInst(nodetag_to_opcode(nodetag));
}

void InputNode::Compile(VM *vm)
{
    printf("input?\n");
    vm->PushInputInst(vm->m_NumInputs++);
}

void OutputNode::Compile(VM *vm)
{
    
    Child->Compile(vm);
    printf("ADding output inst\n");
    printf("here\n");
    vm->OutputInst();
}

void RegNode::Compile(VM *vm)
{
    Child->Compile(vm);
}
