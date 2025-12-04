.PHONY = clean
CC = gcc
CFLAGS = -fsanitize=address -fsanitize=undefined
LDFLAGS = -fsanitize=address -fsanitize=undefined

get_input: get_input.o
	$(CC) $(LDFLAGS) $^ -lcurl -o $@

d1: d1.o parse_input.o

d2: d2.o parse_input.o
	$(CC) -fopenmp $(LDFLAGS) $^ $(LDLIBS) -o $@

d2.o: d2.c
	$(CC) -c $(CPPFLAGS) -fopenmp $(CFLAGS) $^ -o $@

d3: d3.o parse_input.o

d4: d4.o parse_input.o

clean:
	rm -v *.o get_input d?