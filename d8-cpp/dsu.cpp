#include <utility>
#include <numeric>

#include "dsu.hpp"

int capacity;
std::vector<int> parent;
std::vector<int> size;

void init_dsu(int cap) {
    capacity = cap;
    parent.resize(cap);
    std::iota(parent.begin(), parent.end(), 0);
    size = std::vector<int>(cap, 1);
}

int find_set(int v) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent[v]);
}

int union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (size[a] < size[b])
            std::swap(a, b);
        parent[b] = a;
        size[a] += size[b];
    }

    if (size[a] == capacity) {
        return 1;
    }

    return 0;
}
