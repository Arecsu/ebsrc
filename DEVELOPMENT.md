# Development Guide

Comprehensive guide for contributing to the EarthBound decompilation project.

## Project Architecture

### Build Targets

#### PC Build (Current `main` branch)
- **Language**: Modern C11
- **Platform**: SDL2 cross-platform
- **Build System**: CMake
- **Target**: Windows/Linux/macOS executables

#### SNES Build (Archived `snes-build-archive` branch) 
- **Language**: C89 
- **Platform**: SNES hardware via CC65
- **Build System**: Make + CC65 toolchain
- **Target**: `.sfc` ROM files

### Code Organization

```
src/
├── main.c              # PC build entry point
├── platform/           # SDL2 abstraction layer
├── *.c                 # Core game systems (modernized)
├── stubs.c            # Temporary function implementations  
└── */                 # ASM files by system (being converted)

include/
├── platform/          # SDL2 platform headers
└── *.h               # Game system headers
```

## Development Workflow

### 1. Environment Setup

#### PC Development
```bash
# macOS
brew install cmake sdl2 sdl2_image sdl2_mixer

# Ubuntu/Debian  
sudo apt install cmake libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev

# Windows (MSYS2)
pacman -S cmake mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-SDL2_mixer
```

#### Build and Test
```bash
mkdir -p build && cd build
cmake ..
make

# Test run
./earthbound
```

### 2. ASM to C Conversion

#### Priority Order
1. **Simple utility functions** - Math, string operations, basic algorithms
2. **Core game systems** - Battle mechanics, character management  
3. **Complex subsystems** - Graphics, audio, save system
4. **Hardware-specific code** - Last priority, may need SDL equivalents

#### Conversion Procedure

**Step 1: Check for Existing Implementation**
```bash
# Search for function in current C code
grep -r "function_name" src/ include/
```

**Step 2: Analyze ASM Function**
- Read the `.asm` file to understand function behavior
- Identify parameters, return values, and dependencies
- Note any SNES-specific hardware operations

**Step 3: Convert to Modern C**
- Use C11 syntax with inline variable declarations
- Replace SNES-specific operations with SDL equivalents
- Add comprehensive error handling
- Use descriptive variable names

**Step 4: Add Function Declaration**
- Add declaration to appropriate header file
- Include proper parameter types and documentation

**Step 5: Test and Mark Complete**
```bash
# Test compilation
make

# If successful, mark ASM file as converted
mv src/system/function.asm src/system/function.converted.asm
```

#### Code Style Guidelines

**Modern C11 Style:**
```c
// ✅ Good: Modern C style
void process_battle_action(int action_id, int target_id) {
    if (action_id < 0 || target_id >= MAX_BATTLERS) {
        return;
    }
    
    // Inline variable declarations
    battler* target = &BATTLERS_TABLE[target_id];
    unsigned short damage = calculate_damage(action_id, target);
    
    // Modern for loop
    for (int i = 0; i < target->resistance_count; i++) {
        damage = apply_resistance(damage, target->resistances[i]);
    }
    
    apply_damage(target, damage);
}
```

**Legacy C89 Style (Avoid):**
```c
// ❌ Avoid: Legacy C89 style  
void process_battle_action(int action_id, int target_id) {
    battler* target;
    unsigned short damage;
    int i;  /* All declarations at top */
    
    target = &BATTLERS_TABLE[target_id];
    damage = calculate_damage(action_id, target);
    
    for (i = 0; i < target->resistance_count; i++) {
        damage = apply_resistance(damage, target->resistances[i]);
    }
    
    apply_damage(target, damage);
}
```

#### Hardware Abstraction

Replace SNES-specific code with SDL equivalents:

**Graphics:**
```c
// SNES (old)
*(volatile unsigned char*)0x2100 = brightness;

// SDL (new)  
set_screen_brightness(brightness);
```

**Audio:**
```c
// SNES (old)
load_spc700_data(bank, address);

// SDL (new)
play_music(track_id);
```

**Input:**
```c
// SNES (old)  
unsigned short pad_state = *(volatile unsigned short*)0x4218;

// SDL (new)
unsigned short pad_state = get_joypad_state(PLAYER_1);
```

### 3. Platform Layer Implementation

#### Graphics System
- **File**: `src/platform/graphics.c`
- **Purpose**: Abstract SNES PPU operations to SDL rendering
- **Key Functions**: sprite rendering, background drawing, screen effects

#### Audio System  
- **File**: `src/platform/audio.c`
- **Purpose**: Replace SPC700 audio with SDL_mixer
- **Key Functions**: music playback, sound effects, volume control

#### Input System
- **File**: `src/platform/input.c`  
- **Purpose**: Map SDL input events to EarthBound controls
- **Key Functions**: controller input, keyboard mapping, input state

### 4. Asset Management

#### Current State
- Game assets are embedded via `.incbin` in SNES build
- PC build needs runtime asset loading system

#### Planned Implementation
```
assets/
├── graphics/
│   ├── sprites/
│   ├── backgrounds/ 
│   └── ui/
├── audio/
│   ├── music/
│   └── sounds/
└── data/
    ├── text/
    ├── maps/
    └── battle_data/
```

### 5. Testing Procedures

#### Compilation Testing
```bash
# Test PC build
cd build && make

# Check for warnings (should be minimal)
make 2>&1 | grep warning

# Test with different compilers
CC=clang make
CC=gcc make
```

#### Runtime Testing
```bash
# Basic execution test
./earthbound --test-mode

# Memory leak detection (Linux/macOS)
valgrind --tool=memcheck ./earthbound

# Performance profiling
perf record ./earthbound
```

#### Cross-Platform Testing
- **macOS**: Native development platform
- **Linux**: Test in Docker/VM 
- **Windows**: Test via MSYS2 or cross-compilation

### 6. Managing Technical Debt

#### Stub Functions
- **Location**: `src/stubs.c`
- **Purpose**: Temporary implementations to enable compilation
- **Strategy**: Replace with proper implementations incrementally

#### Missing Features
Current stubs that need implementation:
- Complex battle calculations
- Graphics rendering pipeline  
- Audio system integration
- Save/load functionality
- Menu system

#### Code Quality
- Use static analysis tools: `cppcheck`, `clang-static-analyzer`
- Maintain consistent naming conventions
- Add comprehensive error handling
- Document complex algorithms

### 7. Debugging Strategies

#### ASM to C Conversion Issues
```bash
# Compare ASM behavior with C implementation
objdump -d original.o > original.asm
objdump -d converted.o > converted.asm  
diff original.asm converted.asm
```

#### Runtime Issues
```bash
# Enable verbose debugging
./earthbound --debug --verbose

# Memory debugging
ADDRESS_SANITIZER=1 make
./earthbound

# Performance profiling
./earthbound --profile-output=profile.log
```

#### Build Issues
```bash
# Clean rebuild
make clean && make

# Verbose build output
make VERBOSE=1

# Check dependencies
cmake .. --debug-output
```

## Contribution Guidelines

### Code Review Checklist
- [ ] Modern C11 style used consistently
- [ ] No SNES-specific hardware operations remain
- [ ] Function declarations added to headers
- [ ] ASM files marked as `.converted.asm`
- [ ] Compilation successful with no warnings
- [ ] Basic runtime testing completed

### Commit Message Format
```
Brief description of changes

- Detailed list of modifications
- Reference to ASM files converted  
- Any breaking changes or new dependencies

🤖 Generated with [Claude Code](https://claude.ai/code)

Co-Authored-By: Claude <noreply@anthropic.com>
```

### Pull Request Process
1. Create feature branch from `main`
2. Convert related ASM functions as a batch
3. Test compilation and basic runtime
4. Update documentation if needed
5. Submit PR with detailed description

## Troubleshooting

### Common Issues

**Build Failures:**
- Missing SDL2 development packages
- Incorrect CMake configuration
- Missing function declarations

**Runtime Crashes:**  
- Uninitialized global variables
- Invalid pointer operations
- Missing SDL subsystem initialization

**Performance Issues:**
- Inefficient SDL rendering
- Missing optimizations  
- Memory leaks in asset loading

### Getting Help

- Check existing GitHub issues
- Review similar conversions in codebase
- Consult original ASM disassembly documentation
- Test against SNES build behavior when possible

---

This guide will be updated as the project evolves. Always check the latest version before starting significant work.