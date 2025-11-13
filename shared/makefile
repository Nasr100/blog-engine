# Makefile for shared components - ORGANIZED FOLDERS
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -D_POSIX_C_SOURCE=200809L -I. -Iconfig -Idb -Ilibs -Ilogger -Iutils/hashmap -Iutils
AR = ar
ARFLAGS = rcs

# Directory structure
LIB_DIR = lib
OBJ_DIR = obj

# Library names (in lib folder)
CONFIG_LIB = $(LIB_DIR)/libconfig.a
DB_LIB = $(LIB_DIR)/libdb.a
CORE_LIB = $(LIB_DIR)/libcore.a
LOGGER_LIB = $(LIB_DIR)/liblogger.a
UTILS_LIB = $(LIB_DIR)/libutils.a

ALL_LIBS = $(CONFIG_LIB) $(DB_LIB) $(CORE_LIB) $(LOGGER_LIB) $(UTILS_LIB)

# Source files
CONFIG_SRCS = config/config.c
DB_SRCS = db/blog_db.c db/database.c
CORE_SRCS = libs/entity.c libs/md4c.c libs/md4c-html.c libs/sqlite3.c
LOGGER_SRCS = logger/logger.c
UTILS_SRCS = utils/hashmap/hashmap.c utils/file_handling.c utils/file_utils.c

# Test programs
CONFIG_TEST = config/test_config
HASHMAP_TEST = utils/hashmap/test_hashmap

# Object files (in obj folder)
CONFIG_OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(CONFIG_SRCS:.c=.o)))
DB_OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(DB_SRCS:.c=.o)))
CORE_OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(CORE_SRCS:.c=.o)))
LOGGER_OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(LOGGER_SRCS:.c=.o)))
UTILS_OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(UTILS_SRCS:.c=.o)))

ALL_OBJS = $(CONFIG_OBJS) $(DB_OBJS) $(CORE_OBJS) $(LOGGER_OBJS) $(UTILS_OBJS)

.PHONY: all clean test

all: $(LIB_DIR) $(OBJ_DIR) $(ALL_LIBS)

test: $(CONFIG_TEST) $(HASHMAP_TEST)
	@echo "Built test programs"

# Create directories
$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Test programs
$(CONFIG_TEST): config/test_config.c $(CONFIG_LIB)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -lconfig -o $@

$(HASHMAP_TEST): utils/hashmap/test_hashmap.c $(UTILS_LIB)
	$(CC) $(CFLAGS) $< -L$(LIB_DIR) -lutils -o $@

# Individual libraries
$(CONFIG_LIB): $(CONFIG_OBJS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Built config library: $@"

$(DB_LIB): $(DB_OBJS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Built database library: $@"

$(CORE_LIB): $(CORE_OBJS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Built core library: $@"

$(LOGGER_LIB): $(LOGGER_OBJS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Built logger library: $@"

$(UTILS_LIB): $(UTILS_OBJS)
	$(AR) $(ARFLAGS) $@ $^
	@echo "Built utils library: $@"

# Pattern rules for object files - ALL IN OBJ FOLDER
$(OBJ_DIR)/%.o: config/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: db/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: libs/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# SPECIAL RULE FOR SQLITE3 - with thread safety
$(OBJ_DIR)/sqlite3.o: libs/sqlite3.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -DSQLITE_THREADSAFE=1 -c $< -o $@

$(OBJ_DIR)/%.o: logger/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: utils/hashmap/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: utils/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(LIB_DIR) $(CONFIG_TEST) $(HASHMAP_TEST)
	@echo "Cleaned all objects and libraries"

# Dependencies
$(OBJ_DIR)/config.o: config/config.c config/config.h
$(OBJ_DIR)/blog_db.o: db/blog_db.c db/blog_db.h db/database.h
$(OBJ_DIR)/database.o: db/database.c db/database.h
$(OBJ_DIR)/entity.o: libs/entity.c libs/entity.h
$(OBJ_DIR)/md4c.o: libs/md4c.c libs/md4c.h
$(OBJ_DIR)/md4c-html.o: libs/md4c-html.c libs/md4c-html.h
$(OBJ_DIR)/sqlite3.o: libs/sqlite3.c libs/sqlite3.h
$(OBJ_DIR)/logger.o: logger/logger.c logger/logger.h
$(OBJ_DIR)/hashmap.o: utils/hashmap/hashmap.c utils/hashmap/hashmap.h
$(OBJ_DIR)/file_handling.o: utils/file_handling.c utils/file_handling.h
$(OBJ_DIR)/file_utils.o: utils/file_utils.c utils/file_utils.h