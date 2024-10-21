# Makefile

# Compiler
CC = g++

# Output filename
OUTPUT = code

# Source file
SOURCE = code.cpp

# Compiler flags
CFLAGS = -Wall

# Default rule
all: $(OUTPUT)

# Generate output file
$(OUTPUT): $(SOURCE)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCE)

# Clean up generated files
clean:
	rm -f $(OUTPUT)
