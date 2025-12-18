#include <stdlib.h>

#include "../include/dsu.h"

int capacity;
int *parent;
int *size;

void init_dsu(int cap) {
    capacity = cap;
    parent = (int *)malloc(cap * sizeof(int));
	size = (int *)malloc(cap * sizeof(int));
	for (int i=0; i<cap; i++) {
		parent[i] = i;
		size[i] = 1;
	}
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
        if (size[a] < size[b]) {
			int temp = a;
			a = b;
			b = temp;
		}
        parent[b] = a;
        size[a] += size[b];
    }

	// Not standard DSU implementation; written for day 8 part 2.
    if (size[a] == capacity) {
        return 1;
    }

    return 0;
}

void free_dsu() {
	free(parent);
	free(size);
}