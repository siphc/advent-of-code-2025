.PHONY = clean
CC = gcc

get_input: get_input.o
	$(CC) $(LDFLAGS) $^ -lcurl -o $@

d1: d1.o parse_input.o
	$(CC) $(LDFLAGS) $^ -lm -o $@

clean:
	rm -v *.o get_input d?