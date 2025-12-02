.PHONY = clean
CC = gcc
LDLIBS = -lcurl

get_input: get_input.o

clean:
	rm -v *.o get_input