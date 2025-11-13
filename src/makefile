# Makefile for Blog Engine - UPDATED FOR ORGANIZED SHARED LIBS
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -I. -Iinclude -I../shared -I../shared/config -I../shared/db -I../shared/libs -I../shared/logger -I../shared/utils -I../shared/utils/hashmap
LDFLAGS = -L../shared/lib
LIBS = -lutils -llogger -lcore -ldb -lconfig -lsqlite3 -lpthread -ldl

# Source directories
SRC_DIRS = blog context http network parser router static
ALL_SRCS = $(shell find $(SRC_DIRS) -name '*.c' 2>/dev/null || true)
MAIN_SRC = main.c

# Object files directory
OBJ_DIR = obj

# Object files
MAIN_OBJ = $(OBJ_DIR)/main.o
SRC_OBJS = $(patsubst %.c,$(OBJ_DIR)/%.o,$(ALL_SRCS))
ALL_OBJS = $(MAIN_OBJ) $(SRC_OBJS)

# Target executable
TARGET = blog_engine

.PHONY: all clean

all: $(TARGET)

$(TARGET): ../shared/lib/$(ALL_LIBS) $(ALL_OBJS)
	$(CC) $(ALL_OBJS) $(LDFLAGS) $(LIBS) -o $@
	@echo "Built blog engine: $@"

# Ensure shared libraries are built
../shared/lib/$(ALL_LIBS):
	@echo "Building shared libraries first..."
	$(MAKE) -C ../shared all

# Compile source files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned blog engine"