#include "disjoint_set.h"

DisjointSet::DisjointSet(int n) {
    // allocate memory for parent and rank arrays
    parent.resize(n);
    rank.resize(n);

    // Set the initial values, each node is a distinct set and rank is 1
    for (int i = 0; i < n; i++) {
        parent[i] = i;
        rank[i] = 1;
    }
}

int DisjointSet::find(int x) {
    // Recursively find the root of the set containing x
    // Path compression is implemented to reduce the height of the tree
    return (parent[x] == x) ? x : parent[x] = find(parent[x]);
}

bool DisjointSet::is_in_same_set(int x, int y) {
    // x and y are in the same set if they have the same root
    return find(x) == find(y);
}

int DisjointSet::merge(int x, int y) {
    // Find the roots of the sets containing x and y
    int x_root = find(x);
    int y_root = find(y);

    // If x and y are already in the same set, we shall do nothing
    if (x_root == y_root) {
        return x_root;
    }

    // Merge the two sets by attaching the smaller ranked set to the larger ranked set
    if (rank[x_root] < rank[y_root]) {
        parent[x_root] = y_root;
        rank[y_root] += rank[x_root];
        return y_root;
    } else {
        parent[y_root] = x_root;
        rank[x_root] += rank[y_root];
        return x_root;
    }
}
