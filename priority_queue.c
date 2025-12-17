#include <string.h>
#include <stdlib.h>

#include "priority_queue.h"

/* Helper Functions */
int memswap(void *lhs, void *rhs, size_t size) {
	void *temp = malloc(size);
	if (!temp)
		return 1;
	
	memcpy(temp, lhs, size);
	memcpy(lhs, rhs, size);
	memcpy(rhs, temp, size);

	free(temp);
	return 0;
}

/*	Public API
	int (*comp) is expected to act like either:
		<= or < (min heap), or
		>= or > (max heap).
	If you use <= or >=, it's a tad bit more efficient.
*/
int heap_pop(void *ptr, size_t count, size_t size, \
				int (*comp)(const void *, const void *)) {
	if (count <= 0 || size <= 0 || !ptr || !comp)
		return 1;

	if (count == 1) {
		memset(ptr, 0, size);
		return 0;
	}

	count--;
	memcpy(ptr, ptr+count*size, size);

	int idx = 0;
	while (1) {
		if (2 * idx + 1 >= count)
			break;
		
		void *curr = ptr + idx * size;
		void *left = ptr + (2 * idx + 1) * size;
		void *right = ptr + (2 * idx + 2) * size;
		if (comp(curr, left) && comp(curr, right))
			break;
		
		if (comp(left, right)) {
			memswap(curr, left, size);
			idx = 2 * idx + 1;
		}
		else {
			memswap(curr, right, size);
			idx = 2 * idx + 2;
		}
	}
	
	return 0;
}

int heap_add(void *ptr, size_t count, size_t size, void *toadd, \
				int (*comp)(const void *, const void *)) {
	if (count < 0 || size <=0 || !ptr || !toadd || !comp)
		return 1;
	
	memcpy(ptr+count*size, toadd, size);
	count++;

	int idx = count-1;
	while (1) {
		if (!idx)
			break;
		
		void *curr = ptr + idx * size;
		void *parent = ptr + (idx-1)/2 * size;
		if (comp(curr, parent))
			memswap(curr, parent, size);
		else
			break;
	}

	return 0;
}

int heap_clear(void *ptr, size_t count, size_t size) {
	if (count < 0 || size <= 0 || !ptr)
		return 1;
	
	memset(ptr, 0, count * size);
	return 0;
}