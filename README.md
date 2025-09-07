# EarthBound Decompilation Project

Converting EarthBound (Mother 2) from assembly to maintainable C code for cross-platform compatibility.

## Quick Start

### Prerequisites
- CC65 compiler (for SNES builds)
- Python 3 (for build tools)
- Your own EarthBound ROM file as `donor.sfc`

### Build
```bash
# Build SNES ROM
make -f Makefile.phase1

# Test C compilation only
make -f Makefile.c
```

## Project Status

**Phase 1: C89 Compliance & Build System** ✅ **COMPLETED**
- All C files compile with CC65
- Functional C→ASM→OBJ build pipeline  
- System vectors integrated with C interrupt handlers
- ROM data extraction system working

**Phase 2: ASM→C Conversion** 🔄 **IN PROGRESS**  
- Continue converting remaining ASM functions to C, prioritizing a minimal bootable build.
- Resolve runtime library dependencies
- Implement missing global variables

**Phase 3: Cross-Platform** ⏳ **PLANNED**
- SDL builds for modern platforms
- Asset loading system
- Cross-platform compatibility layer

## Documentation

- **[CLAUDE.md](CLAUDE.md)** - Project guidelines and conversion procedures. Yes we're vibing.
- **[C89_CONVERSION_GUIDE.md](C89_CONVERSION_GUIDE.md)** - Technical reference for C89 compliance and build system
- **[VUCC notes.md](VUCC%20notes.md)** - Original disassembly notes and function naming

## Key Principles

- **Functional equivalence** over bit-identical reproduction
- **C89 compliance** for CC65 compatibility  
- **Cross-platform foundation** for future SDL ports
- **Copyright compliance** via user-provided ROM extraction

## File Structure
```
src/           # C source files
include/       # Header files  
tools/         # Build and extraction tools
build/         # Compiled output (generated)
```

## Contributing

1. Read **C89_CONVERSION_GUIDE.md** for technical requirements
2. Follow ASM→C conversion workflow in **CLAUDE.md**
3. Test compilation: `cc65 -t none -O -I include src/file.c -o build/file.s`
4. Mark converted ASM files with `.converted.asm` extension

## References

- [Original ASM disassembly repo](https://github.com/Herringway/ebsrc)

## Copyright

This project contains no copyrighted game data. Users must provide their own ROM files for data extraction.