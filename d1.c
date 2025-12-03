#include <stdio.h>
#include <stdlib.h>

#include "parse_input.h"

int main() {
	Matrix *m = matrix_parse("1.input", ' ');

	int curr = 50;
	int res_1 = 0;
	int res_2 = 0;

	for (int i=0; i<matrix_rows(m); i++) {
		const char *rotation = matrix_get(m, i, 0);
		int clicks = atoi(rotation+1);

		if (rotation[0] == 'L') {
			res_2 += clicks>=curr%100 ? (clicks-curr%100)/100+1 : 0;
			if (!(curr % 100)) res_2--;
			curr -= clicks;
		}
		else {
			res_2 += clicks+curr%100>=100 ? (clicks+curr%100)/100 : 0;
			curr += clicks;
		}

		if (!(curr % 100))
			res_1++;	
	}

	printf("Part 1: %d\n", res_1);
	printf("Part 2: %d\n", res_2);

	matrix_free(m);

	return 0;
}
