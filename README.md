# EarthBound Decompilation Project

A multi-target decompilation of EarthBound (Mother 2) supporting both SNES ROM builds and modern PC platforms.

## Project Overview

This project converts EarthBound's original assembly code to maintainable C code with multiple build targets:

- **SNES ROM Build**: Creates bit-accurate `.sfc` ROM files using CC65
- **PC Build**: Cross-platform executable using SDL2 for modern systems

## Quick Start

### PC Build (Current Development)
```bash
# Install dependencies (macOS)
brew install cmake sdl2 sdl2_image sdl2_mixer

# Build
mkdir -p build && cd build
cmake ..
make

# Run
./earthbound
```

### SNES Build (Archive Branch)
```bash
# Switch to archived SNES build
git checkout snes-build-archive

# Prerequisites
# - CC65 compiler
# - Your own EarthBound ROM file as `donor.sfc`

# Build SNES ROM
make -f Makefile.phase1
```

## Project Evolution

### Phase 1: ASM Decompilation ✅ **COMPLETED**
- Original ASM files extracted from EarthBound ROM
- Located in directories with `.asm` files
- Build system: `Makefile` → CC65 → `.sfc` ROM

### Phase 2: ASM to C89 for SNES ✅ **ARCHIVED** (`snes-build-archive` branch)
- C89 compliant code targeting SNES hardware
- Banking pragmas and SNES-specific optimizations  
- Build system: CC65 with C89 → `.sfc` ROM
- **Status**: Stable build preserved in archive branch

### Phase 3: Modern PC Build 🔄 **CURRENT** (`main` branch)
- Modern C11 code with SDL2 platform layer
- Cross-platform executable for Linux/Windows/macOS
- Build system: CMake → SDL2 executable
- **Status**: Active development

## Directory Structure

```
ebsrc/
├── CMakeLists.txt              # Modern PC build configuration
├── Makefile*                   # Legacy SNES build files
├── snes*.cfg                   # SNES memory layout configs
├── README.md                   # This file
├── DEVELOPMENT.md              # Development guidelines
├── CLAUDE.md                   # AI assistant instructions
├── C89_CONVERSION_GUIDE.md     # Legacy SNES conversion guide
│
├── src/                        # Source code
│   ├── main.c                  # PC build entry point (NEW)
│   ├── platform/               # SDL2 abstraction layer (NEW)
│   │   ├── graphics.c/h        # Graphics/rendering
│   │   ├── audio.c/h          # Audio/music system  
│   │   └── input.c/h          # Input handling
│   ├── stubs.c                # Temporary implementations (NEW)
│   ├── battle.c               # Battle system (modernized)
│   ├── system.c               # Core system functions (modernized)
│   ├── audio.c                # Audio system (legacy)
│   ├── *.c                    # Other game systems
│   ├── battle/                # Battle ASM files
│   │   ├── *.asm              # Unconverted ASM
│   │   └── *.converted.asm    # Converted to C
│   └── */                     # Other ASM directories
│
├── include/                   # Header files
│   ├── platform/             # Platform abstraction headers (NEW)
│   ├── battle.h              # Battle system
│   ├── structs.h             # Game data structures  
│   ├── hardware.h            # Hardware definitions
│   └── *.h                   # Other headers
│
├── build/                     # PC build output (NEW)
├── assets/                    # Game assets (extracted ROM data)
└── tools/                     # Build tools
```

## Development Status

### ✅ Completed
- [x] CMake-based cross-platform build system
- [x] SDL2 platform abstraction layer
- [x] Modern C11 code style migration
- [x] Project structure organization
- [x] Basic compilation pipeline

### 🚧 In Progress  
- [ ] Complete ASM to C conversion
- [ ] Implement missing game functions
- [ ] Asset loading system
- [ ] Graphics rendering pipeline

### 📋 Planned
- [ ] Audio system implementation  
- [ ] Input mapping and controls
- [ ] Save/load system
- [ ] Cross-platform distribution

## Code Organization

### Modern C Code (PC Build)
- **Location**: `src/*.c` (main files)
- **Style**: C11 with inline declarations
- **Platform**: SDL2 abstraction layer
- **Build**: CMake

### Legacy ASM Files
- **Location**: `src/*/` subdirectories  
- **Status**: Being converted to C
- **Naming**: 
  - `.asm` = Needs conversion
  - `.converted.asm` = Successfully converted
  - `.critical.asm` = Never convert (SNES boot code)

### Headers
- **Location**: `include/`
- **Organization**: 
  - `platform/` = SDL2 abstraction
  - Root = Game systems and data structures

## Documentation

- **[README.md](README.md)** - This file - project overview and quick start
- **[DEVELOPMENT.md](DEVELOPMENT.md)** - Development guidelines and procedures
- **[CLAUDE.md](CLAUDE.md)** - AI assistant instructions and conversion procedures
- **[C89_CONVERSION_GUIDE.md](C89_CONVERSION_GUIDE.md)** - Legacy SNES build technical reference

## Contributing

See `DEVELOPMENT.md` for detailed development guidelines including:
- ASM to C conversion procedures
- Code style guidelines  
- Testing procedures
- Platform compatibility requirements

## References

- [Original ASM disassembly repo](https://github.com/Herringway/ebsrc)

## License

This is a decompilation project for educational and preservation purposes. This project contains no copyrighted game data - users must provide their own ROM files for data extraction.