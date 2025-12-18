#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

int heap_pop(void *ptr, size_t count, size_t size, \
				int (*comp)(const void *, const void *));
int heap_add(void *ptr, size_t count, size_t size, void *toadd, \
				int (*comp)(const void *, const void *));
int heap_clear(void *ptr, size_t count, size_t size);

#endif /* PRIORITY_QUEUE_H */