# C Compilation Test Makefile for EarthBound Decompilation
# Uses CC65 to verify C89 compliance and compilation correctness

CC = cc65
CFLAGS = -t none --cpu 65816 -O -I include -DTEST_COMPILATION

# Build directory
BUILDDIR = build

# C source files  
CSOURCES = src/audio.c src/battle.c src/system.c src/misc.c src/overworld.c src/inventory.c src/text.c src/psi.c

# Assembly files (CC65 output - in build directory)
ASMFILES = $(CSOURCES:src/%.c=$(BUILDDIR)/%.s)

# Test target (compilation test only - no linking)
TEST_TARGET = compilation_test

.PHONY: all clean test

all: $(TEST_TARGET)

$(TEST_TARGET): $(ASMFILES) | $(BUILDDIR)
	@echo "✅ CC65 compilation test completed successfully!"
	@echo "All C files compiled to assembly without errors."
	@touch $(BUILDDIR)/$(TEST_TARGET)

$(BUILDDIR)/%.s: src/%.c | $(BUILDDIR)
	@echo "Compiling $< with CC65..."
	$(CC) $(CFLAGS) -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

test: $(TEST_TARGET)
	@echo "🎯 CC65 compilation test successful!"
	@echo "All C files compiled to assembly without errors:"
	@echo "  ✅ audio.c: music/sound functions"
	@echo "  ✅ battle.c: battle actions and recovery functions"  
	@echo "  ✅ system.c: memory utilities and hardware functions"
	@echo "  ✅ misc.c: inventory, equipment, ATM functions"
	@echo "  ✅ overworld.c: movement, hotspots, entity collision functions"
	@echo "  ✅ inventory.c: item classification and management functions"
	@echo "  ✅ text.c: dialogue and text rendering functions"
	@echo "  ✅ psi.c: PSI/magic system functions"
	@echo ""
	@echo "✨ C89 compliance verified with CC65!"
	@echo "Note: This tests C→ASM compilation only (no linking/ROM generation)"

clean:
	rm -rf $(BUILDDIR)

# Help target
help:
	@echo "Available targets:"
	@echo "  all    - Compile all C files to assembly with CC65" 
	@echo "  test   - Verify C89 compliance and compilation"
	@echo "  clean  - Remove generated assembly files"
	@echo "  help   - Show this help"
	@echo ""
	@echo "This makefile tests C89 compliance using the actual CC65 compiler."