#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../include/parse_input.h"
#include "../include/priority_queue.h"
#include "../include/dsu.h"

typedef struct {
	double m_dist;
	int m_first;
	int m_second;
} Pair;

int ge(const void *lhs, const void *rhs) {
	const Pair *l = (const Pair *)lhs;
	const Pair *r = (const Pair *)rhs;
	return l->m_dist >= r->m_dist;
}

int le(const void *lhs, const void *rhs) {
	const Pair *l = (const Pair *)lhs;
	const Pair *r = (const Pair *)rhs;
	return l->m_dist <= r->m_dist;
}

int comp(const void *lhs, const void *rhs) {
	return *(int *)rhs - *(int *)lhs;
}

double getdist(const Matrix *m, size_t first, size_t second) {
	double x = (double)(strtol(matrix_get(m, first, 0), NULL, 10) - \
		strtol(matrix_get(m, second, 0), NULL, 10));
	double y = (double)(strtol(matrix_get(m, first, 1), NULL, 10) - \
		strtol(matrix_get(m, second, 1), NULL, 10));
	double z = (double)(strtol(matrix_get(m, first, 2), NULL, 10) - \
		strtol(matrix_get(m, second, 2), NULL, 10));

	return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

int main() {
	Matrix *m = matrix_parse("inputs/8.input", ',');

	const int MAX_NUM_CONNECTIONS = 1000;
	Pair * const max_heap = (Pair * const)malloc( \
		MAX_NUM_CONNECTIONS * sizeof(Pair));
	Pair * const min_heap = (Pair * const)malloc( \
		matrix_rows(m) * (matrix_rows(m)-1) * sizeof(Pair));
	int num_pairs_maxh = 0;
	int num_pairs_minh = 0;
	for (int i=0; i<matrix_rows(m); i++) {
		for (int j=i+1; j<matrix_rows(m); j++) {
			Pair toadd = {getdist(m, i, j), i, j};

			heap_add(min_heap, num_pairs_minh, sizeof(Pair), &toadd, le);
			num_pairs_minh++;

			if (num_pairs_maxh && ge(&toadd, max_heap))
				continue;

			if (MAX_NUM_CONNECTIONS == num_pairs_maxh) {
				heap_pop(max_heap, num_pairs_maxh, sizeof(Pair), ge);
				heap_add(max_heap, num_pairs_maxh-1, sizeof(Pair), &toadd, ge);
			}
			else {
				heap_add(max_heap, num_pairs_maxh, sizeof(Pair), &toadd, ge);
				num_pairs_maxh++;
			}
		}
	}

	// Tally up results for Part 1
	int *groups = calloc(matrix_rows(m), sizeof(int));
	int num_groups = 0;
	extern int *parent;
	extern int *size;
	init_dsu(matrix_rows(m));
	for (int i=0; i<num_pairs_maxh; i++) {
		union_sets(max_heap[i].m_first, max_heap[i].m_second);
	}
	for (int i=0; i<matrix_rows(m); i++) {
		if (parent[i] == i) {
			groups[num_groups] = size[i];
			num_groups++;
		}
	}
	free_dsu();

	qsort(groups, num_groups, sizeof(int), comp);
	int res_1 = groups[0] * groups[1] * groups[2];

	free(groups);
	
	printf("Part 1: %d\n", res_1);

	// And now for Part 2
	long long res_2 = 0;
	init_dsu(matrix_rows(m));
	while (num_pairs_minh) {
		if (union_sets(min_heap[0].m_first, min_heap[0].m_second)) {
			res_2 = strtoll(matrix_get(m, min_heap[0].m_first, 0), NULL, 10) * \
				strtoll(matrix_get(m, min_heap[0].m_second, 0), NULL, 10);
			break;
		}
		heap_pop(min_heap, num_pairs_minh, sizeof(Pair), le);
		num_pairs_minh--;
	}
	free_dsu();

	printf("Part 2: %lld\n", res_2);

	free(max_heap);
	free(min_heap);
	matrix_free(m);
	return 0;
}