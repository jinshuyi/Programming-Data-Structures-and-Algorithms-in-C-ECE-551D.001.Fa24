# Makefile for test.c

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source file and executable name
SOURCE = test.c
EXECUTABLE = test

# Default target
all: $(EXECUTABLE)

# Build the executable
$(EXECUTABLE): $(SOURCE)
	$(CC) $(CFLAGS) -o $@ $<

# Clean up generated files
clean:
	rm -f $(EXECUTABLE)
