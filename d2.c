#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#include "parse_input.h"

int main() {
	Matrix *m = matrix_parse("2.input", ',');

	unsigned long long res_1 = 0;
	unsigned long long res_2 = 0;

	for (int i=0; i<matrix_cols(m); i++) {
		// find lower and upper bound
		const char *curr = matrix_get(m, 0, i);
		int dash = -1;
		for (int j=0; j<strlen(curr); j++) {
			if (curr[j] == '-') {
				dash = j;
				break;
			}
		}

		/* This code is horrible!
			Average >6s on my standard setup (GCC 14, asan, ubsan).
			With OpenMP, it's now 1.6s :)
		*/
		#define NUM_THREADS 16
		#pragma omp parallel for reduction (+:res_1,res_2)
		for (unsigned long long j=atoll(curr); j<=atoll(curr+dash+1); j++) {
			char buffer[21]; // long enough for all llu values
			sprintf(buffer, "%llu", j); // therefore, this is safe!

			for (int k=1; k<=strlen(buffer)/2; k++) {
				if (strlen(buffer)%k)
					continue;
								
				char **candidates = (char **)calloc(strlen(buffer)/k,
													sizeof(char *));
				for (int l=0; l<strlen(buffer)/k; l++) {
					candidates[l] = malloc((k+1) * sizeof(char));
					memcpy(candidates[l], buffer+k*l, k);
					candidates[l][k] = '\0';

					// If this check fails, the current j doesn't work with
					// the current k-partition; we continue onto the next k.
					if (strcmp(candidates[0], candidates[l]))
						goto cleanup;
				}

				// However, if we decide that the current j with the current
				// k-partition is good enough, we don't want to double-count j.
				if (!(strlen(buffer)/k % 2))
					res_1 += j;
				res_2 += j;

				for (int l=0; l<strlen(buffer)/k; l++)
					free(candidates[l]);
				free(candidates);
				break;
				
cleanup:
				for (int l=0; l<strlen(buffer)/k; l++)
					free(candidates[l]);
				free(candidates);
			}
		}
	}

	printf("Part 1: %llu\n", res_1);
	printf("Part 2: %llu\n", res_2);

	matrix_free(m);

	return 0;
}