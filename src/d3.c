#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parse_input.h"

static inline void swap_chars(char *l, char *r) {
	char temp = *l;
	*l = *r;
	*r = temp;
}

int main() {
	Matrix *m = matrix_parse("inputs/3.input", ' ');

	int res_1 = 0;
	unsigned long long res_2 = 0;

	for (int i=0; i<matrix_rows(m); i++) {
		const char *curr = matrix_get(m, i, 0);

		// Part 1
		char first = curr[strlen(curr) - 2];
		char second = curr[strlen(curr) - 1];
		for (int j=strlen(curr)-3; j>=0; j--) {
			if (curr[j] >= first) {
				if (first > second)
					second = first;
				first = curr[j];
			}
		}
		res_1 += ((int)first - 48) * 10;
		res_1 += ((int)second - 48);

		// Part 2
		// This doesn't happen, but we want to have safe code.
		if (strlen(curr) < 12)
			return 1;
		
		char joltage[13];
		memcpy(joltage, curr + strlen(curr) - 12, 13);
		for (int j=strlen(curr)-13; j>=0; j--) {
			char spare = curr[j];
			for (int k=0; k<12; k++) {
				if (spare >= joltage[k])
					swap_chars(&spare, &joltage[k]);
				else
					break;
			}
		}
		res_2 += strtoull(joltage, NULL, 0);
	}

	printf("Part 1: %d\n", res_1);
	printf("Part 2: %llu\n", res_2);

	matrix_free(m);
	return 0;
}