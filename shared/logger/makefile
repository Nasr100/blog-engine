CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I.
TARGET = test_logger

SRC = logger.c
OBJ = $(SRC:.c=.o)
LIB = liblogger.a

all: $(TARGET)

$(LIB): $(OBJ)
	ar rcs $@ $^

$(TARGET): test_logger.c $(LIB)
	$(CC) $(CFLAGS) -o $@ test_logger.c -L. -llogger

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(LIB) $(TARGET)

test: $(TARGET)
	./$(TARGET)

.PHONY: all clean test