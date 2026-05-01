#include <iostream>
#include <thread>
#include <mutex>

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
            if (newOutputNode->Child) newOutputNode->Child->parent = newOutputNode;
            newNode = newOutputNode;
            break;
        }
        case REG_NODE: {
            RegNode* regNode = static_cast<RegNode*>(toClone);
            RegNode* newRegNode = new RegNode();
            newRegNode->Child = clone(regNode->Child);
            if (newRegNode->Child) newRegNode->Child->parent = newRegNode;
            newNode = newRegNode;
            break;
        }
        case PLUS_NODE: {
            PlusNode* plusNode = static_cast<PlusNode*>(toClone);
            PlusNode* newPlusNode = new PlusNode();
            newPlusNode->leftChild = clone(plusNode->leftChild);
            newPlusNode->rightChild = clone(plusNode->rightChild);
            if (newPlusNode->leftChild) newPlusNode->leftChild->parent = newPlusNode;
            if (newPlusNode->rightChild) newPlusNode->rightChild->parent = newPlusNode;
            newNode = newPlusNode;
            break;
        }
        case MINUS_NODE: {
            MinusNode* minusNode = static_cast<MinusNode*>(toClone);
            MinusNode* newMinusNode = new MinusNode();
            newMinusNode->leftChild = clone(minusNode->leftChild);
            newMinusNode->rightChild = clone(minusNode->rightChild);
            if (newMinusNode->leftChild) newMinusNode->leftChild->parent = newMinusNode;
            if (newMinusNode->rightChild) newMinusNode->rightChild->parent = newMinusNode;
            newNode = newMinusNode;
            break;
        }
        case TIMES_NODE: {
            TimesNode* timesNode = static_cast<TimesNode*>(toClone);
            TimesNode* newTimesNode = new TimesNode();
            newTimesNode->leftChild = clone(timesNode->leftChild);
            newTimesNode->rightChild = clone(timesNode->rightChild);
            if (newTimesNode->leftChild) newTimesNode->leftChild->parent = newTimesNode;
            if (newTimesNode->rightChild) newTimesNode->rightChild->parent = newTimesNode;
            newNode = newTimesNode;
            break;
        }
        case SHIFTL_NODE: {
            ShiftLeftNode* shiftLeftNode = static_cast<ShiftLeftNode*>(toClone);
            ShiftLeftNode* newShiftLeftNode = new ShiftLeftNode();
            newShiftLeftNode->leftChild = clone(shiftLeftNode->leftChild);
            newShiftLeftNode->rightChild = clone(shiftLeftNode->rightChild);
            if (newShiftLeftNode->leftChild) newShiftLeftNode->leftChild->parent = newShiftLeftNode;
            if (newShiftLeftNode->rightChild) newShiftLeftNode->rightChild->parent = newShiftLeftNode;
            newNode = newShiftLeftNode;
            break;
        }
        case SHIFTR_NODE: {
            ShiftRightNode* shiftRightNode = static_cast<ShiftRightNode*>(toClone);
            ShiftRightNode* newShiftRightNode = new ShiftRightNode();
            newShiftRightNode->leftChild = clone(shiftRightNode->leftChild);
            newShiftRightNode->rightChild = clone(shiftRightNode->rightChild);
            if (newShiftRightNode->leftChild) newShiftRightNode->leftChild->parent = newShiftRightNode;
            if (newShiftRightNode->rightChild) newShiftRightNode->rightChild->parent = newShiftRightNode;
            newNode = newShiftRightNode;
            break;
        }
        case BITOR_NODE: {
            BitwiseORNode* bitOrNode = static_cast<BitwiseORNode*>(toClone);
            BitwiseORNode* newBitOrNode = new BitwiseORNode();
            newBitOrNode->leftChild = clone(bitOrNode->leftChild);
            newBitOrNode->rightChild = clone(bitOrNode->rightChild);
            if (newBitOrNode->leftChild) newBitOrNode->leftChild->parent = newBitOrNode;
            if (newBitOrNode->rightChild) newBitOrNode->rightChild->parent = newBitOrNode;
            newNode = newBitOrNode;
            break;
        }
        case BITXOR_NODE: {
            BitWiseXORNode* bitXorNode = static_cast<BitWiseXORNode*>(toClone);
            BitWiseXORNode* newBitXorNode = new BitWiseXORNode();
            newBitXorNode->leftChild = clone(bitXorNode->leftChild);
            newBitXorNode->rightChild = clone(bitXorNode->rightChild);
            if (newBitXorNode->leftChild) newBitXorNode->leftChild->parent = newBitXorNode;
            if (newBitXorNode->rightChild) newBitXorNode->rightChild->parent = newBitXorNode;
            newNode = newBitXorNode;
            break;
        }
        case BITAND_NODE: {
            BitwiseANDNode* bitAndNode = static_cast<BitwiseANDNode*>(toClone);
            BitwiseANDNode* newBitAndNode = new BitwiseANDNode();
            newBitAndNode->leftChild = clone(bitAndNode->leftChild);
            newBitAndNode->rightChild = clone(bitAndNode->rightChild);
            if (newBitAndNode->leftChild) newBitAndNode->leftChild->parent = newBitAndNode;
            if (newBitAndNode->rightChild) newBitAndNode->rightChild->parent = newBitAndNode;
            newNode = newBitAndNode;
            break;
        }
        case BITNOT_NODE: {
            BitWiseNOTNode* bitNotNode = static_cast<BitWiseNOTNode*>(toClone);
            BitWiseNOTNode* newBitNotNode = new BitWiseNOTNode();
            newBitNotNode->Child = clone(bitNotNode->Child);
            if (newBitNotNode->Child) newBitNotNode->Child->parent = newBitNotNode;
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
    case BITAND_NODE: {
        BinaryNode* cast = static_cast<BinaryNode*>(node);
        return isComplete(cast->leftChild) && isComplete(cast->rightChild);
        break;
    }
    case BITNOT_NODE: {
        BitWiseNOTNode* cast = static_cast<BitWiseNOTNode*>(node);
        return isComplete(cast->Child);
        break;
    }
    default:
        std::cerr << "Error: Unknown node type in isComplete function.\n";
        break;
    }

    return false;
}

std::optional<NonTermLocation> Search::leftMostNonTerm(RTLNode* node) {
    if (node == nullptr) {
        return std::nullopt;
    }

    switch (node->nodetag) {
    case INPUT_NODE:
        return std::nullopt;
        break;
    case OUTPUT_NODE: {
        OutputNode* cast = static_cast<OutputNode*>(node);
        if (cast->Child == nullptr) {
            return NonTermLocation{node, NonTermLocation::CHILD};
        }
        return leftMostNonTerm(cast->Child);
        break;
    }
    case REG_NODE: {
        RegNode* cast = static_cast<RegNode*>(node);
        if (cast->Child == nullptr) {
            return NonTermLocation{node, NonTermLocation::CHILD};
        }
        return leftMostNonTerm(cast->Child);
        break;
    }
    case PLUS_NODE:
    case MINUS_NODE:
    case TIMES_NODE:
    case SHIFTL_NODE:
    case SHIFTR_NODE:
    case BITOR_NODE:
    case BITXOR_NODE:
    case BITAND_NODE: {
        BinaryNode* cast = static_cast<BinaryNode*>(node);
        if (cast->leftChild == nullptr) {
            return NonTermLocation{node, NonTermLocation::LEFT};
        }

        std::optional<NonTermLocation> leftResult = leftMostNonTerm(cast->leftChild);
        if (leftResult.has_value()) {
            return leftResult;
        }

        if (cast->rightChild == nullptr) {
            return NonTermLocation{node, NonTermLocation::RIGHT};
        }
        return leftMostNonTerm(cast->rightChild);
        break;
    }
    case BITNOT_NODE: {
        BitWiseNOTNode* cast = static_cast<BitWiseNOTNode*>(node);
        if (cast->Child == nullptr) {
            return NonTermLocation{node, NonTermLocation::CHILD};
        }
        return leftMostNonTerm(cast->Child);
        break;
    }
    default:
        std::cerr << "Error: Unknown node type in leftMostNonTerm function.\n";
        break;
    }

    return std::nullopt;
}

std::vector<NODETAG> Search::productions(const NonTermLocation& location) {
    switch (location.parent->nodetag) {
    case INPUT_NODE:
        // we should not get here
        std::cerr << "Error: Tried to get productions for terminal input node.\n";
        return {};
        break;
    case REG_NODE:
        return {PLUS_NODE, TIMES_NODE, SHIFTL_NODE,
                SHIFTR_NODE, BITOR_NODE, BITXOR_NODE, BITAND_NODE, BITNOT_NODE, MINUS_NODE};
        break;
    case OUTPUT_NODE:
      //  printf("OUTPUTNODE PROD\n");
        return {INPUT_NODE,PLUS_NODE, TIMES_NODE, SHIFTL_NODE,
                SHIFTR_NODE, BITOR_NODE, BITXOR_NODE, BITAND_NODE, BITNOT_NODE, MINUS_NODE,REG_NODE};
        break;
    case PLUS_NODE:
    case MINUS_NODE:
    case TIMES_NODE:
    case SHIFTL_NODE:
    case SHIFTR_NODE:
    case BITOR_NODE:
    case BITXOR_NODE:
    case BITAND_NODE:
    case BITNOT_NODE:
        return {INPUT_NODE,PLUS_NODE, TIMES_NODE, SHIFTL_NODE,
                SHIFTR_NODE, BITOR_NODE, BITXOR_NODE, BITAND_NODE, BITNOT_NODE, MINUS_NODE,REG_NODE};
        break;
    default:
        std::cerr << "Error: Unknown node tpye in productions functions.\n";
        break;
    }

    return {};
}

RTLNode* produceNode(NODETAG production) {
    switch (production) {
    case PLUS_NODE:
        return new PlusNode();
        break;
    case MINUS_NODE:
        return new MinusNode();
        break;
    case TIMES_NODE:
        return new TimesNode();
        break;
    case SHIFTL_NODE:
        return new ShiftLeftNode();
        break;
    case SHIFTR_NODE:
        return new ShiftRightNode();
        break;
    case BITOR_NODE:
        return new BitwiseORNode();
        break;
    case BITXOR_NODE:
        return new BitWiseXORNode();
        break;
    case BITAND_NODE:
        return new BitwiseANDNode();
        break;
    case BITNOT_NODE:
        return new BitWiseNOTNode();
        break;
    case REG_NODE:
        return new RegNode();
        break;
    case INPUT_NODE:
        return new InputNode();
        break;
    default:
        std::cerr << "Error: Unknown production type in produceNode function.\n";
        break;
    }

    return nullptr;
}

// replaces left most non terminal, call clone before
WorkItem* Search::replaceNonTerm(RTLNode* root, const NonTermLocation& location, NODETAG production) {
    if (root == nullptr || location.parent == nullptr) {
        return nullptr;
    }

    WorkItem* newItem = nullptr;
    RTLNode* newNode = produceNode(production);
   // printf("NODETAG: %d\n", newNode->nodetag);
    switch (location.slot) {
    case NonTermLocation::CHILD: {
        if (location.parent->nodetag == REG_NODE) {
            RegNode* cast = static_cast<RegNode*>(location.parent);
            cast->Child = newNode;
        } else if (location.parent->nodetag == OUTPUT_NODE) {
            OutputNode* cast = static_cast<OutputNode*>(location.parent);
            cast->Child = newNode;
        } else if (location.parent->nodetag == BITNOT_NODE) {
            BitWiseNOTNode* cast = static_cast<BitWiseNOTNode*>(location.parent);
            cast->Child = newNode;
        }
        newNode->parent = location.parent;
        break;
    }
    case NonTermLocation::LEFT: {
        BinaryNode* cast = static_cast<BinaryNode*>(location.parent);
        cast->leftChild = newNode;
        newNode->parent = location.parent;
        break;
    }
    case NonTermLocation::RIGHT: {
        BinaryNode* cast = static_cast<BinaryNode*>(location.parent);
        cast->rightChild = newNode;
        newNode->parent = location.parent;
        break;
    }
    default:
        std::cerr << "Error: Unknown slot in replaceNonTerm.\n";
        return nullptr;
        break;
    }

    return new WorkItem{root, 0, 0, 0};
}

uint32_t combDelayToBoundary(RTLNode* node, const CostModel& costModel) {
    uint32_t delay = 0;
    RTLNode* curr = node->GetParent();
    while (curr != nullptr) {
        if (curr->nodetag == REG_NODE || curr->nodetag == OUTPUT_NODE) {
            break;
        }
        delay += costModel.gateCost(curr->nodetag);
        curr = curr->parent;
    }

    return delay;
}

void cleanupAST(RTLNode* node) {
    if (node == nullptr)
        return;

    switch (node->nodetag) {
    case INPUT_NODE:
        delete node;
        break;
    case REG_NODE: {
        RegNode* cast = static_cast<RegNode*>(node);
        cleanupAST(cast->Child);
        delete cast;
        break;
    }
    case OUTPUT_NODE: {
        OutputNode* cast = static_cast<OutputNode*>(node);
        cleanupAST(cast->Child);
        delete cast;
        break;
    }
    case PLUS_NODE:
    case MINUS_NODE:
    case TIMES_NODE:
    case SHIFTL_NODE:
    case SHIFTR_NODE:
    case BITOR_NODE:
    case BITXOR_NODE:
    case BITAND_NODE: {
        BinaryNode* cast = static_cast<BinaryNode*>(node);
        cleanupAST(cast->leftChild);
        cleanupAST(cast->rightChild);
        delete cast;
        break;
    }
    case BITNOT_NODE: {
        BitWiseNOTNode* cast = static_cast<BitWiseNOTNode*>(node);
        cleanupAST(cast->Child);
        delete cast;
        break;
    }
    default:
        std::cerr << "Error: Tried to cleanup and unknown AST node type \n";
        break;
    }
}

WorkItem::~WorkItem() {
    if (node != nullptr) {
        cleanupAST(node);
    }
}

void Search::unroll(WorkItem* workItem, const CostModel& costModel) {
    if (auto nonTerm = leftMostNonTerm(workItem->node)) {
        std::vector<NODETAG> prods = productions(*nonTerm);
        for (auto& prod : prods) {
            RTLNode* cloned = clone(workItem->node);
            auto loc = leftMostNonTerm(cloned);
            WorkItem item = std::move(*replaceNonTerm(cloned, *loc, prod));
            item.combDelay= workItem->combDelay;
            item.totalCost = workItem->totalCost;

            if (prod == REG_NODE) {
                item.totalCost += costModel.regCost;
                item.combDelay = 0;
            } else if (prod == INPUT_NODE) {
                // NOTHING
            } else {
                item.combDelay = combDelayToBoundary(loc->parent, costModel) + costModel.gateCost(prod);
                item.totalCost +=  costModel.gateCost(prod);
               // printf("item cost %d\n", item.totalCost);
            }

            if (item.combDelay <= costModel.maxCombPath) {
                m_workList.push(std::move(item));
            }
        }
    }
  //  printf("worklist size %d\n", m_workList.size());
}

void Search::collectInputNodes(RTLNode* node, std::vector<InputNode*>& out) {
    if (node == nullptr) return;
    switch (node->nodetag) {
        case INPUT_NODE:
            out.push_back(static_cast<InputNode*>(node));
            break;
        case OUTPUT_NODE:
            //std::cout << static_cast<OutputNode*>(node)->Child << "\n";
            collectInputNodes(static_cast<OutputNode*>(node)->Child, out);
            break;
        case REG_NODE:
            collectInputNodes(static_cast<RegNode*>(node)->Child, out);
            break;
        case BITNOT_NODE:
            //std::cout << node->nodetag << "\n";
            collectInputNodes(static_cast<BitWiseNOTNode*>(node)->Child, out);
            break;
        default: {
            BinaryNode* b = static_cast<BinaryNode*>(node);
            collectInputNodes(b->leftChild, out);
            collectInputNodes(b->rightChild, out);
            break;
        }
    }
}
#define MAX_INPUT_FANOUT 1
#define MIN_INPUT_FANOUT 1
RTLNode* Search::topDown(const std::vector<std::vector<int>>& inputs, const std::vector<int>& outputs, const CostModel& costModel) {
    if (!m_workList.empty()) {
        std::cerr << "Error: starting search with a non-empty worklist. You messed something up.\n";
        return nullptr;
    }

    Verifier verify(1);

    WorkItem start = WorkItem(new OutputNode(), 0, 0, 0);
    m_workList.push(std::move(start));
    while (!m_workList.empty()) {
        WorkItem curr = std::move(const_cast<WorkItem&>(m_workList.top()));
        m_workList.pop();
        std::vector<InputNode*> liveInputs;
        collectInputNodes(curr.node, liveInputs);
        if (liveInputs.size() > MAX_INPUT_FANOUT*inputs[0].size()) {
            continue;
        }

        if (liveInputs.size() < inputs[0].size()*MIN_INPUT_FANOUT)
        {
            unroll(&curr, costModel);
            continue;
        }

        verify.Setup(inputs, outputs, liveInputs);

        bool complete = isComplete(curr.node);
        if (complete)
            std::cout << curr.node->PrintExpr() << std::endl;
        if (complete && verify.VerifyVM()) {
            std::cout << "SEARCH VERIFIED\n";
            RTLNode* temp = curr.node;
            curr.node = nullptr;
            return temp;
        }
        verify.Reset();

        unroll(&curr, costModel);
    }

    return nullptr;
}