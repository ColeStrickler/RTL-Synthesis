#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <optional>
#include <queue>
#include <stdint.h>
#include <vector>

#include "node.hpp"

struct CostModel {
    uint32_t nBits = 32;
    uint32_t maxCombPath;
    uint32_t regCost;

    uint32_t gateCost(NODETAG tag) const {
        switch (tag) {
            case PLUS_NODE:
            case MINUS_NODE: return nBits * 8;
            case TIMES_NODE: return nBits * 7;
            case SHIFTL_NODE:
            case SHIFTR_NODE: return nBits * 10;
            case BITAND_NODE:
            case BITOR_NODE:
            case BITXOR_NODE:
            case BITNOT_NODE: return nBits * 10;
            default: return 0;
        }
    }
};

struct NonTermLocation {
    RTLNode* parent;
    enum Slot {LEFT, RIGHT, CHILD} slot;
};

struct WorkItem {
    RTLNode* node;
    uint32_t depth;
    uint32_t totalCost;
    uint32_t combDelay;

    bool operator<(const WorkItem& other) const {
        return totalCost > other.totalCost;
    }
};

class Search {
public:
    RTLNode* topDown(const std::vector<std::vector<int>>& inputs, const std::vector<int>& outputs, const CostModel& costModel);

private:
    std::priority_queue<WorkItem> m_workList;

    RTLNode* clone(RTLNode* toClone);
    bool isComplete(RTLNode* node);
    std::optional<NonTermLocation> leftMostNonTerm(RTLNode* node);
    std::vector<NODETAG> productions(const NonTermLocation& location);
    WorkItem* replaceNonTerm(RTLNode* root, const NonTermLocation& location, NODETAG production);
    void unroll(WorkItem* workItem, const CostModel& costModel);
    void collectInputNodes(RTLNode* node, std::vector<InputNode*>& out);
};

#endif