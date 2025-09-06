# C Compilation Makefile for EarthBound Decompilation
# This is a test makefile to verify our C code compiles properly

CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I. -DTEST_COMPILATION

# Build directory
BUILDDIR = build

# C source files
CSOURCES = src/audio.c src/battle.c src/system.c src/misc.c src/overworld.c src/inventory.c src/text.c src/psi.c

# Object files (in build directory)
COBJECTS = $(CSOURCES:src/%.c=$(BUILDDIR)/%.o)

# Test program
TEST_TARGET = test_earthbound

.PHONY: all clean test

all: $(TEST_TARGET)

$(TEST_TARGET): $(COBJECTS) $(BUILDDIR)/test_compile.o
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: src/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR)/test_compile.o: test_compile.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

test: $(COBJECTS) $(BUILDDIR)/test_compile.o
	@echo "C compilation test successful!"
	@echo "All C files compiled without syntax errors:"
	@echo "  - audio.c: music/sound functions"
	@echo "  - battle.c: battle actions and recovery functions"  
	@echo "  - system.c: memory utilities and hardware functions"
	@echo "  - misc.c: inventory, equipment, ATM functions"
	@echo "  - overworld.c: movement, hotspots, entity collision functions"
	@echo "  - inventory.c: item classification and management functions"
	@echo "  - text.c: dialogue and text rendering functions"
	@echo "  - psi.c: PSI/magic system functions"
	@echo "Note: Linking skipped due to external dependencies"

clean:
	rm -rf $(BUILDDIR) $(TEST_TARGET)

# Help target
help:
	@echo "Available targets:"
	@echo "  all    - Build test compilation target" 
	@echo "  test   - Build and verify compilation"
	@echo "  clean  - Remove compiled files"
	@echo "  help   - Show this help"