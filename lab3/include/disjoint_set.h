#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H

#include <vector>

// The implementation of a disjoint set data structure
// The disjoint set is used for castle maze generation
class DisjointSet {
private:
    std::vector<int> parent;    // parent[i] represents the parent node of i
    std::vector<int> rank;      // rank[i] represents the size of the subtree rooted at i

public:
    // Constructor, creates a disjoint set with n nodes.
    DisjointSet(int n);

    // @brief Find the root of the set containing x.
    int find(int x);

    // @brief Check if two nodes are in the same set.
    bool is_in_same_set(int x, int y);

    // @brief Merge two sets containing x and y.
    int merge(int x, int y);
};

#endif
