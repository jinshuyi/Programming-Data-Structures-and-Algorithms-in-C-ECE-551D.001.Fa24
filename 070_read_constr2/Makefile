# Makefile

# Compiler
CC = g++

# Output filename
OUTPUT = code

# Source and header files
SOURCE = code.cpp
HEADER = code.h

# Compiler flags
CFLAGS = -Wall

# Default rule
all: $(OUTPUT)

# Generate output file
$(OUTPUT): $(SOURCE) $(HEADER)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SOURCE)

# Clean up generated files
clean:
	rm -f $(OUTPUT)
