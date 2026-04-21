#ifndef PRINTER_HPP
#define PRINTER_HPP



#include <string>
#include <unordered_map>
#include <string>

#include "node.hpp"


class RTLModulePrint
{
public:
    RTLModulePrint() {
        m_InputNum = 0;
        m_WireNum = 0;
        m_RegNum = 0;
    }


    static std::string PrintModule(std::string mod_name, OutputNode* out) {
        auto rtlPrint = new RTLModulePrint();

        rtlPrint->m_Output = out;

        std::string module_body = out->Print(rtlPrint);

        std::string ret = "class " + mod_name + " extends Module {";
        ret += rtlPrint->PrintIO();
        ret += rtlPrint->GetDecls();
        ret += module_body;
        ret += "}\n";

        delete rtlPrint;
        return ret;
    }


    std::string PrintIO() {
        std::string ret = "val io = IO(new Bundle {\n";

        for (auto& in: m_Inputs)
            ret += "\tval " + m_NodeNames[static_cast<RTLNode*>(in)] + " =\tInput(UInt(32.W))\n";

        ret  += "\tval output =\tOutput(UInt(32.W))\n";

        ret += "})\n";
        return ret;
    }


    void NameInputNode(RTLNode* node)  {
        m_NodeNames.insert({node, "input" + std::to_string(m_InputNum++)});
    } 

    void NameRegNode(RTLNode* node)  {
        m_NodeNames.insert({node, "reg" + std::to_string(m_RegNum++)});
    } 


    void NameWireNode(RTLNode* node)  {
        m_NodeNames.insert({node, "wire" + std::to_string(m_WireNum++)});
    } 



    std::string GetNodeName(RTLNode* node) {
        assert (m_NodeNames.find(node) !=  m_NodeNames.end());

        bool io = node->nodetag == NODETAG::OUTPUT_NODE || node->nodetag == NODETAG::INPUT_NODE;
        std::string name = m_NodeNames.at(node);

        name = io ? "io." + name : name;
        return  name;
    }


    void AddInput(RTLNode* input_node) {
        m_Inputs.push_back(static_cast<InputNode*>(input_node));
    }


    void AddDecl(std::string d) {
        decl += d;
    }
    
    std::string GetDecls() {
        return decl;
    }

private:
    std::string decl;
    int m_InputNum;
    int m_RegNum;
    int m_WireNum;
    std::unordered_map<RTLNode*, std::string> m_NodeNames;
    std::vector<InputNode*> m_Inputs;
    OutputNode* m_Output;

};
#endif