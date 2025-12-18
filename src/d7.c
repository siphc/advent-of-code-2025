#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parse_input.h"

#define ull unsigned long long

int main() {
	Matrix *m = matrix_parse("inputs/7.input", ' ');
	const int COLS = strlen(matrix_get(m, 0, 0));

	// The problem spec does not mention the possibly of two splitters being
	// adjacent to each other; assuming undefined behavior.

	// init
	ull *beams = malloc(COLS * sizeof(ull) * 2);
	int size = 1;
	for (int j=0; j<COLS; j++) {
		if (matrix_get(m, 0, 0)[j] == 'S') {
			beams[0] = j;
			beams[COLS + 0] = 1;
		}
	}

	int res_1 = 0;

	for (int i=1; i<matrix_rows(m); i++) {
		ull *new_beams = malloc(COLS * sizeof(ull) * 2);
		int new_size = 0;
		for (int j=0; j<size; j++) {
			// Very spaghetti, I know...
			if (matrix_get(m, i, 0)[beams[j]] == '.') {
				if (!new_size || new_beams[new_size-1] != beams[j]) {
					new_beams[new_size] = beams[j];
					new_beams[COLS + new_size] = beams[COLS + j];
					new_size++;
				}
				else {
					new_beams[COLS + new_size-1] += beams[COLS + j];
				}
			}
			else {
				if (beams[j] != 0) {
					if (!new_size || new_beams[new_size-1] != beams[j]-1) {
						new_beams[new_size] = beams[j]-1;
						new_beams[COLS + new_size] = beams[COLS + j];
						new_size++;
					}
					else {
						new_beams[COLS + new_size-1] += beams[COLS + j];
					}
				}
				if (beams[j] != COLS-1) {
					new_beams[new_size] = beams[j]+1;
					new_beams[COLS + new_size] = beams[COLS + j];
					new_size++;
				}
				res_1++;
			}
		}
		
		memcpy(beams, new_beams, COLS * sizeof(ull) * 2);
		size = new_size;
		free(new_beams);
	}

	printf("Part 1: %d\n", res_1);

	ull res_2 = 0;
	for (int i=0; i<size; i++) {
		res_2 += beams[COLS + i];
	}
	printf("Part 2: %llu\n", res_2);

	matrix_free(m);
	free(beams);
	return 0;
}