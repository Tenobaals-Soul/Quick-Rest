TARGET_EXEC := Quick-Rest
CC := gcc

CFLAGS := -O3
LDFLAGS :=

BUILD_DIR := out
SRC_DIRS := src/code

all: $(TARGET_EXEC)

debug: CFLAGS = -DDEBUG -g
debug: $(TARGET_EXEC)

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. Make will incorrectly expand these otherwise.
SRCS := $(shell find $(SRC_DIRS) -name '*.c')

# String substitution for every C file.
# As an example, hello.c turns into ./build/hello.c.o
OBJS := $(SRCS:$(SRC_DIRS)/%=$(BUILD_DIR)/%.o)

INC_DIRS=src/header
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The final build step.
$(TARGET_EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -O3 -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: $(SRC_DIRS)/%.c
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

.PHONY: clean debug

clean:
	rm -f -r $(BUILD_DIR)/*
	rm -f $(TARGET_EXEC)