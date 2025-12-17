#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "parse_input.h"
#include "priority_queue.h"
#include "dsu.h"

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
	Matrix *m = matrix_parse("8.input", ',');

	int res_2 = 0;

	const int MAX_NUM_CONNECTIONS = 1000;
	Pair * const pq = malloc(MAX_NUM_CONNECTIONS * sizeof(Pair));
	int num_pairs = 0;
	for (int i=0; i<matrix_rows(m); i++) {
		for (int j=i+1; j<matrix_rows(m); j++) {
			Pair toadd = {getdist(m, i, j), i, j};
			if (num_pairs && ge(&toadd, pq))
				continue;

			if (MAX_NUM_CONNECTIONS == num_pairs) {
				heap_pop(pq, num_pairs, sizeof(Pair), ge);
				heap_add(pq, num_pairs-1, sizeof(Pair), &toadd, ge);
			}
			else {
				heap_add(pq, num_pairs, sizeof(Pair), &toadd, ge);
				num_pairs++;
			}
		}
	}

	int *groups = calloc(matrix_rows(m), sizeof(int));
	int num_groups = 0;
	extern int *parent;
	extern int *size;
	init_dsu(matrix_rows(m));
	for (int i=0; i<num_pairs; i++) {
		union_sets(pq[i].m_first, pq[i].m_second);
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
	
	printf("Part 1: %d\n", res_1);
	printf("Part 2: %d\n", res_2);

	free(pq);
	free(groups);
	matrix_free(m);
	return 0;
}