TARGET_EXEC := Quick-Rest
CC := gcc

CFLAGS := -O3
LDFLAGS :=

BUILD_DIR := out
SRC_DIRS := src/code

rebuild:
	rm -f $(TARGET_EXEC)

all: $(TARGET_EXEC)

debug: CFLAGS += -DDEBUG -g
debug: $(TARGET_EXEC)

SRCS := $(shell find $(SRC_DIRS) -name '*.c')

OBJS := $(SRCS:$(SRC_DIRS)/%=$(BUILD_DIR)/%.o)

INC_DIRS=src/header
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The final build step.
$(TARGET_EXEC): rebuild $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -O3 -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: $(SRC_DIRS)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(INC_FLAGS) -c $< -o $@

.PHONY: clean debug

clean:
	rm -f -r $(BUILD_DIR)/*
	rm -f $(TARGET_EXEC)