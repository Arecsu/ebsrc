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

**PHASE 1 DEVELOPMENT NOTICE**: All current ASM→C conversion work is **Phase 1** development focused on functional SNES ROM builds with `.incbin` data inclusion. See ARCHITECTURE.md for full multi-phase strategy.

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
5. Test compilation to ensure no errors

This procedure prevents duplicate function creation and avoids repeatedly analyzing complex dependencies.

## Display Function Usage Patterns

**CRITICAL: Always use the correct function signatures and message types to avoid compilation errors!**

### Message Display Functions:

#### `display_battle_text_ptr(const unsigned char* msg_ptr)`
- **Usage**: `display_battle_text_ptr(MSG_BTL_CONSTANT);` (direct pointer, no cast needed)
- **ALWAYS use ROM data from rom_data.h**:
  ```c
  // Correct pattern - all MSG_BTL constants from rom_data.h are pointers
  display_battle_text_ptr(MSG_BTL_KIKANAI);        // Real ROM data pointer
  display_battle_text_ptr(MSG_BTL_TATAKU_YOKETA);  // Real ROM data pointer  
  display_battle_text_ptr(MSG_BTL_NEMURI_ON);      // Real ROM data pointer
  ```
- **NEVER use local `#define` placeholders** - all messages come from ROM data extraction
- **Include**: Add `#include "include/rom_data.h"` to access MSG_BTL constants
- **Verification**: Before using any MSG_BTL constant, verify it exists in earthbound.yml and rom_data.h
- **Why**: Function takes 32-bit pointers (SNES addressing), not 16-bit IDs. No casting needed!

#### `display_text_wait(const char* text, unsigned long param)`
- **Usage**: `display_text_wait((const char*)MSG_CONSTANT, value);`
- **Parameter 1**: Message pointer (always cast to `const char*`)
- **Parameter 2**: Numeric value to display in message
- **Examples**:
  ```c
  display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
  display_text_wait((const char*)MSG_BTL_2GUTS_UP, new_guts_value);
  ```

#### `get_battle_message(const unsigned char* const message_ptr)`
- **Usage**: `display_text_wait(get_battle_message(MSG_CONSTANT), value);`
- **Returns**: Processed message string
- **Example**: `display_text_wait(get_battle_message(MSG_BTL_KOORI_ON), 0);`

### Status/Afflictions Access Patterns:

#### Battler Struct Fields:
- **Afflictions**: `target->afflictions[STATUS_GROUP_ID]` (array indexed by group)
- **Examples**:
  ```c
  target->afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] = STATUS_0_COLD;
  target->afflictions[STATUS_GROUP_TEMPORARY] = STATUS_2_ASLEEP;
  ```

### Function Implementation Strategy:

#### For Complex Functions Requiring Struct Fields:
1. **First check if battler struct has required fields**
2. **If missing fields**: Search ASM files for struct definitions to add them
3. **Don't delete working code** - comment it out with TODO explaining what's needed:
   ```c
   // TODO: Requires afflictions field in battler struct - add from ASM definition
   // battler* target = get_battler(CURRENT_TARGET);
   // if (target->afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_UNCONSCIOUS) {
   //     revive_target(CURRENT_TARGET, target->hp_max);
   // }
   ```

#### For Missing Constants/Dependencies:
1. **Search for constants in include/config.asm and include/ directory**
2. **Add constants to appropriate C files before implementing functions**
3. **Look for similar working functions to understand patterns**

### Progress Tracking Guidelines:
- **Minimize verbosity** - focus on action over reporting
- **No decorative formatting** - avoid emojis, percentages, "achievements" sections
- **Results over process** - state what was done, not how impressive it was
- **Efficient updates** - update TODO.md only when significant milestones reached

## Speed Optimization Strategies

### 1. Pattern-Based Batch Conversion
**Identify and convert similar functions in batches:**

**A) Null Functions (Empty implementations):**
```bash
# Find all remaining null functions
find src -name "null*.asm" | grep -v converted
# Convert all at once - they're all empty functions
```

**B) Stat Modifier Functions (1d4 pattern):**
```bash  
# Find stat up functions: offense_up, defense_up, speed_up, etc.
find src -name "*_up_1d4.asm" | grep -v converted
# Same pattern: rand_limit(4) + 1, increase stat, display message
```

**C) Status Effect Functions (Similar patterns):**
```bash
# Find status infliction functions
find src -name "*poison*.asm" -o -name "*paralyze*.asm" -o -name "*crying*.asm"
# Pattern: resistance check → inflict_status_battle → display message
```

### 2. Prioritize High-Impact Directories
**Focus on directories with many simple functions:**
- `src/battle/actions/` 
- `src/system/math/` 
- `src/text/`
- `src/misc/`

### 3. Template-Based Conversion
**Create templates for common function types:**

**Template A - Status Effect:**
```c
void btlact_STATUS_NAME(void) {
    if (fail_attack_on_npcs() != 0) return;
    battler* target = get_battler(CURRENT_TARGET);
    if (success_255(target->RESIST_TYPE) == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    if (inflict_status_battle(CURRENT_TARGET, STATUS_VALUE, STATUS_GROUP) != 0) {
        display_text_wait((const char*)MSG_BTL_SUCCESS_MSG, 0);
    } else {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    }
}
```

### 4. Semi-Automated Workflows
**Use grep/sed for repetitive tasks:**
```bash
# Batch rename converted files
find src/battle/actions -name "null*.asm" -exec mv {} {}.converted \;

# Find function patterns
grep -r "BTLACT_.*UP_1D4" src/ | cut -d: -f1 | sort -u

# Generate function declarations automatically  
grep "void btlact_" src/battle.c | sed 's/{.*//g' | sed 's/^/extern /'
```