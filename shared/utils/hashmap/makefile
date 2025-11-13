CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
TARGET = test_hashmap

SRC = hashmap.c
OBJ = $(SRC:.c=.o)
LIB = libhashmap.a

# Default target - build just the library
all: $(LIB)

# Build the library only
$(LIB): $(OBJ)
	ar rcs $@ $^

# Build the test executable (only for testing)
$(TARGET): test_hashmap.c $(LIB)
	$(CC) $(CFLAGS) -o $@ test_hashmap.c -L. -lhashmap

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(LIB) $(TARGET)

# Run the test (builds test program + runs it)
test: $(TARGET)
	./$(TARGET)

.PHONY: all clean test