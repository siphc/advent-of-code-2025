#include <stdio.h>

#include "parse_input.h"

int main() {
	Matrix *m = matrix_parse("x.input", ' ');

	int res_1 = 0;
	int res_2 = 0;

	printf("Part 1: %d\n", res_1);
	printf("Part 2: %d\n", res_2);

	matrix_free(m);
	return 0;
}