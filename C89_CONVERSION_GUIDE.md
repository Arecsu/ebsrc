# C89 Conversion Guide for EarthBound Decompilation

This guide documents the C89 requirements, build system, and best practices for converting EarthBound's ASM code to C while maintaining SNES hardware compatibility.

## Overview

The EarthBound decompilation uses **CC65 compiler** which requires **strict C89 compliance**. The build system uses a two-stage process: C→ASM→OBJ→ROM to maintain precise control over SNES hardware requirements.

## C89 Compliance Requirements

### 1. Variable Declarations

**❌ WRONG (C99+ style):**
```c
void function(void) {
    some_code();
    int variable = 5;  // Mid-function declaration - NOT ALLOWED
    for (int i = 0; i < 10; i++) {  // Loop variable declaration - NOT ALLOWED
        // code
    }
}
```

**✅ CORRECT (C89 style):**
```c
void function(void) {
    int variable;
    int i;
    
    some_code();
    variable = 5;
    for (i = 0; i < 10; i++) {
        // code
    }
}
```

### 2. Function Declarations

All functions must be declared before use. Add to appropriate header files:

```c
// In header file (.h)
void my_function(unsigned short param);

// In source file (.c)  
void my_function(unsigned short param) {
    // implementation
}
```

### 3. Data Types

Use SNES-appropriate data types:
- `unsigned char` (8-bit)
- `unsigned short` (16-bit) 
- `unsigned long` (32-bit)
- Avoid `int` - size is compiler-dependent

### 4. Inline Assembly

CC65 uses different inline assembly syntax than GCC:

**❌ GCC-style (doesn't work in CC65):**
```c
__asm__ volatile ("nop");
```

**✅ CC65-compatible approaches:**
```c
// Option 1: Comment out and add TODO for later implementation
// TODO: Fix inline assembly for CC65 compatibility
// __asm__ volatile ("nop");

// Option 2: Use CC65 inline assembly syntax (research needed)
// asm("nop");

// Option 3: Create separate .s file for complex assembly
```

### 5. Constants and Macros

Use `#define` or `const` for constants. Avoid GNU C extensions:

```c
// Good
#define MAX_ITEMS 14
const unsigned char BATTLE_MENU_SIZE = 8;

// Avoid
enum { MAX_ITEMS = 14 }; // C99 feature
```

## Build System Architecture

### Two-Stage Compilation Process

```
C Source Files → CC65 → Assembly Files → CA65 → Object Files → LD65 → ROM
    (.c)                   (.s)                 (.o)              (.sfc)
```

### Makefile Structure

```makefile
# Stage 1: C to Assembly
$(CC) $(CC65FLAGS) --code-name BANK01 -o "build/file.s" "src/file.c"

# Stage 2: Assembly to Object  
$(AS) $(CA65FLAGS) -o "build/file.o" "build/file.s"

# Stage 3: Link all objects
$(LD) $(LD65FLAGS) -o "build/earthbound.sfc" build/*.o
```

### Bank Organization

Each C file is assigned to a specific SNES memory bank:
- `--code-name BANK01` for battle.c
- `--code-name BANK02` for audio.c  
- `--code-name BANK03` for system.c
- etc.

## Interrupt Handler Integration

### C Interrupt Handlers

System interrupt handlers are implemented in C but called from ASM vectors:

```c
// In system.c
void reset_handler(void) {
    // Hardware reset logic
    while (1) {} // Halt system
}

void nmi_handler(void) {
    // NMI (vertical blank) interrupt
}

void irq_handler(void) {
    // IRQ interrupt
    irq(); // Call existing IRQ function
}
```

### ASM Vector Files (.critical.asm)

Files with `.critical.asm` extension must NEVER be converted to C:

```assembly
; vectors.critical.asm
.IMPORT _reset_handler  ; Import C function (note _ prefix)
.IMPORT _nmi_handler
.IMPORT _irq_handler

RESET_VECTOR:
    JMP f:_reset_handler

NMI_VECTOR: 
    JMP f:_nmi_handler

IRQ_VECTOR:
    JMP f:_irq_handler
```

## ASM to C Conversion Workflow

### Before Converting Any Function:

1. **Search for existing C implementation:**
   ```bash
   grep -r "function_name" src/ include/
   ```

2. **Check if function already exists:**
   - If found: Compare ASM vs C behavior, fix C if needed
   - If not found: Convert ASM to C

3. **Mark ASM file appropriately:**
   - `.converted.asm` - Successfully converted to C
   - `.later.asm` - Requires dependencies, defer  
   - `.critical.asm` - NEVER convert (SNES boot code)

### Conversion Process:

1. Read and understand the ASM function
2. Implement equivalent C function 
3. Add function declaration to appropriate header
4. Test compilation: `cc65 -t none -O -I include src/file.c -o build/file.s`
5. Mark ASM file as `.converted.asm`

## Common Issues and Solutions

### Undefined Symbols During Linking

Expected during Phase 1 development. These represent:
- CC65 runtime functions (`pushax`, `tosaddeax`, etc.)
- Global variables defined in original ASM
- External functions not yet converted

**Solution:** Continue converting ASM functions to C. Runtime issues will be resolved in later phases.

### Missing Global Variables

Global variables referenced in C must be declared in headers:

```c
// In hardware.h
extern unsigned short CURRENT_TARGET;
extern unsigned char BATTLE_MENU_INDEX;
```

### Inline Assembly Compatibility  

When encountering GCC-style inline assembly:
1. Comment it out with TODO
2. Add explanation of what it does
3. Research CC65 equivalent syntax or create separate .s file

### Data Structure Access

When C code references undefined structs:
1. Search ASM files for struct definitions
2. Convert to C struct declarations  
3. Add to appropriate header files

## File Organization Best Practices

### Header Files Structure
```
include/
├── hardware.h      # Hardware registers, global variables
├── battle.h        # Battle system functions
├── audio.h         # Audio system functions
├── system.h        # System functions, interrupt handlers  
├── overworld.h     # Overworld functions
└── rom_data.h      # ROM data constants (auto-generated)
```

### Source Files Structure  
```
src/
├── battle.c        # Battle system implementation
├── audio.c         # Audio system implementation
├── system.c        # System functions, interrupt handlers
├── overworld.c     # Overworld implementation
└── system/
    ├── vectors.critical.asm     # NEVER CONVERT
    ├── reset_vector.critical.asm
    ├── nmi_vector.critical.asm
    └── irq_vector.critical.asm
```

## Testing and Validation

### Compilation Test
```bash
# Test individual C file
cc65 -t none -O -I include src/filename.c -o build/filename.s

# Test full build
make -f Makefile.phase1
```

### Expected Warnings (Safe to Ignore)
- Parameter never used warnings
- Result of comparison is constant warnings  

### Fatal Errors to Fix
- C89 variable declaration errors
- Undefined symbol errors during C compilation
- Syntax errors

## Memory Layout Considerations

### SNES Memory Constraints
- Each bank is 64KB maximum
- Code must fit within assigned bank segments
- Critical boot code must remain at fixed addresses

### Bank Assignment Strategy
- Frequently used functions in lower banks
- Audio code in dedicated bank (hardware timing critical)
- Battle system in dedicated banks (large code size)
- System functions distributed across banks

## Future Development Guidelines

### Phase 1 Goals (COMPLETED)
✅ C89 compliance for all C files  
✅ Functional C→ASM→OBJ build pipeline  
✅ System vector integration  
✅ All C files compile successfully

### Phase 2 Goals (Future)
- Resolve runtime library dependencies
- Convert remaining ASM functions to C
- Implement complete global variable definitions
- Achieve functional ROM build

### Phase 3 Goals (Future)  
- Cross-platform compatibility layer
- SDL-based modern platform builds
- Asset loading system for non-SNES targets

## Troubleshooting Common Problems

### "Size of data type is unknown"
- Using undefined struct types
- Solution: Define struct in header or use basic types

### "Expression expected" / "Undefined symbol"
- Missing variable declarations
- Mid-function C99-style declarations
- Solution: Move all declarations to function start

### "Symbol 'X' is undefined" in ASM files
- Missing `.IMPORT` statements
- Incorrect symbol names (remember C functions get _ prefix)
- Solution: Add proper imports in ASM files

### Linker errors with many unresolved externals
- Expected in Phase 1 - indicates successful C compilation
- These will be resolved as more ASM is converted to C

Remember: **Functional equivalence over bit-identical reproduction**. The goal is maintainable C code that behaves identically to the original game, not necessarily identical assembly output.

## CC65 Standard Library Reference

CC65 provides a subset of the C standard library optimized for 6502-based systems. Understanding what's available can save time during ASM→C conversion.

### Available Standard Functions

**Memory Functions:**
```c
#include <string.h>
void* memcpy(void* dest, const void* src, size_t count);
void* memset(void* dest, int value, size_t count);
void bzero(void* ptr, size_t count);  // Fill with zeros
```

**String Functions:**
```c  
#include <string.h>
size_t strlen(const char* str);       // ⚠️ Max 255 chars with -Os
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src);
int strcmp(const char* s1, const char* s2);
char* strupper(char* str);            // Convert to uppercase
char* strlower(char* str);            // Convert to lowercase
```

**Math Functions:**
```c
#include <stdlib.h>
int abs(int value);
int rand(void);                       // 0-32767 random number
void srand(unsigned int seed);
void _randomize(void);               // Auto-seed random
```

**Memory Allocation:**
```c
#include <stdlib.h>
void* malloc(size_t size);
void* calloc(size_t count, size_t size);
void free(void* ptr);
```

**Utility Functions:**
```c
#include <stdlib.h>
void qsort(void* base, size_t nmemb, size_t size, int (*compare)(const void*, const void*));
void* bsearch(const void* key, const void* base, size_t nmemb, size_t size, int (*compare)(const void*, const void*));
```

### 6502-Specific Considerations

**Memory Limitations:**
- Total addressable space: 64KB
- Dynamic memory allocation is limited
- Stack space is constrained (typically 256 bytes)

**Performance Notes:**
- No floating-point math in standard library
- Integer operations are optimized for 8-bit/16-bit values
- 32-bit operations are slower

**Calling Conventions:**
- Most functions use fastcall convention (parameters in registers when possible)
- Some functions are implemented as macros for efficiency

### When to Use Standard Library vs Custom Implementation

**Use Standard Library When:**
- Function exists and meets performance requirements
- Reduces code complexity significantly
- Memory usage is acceptable

**Write Custom Implementation When:**
- Need specific SNES hardware integration
- Standard function is too slow/large
- Need exact behavior match with original ASM

### Documentation Resources

- **Function Reference:** https://cc65.github.io/doc/funcref.html
- **Library Overview:** https://cc65.github.io/doc/library.html  
- **Header Files:** Check `/opt/homebrew/Cellar/cc65/*/share/cc65/include/` for complete function lists

### Example Usage in Conversion

```c
// Instead of implementing custom string copy:
void copy_player_name(void) {
    strcpy(CURRENT_PLAYER.name, INPUT_BUFFER);  // Use CC65 standard function
}

// For SNES-specific operations, write custom:
void dma_copy_vram(void* src, unsigned short vram_addr, unsigned short size) {
    // Custom implementation for SNES hardware
    *(volatile unsigned char*)VMAIN = 0x80;
    *(volatile unsigned short*)VMADD = vram_addr;
    // ... SNES-specific DMA code
}
```

This helps make informed decisions during ASM→C conversion about when to leverage existing library functions vs implementing custom solutions.