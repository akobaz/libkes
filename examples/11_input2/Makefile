# =============================================================================
# Makefile for example program
# after CMake Makefile structure
# =============================================================================

# Set environment variables for the build
CC = gcc
CFLAGS = -Wall -g -std=c99
INC = -I ../../src/
LIB = -L ../../lib/Release/
LINK = -lm -lkes

# basename for target program file
SRCFILE = input2

# =============================================================================

# Default target executed when no arguments are given to make
default_target: all
.PHONY : default_target

# =============================================================================

# The main all target
all: $(SRCFILE)
.PHONY : all

# =============================================================================

# The main clean target
clean:
	rm -f $(SRCFILE).o $(SRCFILE)
.PHONY : clean

# =============================================================================

# Build rule for target
$(SRCFILE): $(SRCFILE).c
	$(CC) $(CFLAGS) $(INC) $(LIB) $(SRCFILE).c -o $(SRCFILE) $(LINK)
.PHONY : input2

# =============================================================================

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... help"
	@echo "... $(SRCFILE)"
.PHONY : help

# =============================================================================
