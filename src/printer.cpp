#include "printer.hpp"


std::string BinaryNode::Print(RTLModulePrint *mod_print)
{
    std::string ret = leftChild->Print(mod_print) + rightChild->Print(mod_print);
    std::string lchild_name = mod_print->GetNodeName(leftChild);
    std::string rchild_name = mod_print->GetNodeName(rightChild);

    mod_print->NameWireNode(this);
    std::string wire_name = mod_print->GetNodeName(this);
    mod_print->AddDecl("\tval " + wire_name + " = Wire(UInt(32.W))\n");


    std::string op = "";
    switch (nodetag)
    {
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

    ret += "\t" + wire_name + " := " + lchild_name + " "  + op + " " + rchild_name + "\n";
    return ret;
}


std::string RegNode::Print(RTLModulePrint *mod_print)
{
    std::string ret = Child->Print(mod_print);
    std::string child_name = mod_print->GetNodeName(Child);
    mod_print->NameRegNode(this);
    std::string reg_name = mod_print->GetNodeName(this);
    mod_print->AddDecl("\tval " + reg_name + " = Reg(UInt(32.W))\n");
    ret += + "\t" + reg_name + " := " + child_name + "\n";
    return ret;
}


std::string OutputNode::Print(RTLModulePrint *mod_print)
{
    std::string ret = Child->Print(mod_print);
    std::string child_name = mod_print->GetNodeName(Child);
    ret += "\tio.output := " + child_name + "\n";
    return ret;
}


std::string InputNode::Print(RTLModulePrint *mod_print)
{
    std::string ret = "";
    mod_print->NameInputNode(this);
    mod_print->AddInput(this);
    return ret;
}