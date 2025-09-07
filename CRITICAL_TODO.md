# CRITICAL TODO: Phase 1 Pure C Build Migration

## Overview

**IMMEDIATE PRIORITY**: Migrate existing C files to work with the new Pure C build system (Makefile.phase1) and validate that we can compile functional ROMs without errors.

**Goal**: Get a working SNES ROM build using only C code + critical ASM vectors, with fast iteration cycles for testing. The reason we have scripts to convert banks and such is that we cannot have copyright material in the repo. So we need to provide a workflow to extract those from a provided ROM and process them in such way it fits our build and coding process. There could be procedures in CLAUDE.md like memory references that were stated for (now old) exact ROM memory layout architecture. As we are not striving for a bit-to-bit perfect compilation, these binaries such as text, music, etc can be addressed dynamically by the compiler and assembler as possible. For more information, check how ASM bank files will be converted to C files and don't hesitate to change or add any python script in "tools/" to work with this workflow. Remember: code should be human readable and maintaneable at the end of the day, and properly compiled to work on a SNES funcionally speaking, not bit-to-bit recreation.

---

## Step 1: Analyze Current State (30 mins)

### Files to Analyze:
```bash
# Current C modules that need migration:
src/audio.c      - Audio system functions
src/battle.c     - Battle system functions  
src/inventory.c  - Item/equipment system
src/misc.c       - Miscellaneous utilities
src/overworld.c  - Overworld/map system
src/psi.c        - PSI/magic system
src/rom_data.c   - ROM data access (may conflict with new system)
src/system.c     - Core system utilities
src/text.c       - Text/message system
```

### Analysis Tasks:
- [ ] **Check C file includes** - Do they reference correct headers?
- [ ] **Check ROM data usage** - Do they use old MSG_BTL constants or new rom_data.h?
- [ ] **Check function exports** - Are all functions properly declared in headers?
- [ ] **Check compiler pragmas** - Do files specify correct memory segments?

### Analysis Commands:
```bash
# Check current include patterns
grep -r "#include" src/*.c

# Check ROM data constant usage  
grep -r "MSG_BTL\|MSG_" src/*.c

# Check for segment pragmas
grep -r "#pragma" src/*.c

# Check for hardcoded addresses or ASM references
grep -r "0x[0-9A-Fa-f]\|\.asm\|BANK[0-9]" src/*.c
```

---

## Step 2: One-Time ROM Data Extraction (5 mins)

### Extract ROM Data (Run Once)
```bash
# Copy your EarthBound ROM to project root
cp ~/earthbound.sfc donor.sfc

# Extract ROM data (only need to do this once)
make -f Makefile.phase1 extract

# Verify extraction worked
make -f Makefile.phase1 check-rom-data
```

**IMPORTANT**: After this step, **do NOT re-extract** during development. The src/bin/ data stays constant for fast iteration.

---

## Step 3: Generate C Bank Structure (10 mins)

### Generate Initial C Bank Files
```bash
# Generate C bank files from current ASM structure
make -f Makefile.phase1 generate-c-banks

# Review generated files
ls src/generated/
cat src/generated/bank01.c  # Check structure
cat src/rom_data_c.c        # Check ROM data inclusion
```

### Expected Output:
- `src/generated/bank01.c` - Bank 1 C implementation
- `src/generated/bank02.c` - Bank 2 C implementation  
- `src/generated/bank03.c` - Bank 3 C implementation
- `src/rom_data_c.c` - Centralized ROM data inclusion

---

## Step 4: Migrate C Files for Pure C Build (45 mins)

### 4.1: Add Compiler Pragmas to Existing C Files

**For each C file, add appropriate segment pragma:**

```c
// At top of src/battle.c
#pragma code-name ("BANK01")
#include "include/battle.h"
// ... rest of file
```

**Segment Assignment:**
- `src/battle.c` → `#pragma code-name ("BANK01")`
- `src/audio.c` → `#pragma code-name ("BANK02")`  
- `src/system.c` → `#pragma code-name ("BANK03")`
- `src/psi.c` → `#pragma code-name ("BANK04")`
- `src/inventory.c` → `#pragma code-name ("BANK05")`
- `src/overworld.c` → `#pragma code-name ("BANK06")`
- `src/text.c` → `#pragma code-name ("BANK07")`
- `src/misc.c` → `#pragma code-name ("BANK08")`

### 4.2: Fix ROM Data References

**Problem**: Current C files may use old MSG_BTL constants that conflict with new system.

**Solution**: Ensure all C files use `#include "include/rom_data.h"` and reference ROM data correctly.

```c
// Check current usage:
grep -r "MSG_BTL" src/*.c

// Should be:
display_battle_text_ptr(MSG_BTL_KIKANAI);  // From rom_data.h
```

### 4.3: Update Makefile References

**Modify Makefile.phase1 to compile existing C files:**

```makefile
# Replace the build-c-objects section with actual files:
build-c-objects: $(BUILDDIR)
	@echo "Compiling existing C source files..."
	$(CC) $(CC65FLAGS) --code-name BANK01 -o "$(BUILDDIR)/battle.o" "src/battle.c"
	$(CC) $(CC65FLAGS) --code-name BANK02 -o "$(BUILDDIR)/audio.o" "src/audio.c"
	$(CC) $(CC65FLAGS) --code-name BANK03 -o "$(BUILDDIR)/system.o" "src/system.c"
	$(CC) $(CC65FLAGS) --code-name BANK04 -o "$(BUILDDIR)/psi.o" "src/psi.c"
	$(CC) $(CC65FLAGS) --code-name BANK05 -o "$(BUILDDIR)/inventory.o" "src/inventory.c"
	$(CC) $(CC65FLAGS) --code-name BANK06 -o "$(BUILDDIR)/overworld.o" "src/overworld.c"
	$(CC) $(CC65FLAGS) --code-name BANK07 -o "$(BUILDDIR)/text.o" "src/text.c"
	$(CC) $(CC65FLAGS) --code-name BANK08 -o "$(BUILDDIR)/misc.o" "src/misc.c"
```

---

## Step 5: Fast Iteration Testing Loop (Ongoing)

### 5.1: Initial Build Test
```bash
# First test build (will likely have errors)
make -f Makefile.phase1 clean
make -f Makefile.phase1 earthbound 2>&1 | tee build.log

# Check for compilation errors
grep -i "error\|warning" build.log
```

### 5.2: Rapid Fix-Test Cycle

**Fast iteration workflow (no ROM re-extraction):**

```bash
# Fix C code issues (headers, includes, etc.)
nano src/battle.c

# Compile just the changed file to check syntax quickly
cc65 -t none --cpu 65816 -I include --code-name BANK01 src/battle.c -o /tmp/test.o

# If individual compile works, test full build
make -f Makefile.phase1 earthbound

# Repeat until clean build
```

### 5.3: Build Validation Checklist

**Success Criteria:**
- [ ] **No compilation errors** - all .c files compile to .o files
- [ ] **No linking errors** - ld65 successfully creates .sfc file
- [ ] **No warnings** (goal, but not blocking initially)
- [ ] **Valid ROM file** - output .sfc file has correct size (~3-4MB)
- [ ] **ROM header valid** - basic SNES ROM structure present

**Validation Commands:**
```bash
# Check build artifacts
ls -la build/
file build/earthbound.sfc
hexdump -C build/earthbound.sfc | head -2  # Check ROM header

# Check ROM size (should be ~3-4MB)
stat -f%z build/earthbound.sfc
```

---

## Step 6: Incremental Problem Solving

### Common Issues and Solutions:

**6.1: Missing Function Declarations**
```bash
# Find undefined symbols
grep "undefined" build.log

# Add missing declarations to appropriate headers
echo "extern void missing_function(void);" >> include/battle.h
```

**6.2: Conflicting ROM Data Constants**
```bash
# Find conflicting MSG_BTL definitions
grep -r "define.*MSG_BTL" include/ src/

# Remove local definitions, use only rom_data.h
```

**6.3: Segment Conflicts**
```bash
# Check memory map for conflicts
cat build/earthbound.map | grep -i "error\|conflict"
```

**6.4: Missing Dependencies**  
```bash
# Find missing includes
grep -r "error.*undeclared\|error.*unknown" build.log

# Add missing includes to C files
```

---

## Success Criteria

### Minimum Viable Build:
1. **Clean compilation** - all C files compile without errors
2. **Successful linking** - ld65 creates ROM file without errors  
3. **Valid ROM structure** - output file has correct SNES ROM format
4. **Manageable warnings** - few or no compiler warnings

### Stretch Goals:
1. **Zero warnings** - completely clean build
2. **ROM boots in emulator** - basic functionality test
3. **Fast iteration** - sub-30-second build times
4. **All existing functions working** - no regression in converted code

---

## Next Session Priorities

### Immediate (Day 1):
1. **Complete Steps 1-4** - Migration and basic build setup
2. **Get first build attempt** - identify major issues
3. **Fix critical errors** - focus on compilation first, then linking

### Follow-up (Day 2):  
1. **Clean build achievement** - zero errors, minimal warnings
2. **ROM validation** - ensure output is valid SNES ROM
3. **Functional testing** - basic emulator testing if possible

### Future Sessions:
1. **Performance optimization** - build time improvements
2. **Continue ASM→C conversion** - using new Pure C system
3. **Cross-platform preparation** - Phase 2 planning

---

## Key Principles

1. **Fast Iteration** - No ROM re-extraction during C code development
2. **Incremental Progress** - Fix one error type at a time  
3. **Validation First** - Get basic build working before optimizing
4. **Clean Separation** - C code vs ROM data vs critical ASM
5. **Documentation** - Record solutions for common problems

**Ready to start coding in the next session!** 🚀