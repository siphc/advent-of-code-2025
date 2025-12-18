#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parse_input.h"

typedef struct {
	unsigned long long begin;
	unsigned long long end;
} Range;

int main() {
	Matrix *m = matrix_parse("inputs/5.input", ' ');

	int i = 0;
	while (strstr(matrix_get(m, i, 0), "-"))
		i++;
	
	int res_1 = 0;
	unsigned long long res_2 = 0;

	int range_end = i;
	i++;
	for (; i<matrix_rows(m); i++) {
		unsigned long long curr = strtoull(matrix_get(m, i, 0), NULL, 10);
		for (int j=0; j<range_end; j++) {
			if (curr >= strtoull(matrix_get(m, j, 0), NULL, 10) &&
			curr <= strtoull(strstr(matrix_get(m, j, 0), "-") + 1, NULL, 10)) {
				res_1++;
				break;
			}
		}
	}

	int size = 0;
	Range *ranges = (Range *)calloc(range_end, sizeof(Range));
	for (i=0; i<range_end; i++) {
		Range curr = {strtoull(matrix_get(m, i, 0), NULL, 10), \
		strtoull(strstr(matrix_get(m, i, 0), "-") + 1, NULL, 10)};

		for (int j=0; j<size; j++) {
			if (!ranges[j].begin && !ranges[j].begin)
				continue;
			
			// Entirely contained
			if (curr.begin >= ranges[j].begin && curr.end <= ranges[j].end) {
				curr.begin = 0ull;
				curr.end = 0ull;
				break;
			}
			
			// The self-imposed 80col limit is really annoying here.
			if ((curr.begin<=ranges[j].begin && curr.end>=ranges[j].begin) ||
			(curr.begin <= ranges[j].end && curr.end >= ranges[j].end)) {
				curr.begin = curr.begin<=ranges[j].begin ?
				curr.begin : ranges[j].begin;
				curr.end = curr.end>=ranges[j].end ? curr.end : ranges[j].end;

				ranges[j].begin = 0ull;
				ranges[j].end = 0ull;

				j = -1;
			}
		}
		
		ranges[size] = curr;
		size++;
	}
	for (i=0; i<range_end; i++) {
		if (!ranges[i].begin && !ranges[i].begin)
			continue;
		res_2 += ranges[i].end - ranges[i].begin + 1;
	}

	printf("Part 1: %d\n", res_1);
	printf("Part 2: %llu\n", res_2);

	matrix_free(m);
	free(ranges);
	return 0;
}
