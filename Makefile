.PHONY = clean all

CC := gcc
CXX := g++
CFLAGS := -Wall -Werror -fsanitize=address -fsanitize=undefined
LDFLAGS := -fsanitize=address -fsanitize=undefined

BUILD = build
REGULARS := d1 d3 d4 d5 d6 d7

all: $(REGULARS) d2 d8

# Special rule for get_input due to libcurl
get_input: $(BUILD)/get_input.o
	nix-shell --run '$(CC) $(LDFLAGS) $^ -lcurl -o $@'

$(BUILD)/get_input.o: lib/get_input.c
	nix-shell --run '$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@'

# Regular binaries.
$(REGULARS): %: $(BUILD)/%.o $(BUILD)/parse_input.o
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD)/%.o: src/%.c | $(BUILD)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@

# Binaries with special dynamically linked libraries and/or compiler flags
d2: $(BUILD)/d2.o $(BUILD)/parse_input.o
	$(CC) -fopenmp $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD)/d2.o: src/d2.c | $(BUILD)
	$(CC) -c $(CPPFLAGS) -fopenmp $(CFLAGS) $^ -o $@

d8: $(BUILD)/d8.o $(BUILD)/parse_input.o $(BUILD)/priority_queue.o $(BUILD)/dsu.o
	$(CC) $(LDFLAGS) $^ -lm -o $@

# Utility libraries
LIBS := dsu parse_input priority_queue
LIB_OBJS := $(patsubst %,$(BUILD)/%.o,$(LIBS))

$(LIB_OBJS): $(BUILD)/%.o: lib/%.c | $(BUILD)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@

$(BUILD):
	mkdir -p $@

clean:
	rm -v get_input build/* d?