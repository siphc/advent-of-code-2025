#ifndef DSU_HPP
#define DSU_HPP

#include <vector>

extern int capacity;
extern std::vector<int> parent;
extern std::vector<int> size;

void init_dsu(int cap);
void make_set(int v);
int find_set(int v);
int union_sets(int a, int b);

#endif