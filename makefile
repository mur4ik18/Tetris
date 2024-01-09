# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

SRC_DIR = .
OBJ_DIR = obj

# List all your source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Derive object file names from source file names
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Target executable
TARGET = tetris

# Main target
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Create the obj directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

.PHONY: clean

# Clean up object files and the executable
clean:
	rm -rf $(OBJ_DIR) $(TARGET)
