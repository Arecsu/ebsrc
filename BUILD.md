# Build System Notes

## Current Build Setup

### GCC Test Build (Makefile.c)
- **Purpose**: Development, syntax checking, validation
- **Compiler**: GCC with strict warnings
- **Target**: Host system (development/testing)
- **Output**: Object files in `build/` directory
- **Status**: ✅ Working perfectly

### Future SNES Build (TBD)
- **Purpose**: Final SNES ROM generation
- **Compiler**: CC65 (65816 cross-compiler)  
- **Target**: SNES hardware (65816 processor)
- **Requirements**: Memory mapping, banking, hardware-specific code
- **Status**: ⏳ Planned after more C conversion

## Why Two Build Systems?

1. **GCC for Development**:
   - Fast iteration and testing
   - Excellent error messages and warnings  
   - Familiar toolchain for development
   - Validates C syntax and logic

2. **CC65 for SNES**:
   - Generates proper 65816 assembly
   - Handles SNES memory mapping and banking
   - Required for actual SNES ROM building
   - Hardware-specific optimizations

## File Organization

```
├── src/           # C source files
├── include/       # Header files  
├── build/         # Compiled object files (GCC)
├── Makefile.c     # GCC development build
└── TODO.md        # Project roadmap
```

## Build Commands

```bash
# Development testing (current)
make -f Makefile.c test

# Clean build files  
make -f Makefile.c clean

# Help
make -f Makefile.c help
```

## Next Steps

1. Continue converting ASM to C using GCC for validation
2. Once major conversion is complete, create CC65 build system  
3. Handle SNES-specific requirements (memory layout, banking)
4. Generate final ROM and test on SNES hardware/emulators