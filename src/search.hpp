#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <optional>
#include <queue>
#include <stdint.h>
#include <vector>

#include "node.hpp"

struct NonTermLocation {
    RTLNode* parent;
    enum Slot {LEFT, RIGHT, CHILD} slot;
};

struct WorkItem {
    RTLNode* node;
    uint32_t depth;
    uint32_t cost;

    bool operator<(const WorkItem& other) const {
        return cost < other.cost;
    }
};

class Search {
public:
    RTLNode* search(const std::vector<std::vector<int>>& inputs, const std::vector<std::vector<int>>& outputs);

private:
    std::priority_queue<WorkItem> m_workList;

    RTLNode* clone(RTLNode* toClone);
    bool isComplete(RTLNode* node);
    std::optional<NonTermLocation> leftMostNonTerm(RTLNode* node);
    void unroll(WorkItem* workItem);
};

#endif