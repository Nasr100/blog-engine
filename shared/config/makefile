CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
TARGET = test_config

SRC = config.c
OBJ = $(SRC:.c=.o)
LIB = libconfig.a

# Build everything including the test
all: $(TARGET)

# Build the library
$(LIB): $(OBJ)
	ar rcs $@ $^

# Build the test executable
$(TARGET): test_config.c $(LIB)
	$(CC) $(CFLAGS) -o $@ test_config.c -L. -lconfig

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJ) $(LIB) $(TARGET)

# Run the test
test: $(TARGET)
	./$(TARGET)

.PHONY: all clean test