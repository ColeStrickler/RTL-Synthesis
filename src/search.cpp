#include <iostream>

#include "search.hpp"

// deep copy of a AST with its head being OutputNode
RTLNode* Search::clone(RTLNode* toClone) {
    if (toClone == nullptr) {
        return nullptr;
    }

    RTLNode* newNode = nullptr;
    switch (toClone->nodetag) {
        case INPUT_NODE: {
            // this ends up being one base case
            InputNode* inputNode = static_cast<InputNode*>(toClone);
            InputNode* newInputNode = new InputNode();
            newNode = newInputNode;
            break;
        }
        case OUTPUT_NODE: {
            OutputNode* outputNode = static_cast<OutputNode*>(toClone);
            OutputNode* newOutputNode = new OutputNode();
            newOutputNode->Child = clone(outputNode->Child);
            newNode = newOutputNode;
            break;
        }
        case REG_NODE: {
            RegNode* regNode = static_cast<RegNode*>(toClone);
            RegNode* newRegNode = new RegNode();
            newRegNode->Child = clone(regNode->Child);
            newNode = newRegNode;
            break;
        }
        case PLUS_NODE: {
            PlusNode* plusNode = static_cast<PlusNode*>(toClone);
            PlusNode* newPlusNode = new PlusNode();
            newPlusNode->leftChild = clone(plusNode->leftChild);
            newPlusNode->rightChild = clone(plusNode->rightChild);
            newNode = newPlusNode;
            break;
        }
        case MINUS_NODE: {
            MinusNode* minusNode = static_cast<MinusNode*>(toClone);
            MinusNode* newMinusNode = new MinusNode();
            newMinusNode->leftChild = clone(minusNode->leftChild);
            newMinusNode->rightChild = clone(minusNode->rightChild);
            newNode = newMinusNode;
            break;
        }
        case TIMES_NODE: {
            TimesNode* timesNode = static_cast<TimesNode*>(toClone);
            TimesNode* newTimesNode = new TimesNode();
            newTimesNode->leftChild = clone(timesNode->leftChild);
            newTimesNode->rightChild = clone(timesNode->rightChild);
            newNode = newTimesNode;
            break;
        }
        case SHIFTL_NODE: {
            ShiftLeftNode* shiftLeftNode = static_cast<ShiftLeftNode*>(toClone);
            ShiftLeftNode* newShiftLeftNode = new ShiftLeftNode();
            newShiftLeftNode->leftChild = clone(shiftLeftNode->leftChild);
            newShiftLeftNode->rightChild = clone(shiftLeftNode->rightChild);
            newNode = newShiftLeftNode;
            break;
        }
        case SHIFTR_NODE: {
            ShiftRightNode* shiftRightNode = static_cast<ShiftRightNode*>(toClone);
            ShiftRightNode* newShiftRightNode = new ShiftRightNode();
            newShiftRightNode->leftChild = clone(shiftRightNode->leftChild);
            newShiftRightNode->rightChild = clone(shiftRightNode->rightChild);
            newNode = newShiftRightNode;
            break;
        }
        case BITOR_NODE: {
            BitwiseORNode* bitOrNode = static_cast<BitwiseORNode*>(toClone);
            BitwiseORNode* newBitOrNode = new BitwiseORNode();
            newBitOrNode->leftChild = clone(bitOrNode->leftChild);
            newBitOrNode->rightChild = clone(bitOrNode->rightChild);
            newNode = newBitOrNode;
            break;
        }
        case BITXOR_NODE: {
            BitWiseXORNode* bitXorNode = static_cast<BitWiseXORNode*>(toClone);
            BitWiseXORNode* newBitXorNode = new BitWiseXORNode();
            newBitXorNode->leftChild = clone(bitXorNode->leftChild);
            newBitXorNode->rightChild = clone(bitXorNode->rightChild);
            newNode = newBitXorNode;
            break;
        }
        case BITAND_NODE: {
            BitwiseANDNode* bitAndNode = static_cast<BitwiseANDNode*>(toClone);
            BitwiseANDNode* newBitAndNode = new BitwiseANDNode();
            newBitAndNode->leftChild = clone(bitAndNode->leftChild);
            newBitAndNode->rightChild = clone(bitAndNode->rightChild);
            newNode = newBitAndNode;
            break;
        }
        case BITNOT_NODE: {
            BitWiseNOTNode* bitNotNode = static_cast<BitWiseNOTNode*>(toClone);
            BitWiseNOTNode* newBitNotNode = new BitWiseNOTNode();
            newBitNotNode->Child = clone(bitNotNode->Child);
            newNode = newBitNotNode;
            break;
        }
        default:
            std::cerr << "Error: Unknown node type in clone function.\n";
            break;
    }

    return newNode;
}

// is an AST complete i.e. no nonterms i.e. no nullptr children
bool Search::isComplete(RTLNode* node) {
    if (node == nullptr) {
        return false;
    }

    switch (node->nodetag) {
    case INPUT_NODE:
        return true;
        break;
    case OUTPUT_NODE: {
        OutputNode* cast = static_cast<OutputNode*>(node);
        return isComplete(cast->Child);
        break;
    }
    case REG_NODE: {
        RegNode* cast = static_cast<RegNode*>(node);
        return isComplete(cast->Child);
        break;
    }
    case PLUS_NODE:
    case MINUS_NODE:
    case TIMES_NODE:
    case SHIFTL_NODE:
    case SHIFTR_NODE:
    case BITOR_NODE:
    case BITXOR_NODE:
    case BITAND_NODE:
    case BITNOT_NODE: {
        BinaryNode* cast = static_cast<BinaryNode*>(node);
        return isComplete(cast->leftChild) && isComplete(cast->rightChild);
        break;
    }
    default:
        std::cerr << "Error: Unknown node type in isComplete function.\n";
        break;
    }

    return false;
}

RTLNode* Search::leftMostNonTerm(RTLNode* node) {
    if (node == nullptr) {
        return nullptr;
    }

    
}