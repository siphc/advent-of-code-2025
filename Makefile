.PHONY = clean
CC = gcc
LDLIBS = -lcurl

input: input.o

clean:
	rm *.o input
