# EarthBound Decompilation Project Guidelines

## Core Philosophy: Functional Equivalence Over Bit-Identical Reproduction

**Primary Goal**: Convert EarthBound's ASM to maintainable, modern C code that behaves identically to the original game but allows for future enhancements and cross-platform ports.

## Key Principles

- **Functional equivalence** - Same behavior, not bit-identical ROM reproduction  
- **Modern C practices** - Clean, maintainable code with compiler optimizations enabled
- **ROM data integrity** - All game assets (text, graphics, audio) come from extracted ROM data
- **Cross-platform foundation** - Code structure supports future SDL/modern platform ports
- **Zero compilation warnings** - Strict standards for code quality
- **Human-readable result** - Simple, well-structured C code without verbose ASM references

## Architecture Strategy

### Hybrid Build System
- **C code**: Flexible placement, allow compiler optimization
- **ROM data**: Fixed location via .incbin includes for asset integrity  
- **Memory layout**: Flexible segments for code, fixed segments for hardware requirements only

### Future Platform Support
- **SNES target**: CC65 → ld65 → .sfc ROM
- **Modern platforms**: GCC/Clang → SDL executable (Linux/Windows/Mac/mobile)
- **Asset management**: Runtime loader for cross-platform compatibility

## ASM to C Conversion Procedure

**PHASE 1 DEVELOPMENT NOTICE**: All current ASM→C conversion work is **Phase 1** development focused on functional SNES ROM builds with `.incbin` data inclusion.

**📋 IMPORTANT: See `C89_CONVERSION_GUIDE.md` for complete C89 compliance requirements, build system details, and conversion workflow.**

**CRITICAL ASM FILES**: Files with `.critical.asm` extension must NEVER be converted to C - they contain SNES hardware boot code that must remain at fixed addresses (system vectors, reset handlers, etc.)

**IMPORTANT: Always check for existing C implementations before converting new ASM functions to avoid duplicates!**

### Before Converting Any ASM Function:

1. **Read the ASM file** to understand the function name and behavior
2. **Search for existing C implementation** using:
   ```bash
   grep -r "function_name" src/ include/
   ```
3. **If function exists in C:**
   - Compare ASM behavior with C implementation  
   - Verify the C code matches the ASM functionality
   - If C code is correct, just mark ASM as `.converted.asm`
   - If C code needs fixes, update the C implementation
4. **If function doesn't exist:**
   - Convert ASM to C following the guidelines above
   - Add function declaration to appropriate header
   - Mark ASM file as `.converted.asm`

### Conversion Workflow:
1. `grep -r "BTLACT_FUNCTION_NAME" src/` - Check if already implemented
2. If found: Review and verify C implementation matches ASM
3. If not found: Convert ASM → C → Add to header → Test compilation
4. **Mark ASM file appropriately:**
   - `.converted.asm` - Successfully converted to C
   - `.later.asm` - Requires dependencies/complex systems, defer until later  
   - `.critical.asm` - NEVER convert to C (SNES hardware boot code)
5. **Test compilation**: `cc65 -t none -O -I include src/file.c -o build/file.s`

This procedure prevents duplicate function creation and avoids repeatedly analyzing complex dependencies.

## General Conversion Guidelines

### C89 Compliance
- **All variable declarations at function start** - See C89_CONVERSION_GUIDE.md for details
- **Test compilation after each conversion**: `cc65 -t none -O -I include src/file.c -o build/file.s`
- **Avoid mid-function declarations** - Move all `int i`, `unsigned char result`, etc. to top of function

### Standard Library Usage
- **Check CC65 library first** - Many functions already exist (strlen, memcpy, rand, etc.)
- **Use standard functions when possible** - See C89_CONVERSION_GUIDE.md for complete list
- **Custom implementation when needed** - SNES hardware access, performance requirements

### Function Implementation Strategy

#### For Missing Constants/Dependencies:
1. **Search for constants** in include/ directory and ASM files
2. **Add to appropriate headers** before implementing functions  
3. **Look for similar working functions** to understand patterns

#### For Complex Functions Requiring Struct Fields:
1. **Check if required struct fields exist** in headers
2. **If missing fields**: Search ASM files for struct definitions to add them
3. **Comment out incomplete code** with TODO explaining what's needed:
   ```c
   // TODO: Requires [specific_field] in [struct_name] - add from ASM definition
   // [commented working code that needs the missing field]
   ```

#### For ROM Data Access:
- **Always use ROM data constants** from rom_data.h
- **Include proper headers**: `#include "rom_data.h"`
- **Verify constants exist** in earthbound.yml before using

### Conversion Efficiency Tips

#### Pattern Recognition:
- **Batch similar functions** - Null functions, stat modifiers, status effects
- **Use templates for common patterns** - Similar ASM functions often have identical C structure
- **Focus on high-impact directories first** - Areas with many simple functions

#### Semi-Automated Helpers:
```bash
# Find functions by pattern
find src -name "*pattern*.asm" | grep -v converted

# Generate function declarations
grep "void function_" src/file.c | sed 's/{.*//g' | sed 's/^/extern /'

# Batch rename converted files
find src/directory -name "*.asm" -exec mv {} {}.converted.asm \;
```