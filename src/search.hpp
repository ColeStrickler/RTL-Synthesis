#ifndef SEARCH_HPP
#define SERACH_HPP

#include <queue>
#include <stdint.h>
#include <vector>

#include "node.hpp"

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
    RTLNode* leftMostNonTerm(RTLNode* node);
    void unroll(WorkItem* workItem);
};

#endif