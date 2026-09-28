# Makefile for Blog Engine Importer Tool - WITH SOURCE SUBDIRECTORY
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -I./src -I/src/include -I../../shared -I../../shared/config -I../../shared/db -I../../shared/libs -I../../shared/logger -I../../shared/utils -I../../shared/utils/hashmap -I../../shared/utils/dynamic_array -I../../shared/utils/stack -I../../shared/repository/tag -I../../shared/repository/post -I../../shared/repository/user -I../../shared/repository/post_tag
LDFLAGS = -L../../shared/lib
LIBS = -lrepo -lmodel -lutils -llogger -lcore -ldb -lconfig -lpthread -ldl

# Source files
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:.c=.o)

# Target executable
TARGET = importer

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@
	@echo "Built importer tool: $@"

# Pattern rule for object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "Cleaned importer tool"
