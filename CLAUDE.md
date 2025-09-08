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

### Hardware-Specific Functions Strategy

**SNES Hardware Register Functions**: Functions that use SNES hardware registers (multiplication, division, DMA, sound, etc.) should be preserved as assembly for performance and accuracy.

**Two Implementation Approaches**:

### Approach 1: C with Direct Hardware Register Access (PREFERRED)
For functions using **memory-mapped hardware registers** (multiplication, division, etc.):

1. **Identify hardware register usage**: Look for `f:WRMPYA`, `f:RDMPYL`, `f:WRDIVL`, etc.
2. **Find register addresses**: Check existing C code or SNES documentation
3. **Implement in C**: Use `*(volatile type*)address` for register access
4. **Add timing**: Use `__asm__ ("nop")` for hardware timing requirements

**Example**: Hardware-optimized math functions:
```c
// 16-bit multiplication using SNES hardware multiplier
unsigned short mult16(unsigned short a, unsigned short b) {
    unsigned short low_a = a & 0xFF;
    unsigned short high_a = (a >> 8) & 0xFF;
    unsigned short result = 0;
    
    // Use hardware registers directly
    *(volatile unsigned char*)0x4202 = low_a;   // WRMPYA
    *(volatile unsigned char*)0x4203 = (b & 0xFF);  // WRMPYB
    __asm__ ("nop");  // Wait for hardware
    result += *(volatile unsigned short*)0x4216; // RDMPYL
    
    // Additional multiplications for 16x16...
    return result;
}
```

**Hardware Register Addresses**:
- Multiplication: `0x4202` (WRMPYA), `0x4203` (WRMPYB), `0x4216` (RDMPYL)
- Division: `0x4204` (WRDIVL), `0x4206` (WRDIVH/WRDIVB), `0x4214` (RDDIVL)

### Approach 2: Assembly .s Files (ONLY when needed)
For functions using **CPU mode operations** that C cannot handle:

1. **Identify CPU operations**: Bank switching (`phb`/`plb`), direct page (`phd`/`pld`)
2. **Copy original ASM to .s file**: Create `src/filename.s` with EXACT copy  
3. **Add to Makefile**: Include in `ASM_SRCS`
4. **Use .export _functionname**: Add leading underscore for CC65 compatibility

**Example**: CPU mode functions like `spc700_transfer.s`:
```assembly
.export _spc700_transfer_setup
.proc _spc700_transfer_setup: near
    phb         ; Push bank register (no C equivalent)
    phd         ; Push direct page register
    ; ... exact original code
    rts
.endproc
```

**IMPORTANT**: Never modify the original ASM logic when copying to .s files - preserve exact functionality including timing, temporary variables, and hardware register access patterns.

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

## Batch Processing Strategy for High-Volume Conversions

**IMPORTANT**: Most ASM functions are smaller and simpler than they appear. Process them in large batches for maximum efficiency!

### Function Complexity Classification

#### ✅ **Type 1: Size-Based Complexity (CONVERT IMMEDIATELY)**
**Characteristics:**
- 100-400 lines of straightforward logic
- Multiple switch cases, loops, or repeated patterns
- Long but predictable code structure
- Examples: `miss_calc()`, `eat_food()`, damage calculation functions

**Strategy:** Convert these NOW - they're just large, not algorithmically complex!
- Giant switch statements → Simple C switch cases
- Multiple status checks → Sequential if statements  
- Long parameter lists → C89 variable declarations at function start

#### ⚠️ **Type 2: Dependency Complexity (STRATEGIC DELAY)**
**Characteristics:**
- Calls to `UNKNOWN_*` or missing functions
- Requires undefined structs or complex data structures
- Hardware initialization sequences
- Examples: Functions calling `APPLY_CONDIMENT`, `UNKNOWN_C3ED98`

**Strategy:** Defer until dependencies are resolved
- Add missing struct definitions to headers when possible
- Create placeholder extern declarations for missing functions
- Convert the non-dependent parts and mark rest as TODO

### High-Volume Batch Processing Guidelines

**Target: 15-25+ functions per conversion session**

#### 1. **Pre-Conversion Batch Analysis (5 minutes)**
```bash
# Find all unconverted functions
find src/battle -name "*.asm" | grep -v converted | grep -v critical | head -20

# Quick size check - prioritize smaller ones first
wc -l src/battle/*.asm | sort -n | head -10

# Check for obvious patterns
grep -l "BEGIN_C_FUNCTION" src/battle/*.asm | head -15
```

#### 2. **Rapid Function Assessment**
- **< 50 lines**: Convert immediately, no analysis needed
- **50-200 lines**: Quick scan for missing dependencies, convert if clear
- **200+ lines**: Check for Type 1 vs Type 2 complexity
- **Any size with obvious switch patterns**: Convert immediately

#### 3. **Batch Implementation Strategy**
```bash
# Group related functions for efficiency
# Example batch: All targeting functions
target_all.asm
target_allies.asm  
target_enemies.asm
target_battler.asm

# Example batch: All status effect functions
poison.asm
paralyze.asm
mushroomize.asm
solidify.asm
```

#### 4. **Efficient Header Management**
- **Add 5-10 function declarations at once** to headers
- **Batch similar struct definitions** together
- **Group related constants** in logical blocks

#### 5. **Implementation Batching**
- **Implement 5-8 functions at once** in the same file
- **Test compilation after each batch** (not each function)
- **Mark multiple ASM files as converted** with single command:
```bash
for f in func1 func2 func3; do mv src/battle/$f.asm src/battle/$f.converted.asm; done
```

### Efficiency Multipliers

#### **Pattern Recognition Shortcuts:**
- **Battle actions**: 90% follow identical patterns - template approach
- **Targeting functions**: Almost identical logic with different filters
- **Status effects**: Predictable parameter passing and effect application
- **Damage calculations**: Similar structure with different multipliers

#### **Don't Over-Analyze:**
- If it compiles, it works - don't perfect every function
- Focus on functional equivalence, not optimization
- Simple direct conversions beat complex refactoring

#### **Minimum Viable Implementation:**
- Use basic struct definitions with "// More fields TBD"
- Create extern declarations for missing functions
- Implement core logic and defer edge cases

### Performance Benchmarks

**Proven Results:**
- **Session 1**: 3 functions (conservative approach)
- **Session 2**: 19 functions (optimized batch processing)
- **Target**: 25+ functions per session

**Key Success Factors:**
1. **Stop over-analyzing** - most functions are simpler than they look
2. **Batch similar patterns** - targeting functions, status effects, damage calculations
3. **Don't perfect each function** - functional equivalence over optimization
4. **Test compilation in batches** - not after every single function
5. **Use minimum viable structs** - add fields as needed later

This approach proved successful converting 19 functions in one session - aim for 25+ next time!

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

#### For Large Functions (Type 1 Complexity):
1. **Don't be intimidated by size** - 200+ line functions are usually just many simple cases
2. **Look for patterns**: Switch statements, repeated parameter checks, sequential status updates
3. **Convert directly**: Most long functions are straightforward C conversions
4. **Use basic struct definitions** with placeholder fields when needed
5. **Example approach for large functions**:
   ```c
   // Large switch-heavy function - convert case by case
   switch (food_type) {
       case FOOD_HP_RECOVERY: recover_hp(target, amount); break;
       case FOOD_PP_RECOVERY: recover_pp(target, amount); break;
       case FOOD_STAT_BOOST: /* implement stat boost logic */ break;
       // ... many more cases - all simple logic
   }
   ```

#### For Complex Functions Requiring Missing Dependencies (Type 2):
1. **Check if required functions exist** - search for UNKNOWN_ or missing calls
2. **If missing dependencies**: Add extern declarations and basic struct definitions
3. **Implement what's possible**, comment out complex parts:
   ```c
   // TODO: Requires APPLY_CONDIMENT function - defer until implemented
   // apply_condiment(target, condiment_type);
   
   // Continue with rest of function that doesn't need missing dependency
   update_battle_stats(target, effect);
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
- Remember: as long as you don't imagine or create things that don't actually exist in the original game and ASM files that's totally fine. Stay grounded in what actually exists in the original ASM files.

#### Semi-Automated Helpers:
```bash
# Find functions by pattern
find src -name "*pattern*.asm" | grep -v converted

# Generate function declarations
grep "void function_" src/file.c | sed 's/{.*//g' | sed 's/^/extern /'

# Batch rename converted files
find src/directory -name "*.asm" -exec mv {} {}.converted.asm \;
```