# Makefile for Blog Engine - UPDATED FOR ORGANIZED SHARED LIBS
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -I. -Iinclude -I../shared -I../shared/config -I../shared/db -I../shared/libs -I../shared/logger -I../shared/utils -I../shared/utils/hashmap -I../shared/utils/stack -I../shared/utils/dynamic_array -I../shared/repository -I../shared/repository/post -I../shared/repository/tag -I../shared/repository/user -I../shared/repository/post_tag -Idto -Iservice/post -Iservice/tag -Iservice/user
LDFLAGS = -L../shared/lib
LIBS = -lmodel -lrepo -lutils -llogger -lcore -ldb -lconfig -lpthread -ldl

# Source directories
SRC_DIRS = context controller http json_serialization md_parser router server service/tag service/post service/user  static
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

$(TARGET): $(ALL_OBJS)
	$(CC) $(ALL_OBJS) $(LDFLAGS) $(LIBS) -o $@
	@echo "Built blog engine: $@"

# Compile source files
$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Create object directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)


debug-sources:
	@echo "SRC_DIRS: $(SRC_DIRS)"
	@echo "Found .c files:"
	@find $(SRC_DIRS) -name '*.c' 2>/dev/null || echo "No files found"
	@echo "ALL_SRCS: $(ALL_SRCS)"
# Clean
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
	@echo "Cleaned blog engine"

# Optional: Auto-build shared libs if missing
shared-libs:
	$(MAKE) -C ../shared all

# Full build including shared libs
full: shared-libs $(TARGET)