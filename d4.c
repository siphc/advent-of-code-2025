#include <stdio.h>
#include <string.h>

#include "parse_input.h"

int can_access_paper(Matrix *m, int row, int col, int width) {
	int cnt = 0;

	for (int i=row-1; i<=row+1; i++) {
		for (int j=col-1; j<=col+1; j++) {
			if ((i==row && j==col) || i<0 || j<0 || i==matrix_rows(m) || \
				j==width)
					continue;
			
			if (matrix_get(m, i, 0)[j] == '@')
				cnt++;
		}
	}
	
	return cnt<4 ? 1 : 0;
}

int main() {
	Matrix *m = matrix_parse("4.input", ' ');

	int res_1 = 0;
	int res_2 = 0;
	const int width = strlen(matrix_get(m, 0, 0));
	const int height = matrix_rows(m);

	for (int i=0; i<height; i++) {
		for (int j=0; j<width; j++) {
			if (matrix_get(m, i, 0)[j] == '@')
				res_1 += can_access_paper(m, i, j, width);
		}
	}

	printf("Part 1: %d\n", res_1);

	/* This brute force algorithm runs surprisingly fast.
		On average (of 5 on my machine, GCC) it takes:
			0.110s to execute with no optimizations;
			0.095s with `width` and `height` preload;
			0.090s by unrolling *curr;
			0.084s by swapping the condition (i==row && j==col) first;
			0.037s by passing in `width` (!);
			0.015s by using -O3.
		That's a 7.3X speedup!

		If you're reading this, please tell me where I can optimize further.
		If I'm bored, I will make some kind of queue system to dynamically
		update after removing a roll... But it is 3am.
	*/
	int old_res_2;
	do {
		old_res_2 = res_2;
		for (int i=0; i<height; i++) {
			char *curr = matrix_get(m, i, 0);
			for (int j=0; j<width; j++) {
				if (curr[j] == '@' && can_access_paper(m, i, j, width)) {
					res_2++;
					curr[j] = '.';
				}
			}
		}
	} while (res_2 != old_res_2);

	printf("Part 2: %d\n", res_2);

	matrix_free(m);
	return 0;
}