CC := gcc

OFLAGS := -O3 -Wall -Wextra
CFLAGS := -O3 -Wall -Wextra

EXEC := QuickRest_x86_64
BUILD_DIR := out
SRC_DIR := src/code
INC_DIR := src/header
LINKER := m

DEBUG_FLAGS = -DDEBUG -g

all: remove_executables $(EXEC)

remove_executables:
	@rm -f $(EXEC)

set_debug:
	$(eval CFLAGS += $(DEBUG_FLAGS))

debug: set_debug all

rebuild: clean all

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:$(SRC_DIR)/%=$(BUILD_DIR)/%.o)

INC_FLAGS := $(addprefix -I,$(INC_DIR))
LINKER_FLAGS := $(addprefix -l,$(LINKER))

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LINKER_FLAGS) -o $@

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	$(eval DIR_NAME := $(shell dirname ./$@))
	@mkdir -p $(DIR_NAME)
	$(CC) $(OFLAGS) $(DEBUG_FLAGS) $(INC_FLAGS) -c $< -o $@ $(LINKER_FLAGS)

.PHONY: clean debug rebuild

clean:
	rm -f -r $(BUILD_DIR)/*
	rm -f $(EXEC)