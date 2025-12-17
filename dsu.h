#ifndef DSU_H
#define DSU_H

void init_dsu(int cap);
int find_set(int v);
int union_sets(int a, int b);
void free_dsu();

#endif /* DSU_H */