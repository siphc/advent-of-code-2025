#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "../include/parse_input.h"

int main() {
	Matrix *m = matrix_parse("inputs/6.input", ' ');
	// Quirk in the parser means last column should be ignored
	int cols = matrix_cols(m) - 1;

	unsigned long long res_1 = 0;
	// I keep forgetting about the damn null character!
	char *binops = (char *)calloc(cols + 1, sizeof(char));
	int idx = cols - 1; // Not used until part 2

	for (int j=0; j<cols; j++) {
		// Pre binop parsing for part 2
		char binop = *matrix_get(m, matrix_rows(m)-1, j);
		binops[j] = binop;
		unsigned long long curr;

		// Putting the conditional here saves one branching instruction.
		if (binop == '+') {
			curr = 0;
			for (int i=0; i<matrix_rows(m)-1; i++) {
				curr += strtoull(matrix_get(m, i, j), NULL, 10);
			}
		}
		else {
			curr = 1;
			for (int i=0; i<matrix_rows(m)-1; i++) {
				curr *= strtoull(matrix_get(m, i, j), NULL, 10);
			}
		}

		res_1 += curr;
	}

	printf("Part 1: %llu\n", res_1);

	matrix_free(m);
	m = matrix_parse("inputs/6.input", ',');

	unsigned long long res_2 = 0;
	unsigned long long curr = binops[idx] == '+' ? 0 : 1;
	cols = strlen(matrix_get(m, 0, 0));

	for (int j=cols-1; j>=0; j--) {
		// Parse current row
		int curr_col = 0;
		bool is_empty_col = true;
		for (int i=0; i<matrix_rows(m)-1; i++) {
			if (isdigit(matrix_get(m, i, 0)[j])) {
				is_empty_col = false;
				curr_col *= 10;
				curr_col += matrix_get(m, i, 0)[j] - 48;
			}
		}
		// This literally never happens in my input,
		// but it might happen in yours. Who knows?
		if (is_empty_col)
			continue;

		if (binops[idx] == '+')
			curr += curr_col;
		else
			curr *= curr_col;
						
		// Cleanup
		if (matrix_get(m, matrix_rows(m)-1, 0)[j] == binops[idx]) {
			idx--;
			res_2 += curr;
			if (idx>=0)
				curr = binops[idx] == '+' ? 0 : 1;
		}
	}

	printf("Part 2: %llu\n", res_2);

	free(binops);
	matrix_free(m);
	return 0;
}