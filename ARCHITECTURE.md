# EarthBound Decompilation Project Architecture

## Overview

This document describes the architecture and design principles for converting the EarthBound ROM from 65816 assembly to maintainable C code while preserving all original functionality and data.

## Project Goals

- **Complete ASM→C conversion** of 3,455 assembly files  
- **Zero compilation warnings** with strict GCC flags
- **Human-readable, maintainable code** without verbose ASM references
- **Functional equivalence** - same behavior, not bit-identical reproduction
- **ROM data integrity** - all text, graphics, audio from original ROM data
- **Multi-variant support** - JP/US/prototype ROM variants
- **Cross-platform portability** - foundation for SDL/modern platform ports
- **Flexible memory layout** - allow compiler optimization and code growth


## Architecture Layers

### Layer 1: ROM Data Extraction & Management

The project uses a sophisticated ROM data extraction system:

```
ROM (.sfc) → earthbound.yml → Extracted Data Files → Symbol Files → C Constants
```

#### Key Components:

**earthbound.yml**: Maps ROM offsets to extract data by type and region
```yaml
dumpEntries:
- subdir: audiopacks
  name: '0'
  offset: 0x2277F0
  size: 30012
  extension: ebm
```

**Symbol Files**: Define ROM addresses as global symbols
```asm
.GLOBAL MSG_BTL_HPSUCK_ON: far
.GLOBAL MSG_BTL_CHECK_OFFENSE: far
```

**Memory Layout** (snes.cfg): Flexible segment mapping for modern development
```
MEMORY {
    ROM: start = $C00000, size = $300000, fill = yes, type = ro;
}
SEGMENTS {
    CODE:     load = ROM, type = ro;           # C code (flexible placement)
    ROMDATA:  load = ROM, type = ro;           # Fixed ROM data (.incbin)
    VECTORS:  load = ROM, start = $00FFE4;     # Hardware vectors (fixed)
}
```

### Layer 2: C Data Access Layer (Proposed)

A clean C interface to ROM data that eliminates hardcoded strings:

```c
// include/rom_data.h - Auto-generated from symbol files
extern const unsigned char* const MSG_BTL_HPSUCK_ON;
extern const unsigned char* const MSG_BTL_CHECK_OFFENSE;
extern const unsigned char AUDIOPACK_BATTLE[];
extern const unsigned char SPRITE_NESS[];
extern const unsigned char TILESET_ONETT[];

// src/rom_data.c - Safe data access functions
const char* get_battle_message(unsigned short msg_id);
const unsigned char* get_audio_pack(unsigned char pack_id);
const unsigned char* get_sprite_data(unsigned short sprite_id);
const unsigned char* get_tileset_data(unsigned char tileset_id);
```

### Layer 3: Game Logic (C Implementation)

Clean, readable C code that uses the data layer:

```c
// src/battle.c
void btlact_spy(void) {
    battler* target = get_battler(CURRENT_TARGET);
    
    // Use ROM data, never hardcoded strings
    display_text_wait(get_battle_message(MSG_BTL_CHECK_OFFENSE), target->offense);
    display_text_wait(get_battle_message(MSG_BTL_CHECK_DEFENSE), target->defense);
    
    if (target->fire_resist == 0xFF) {
        display_battle_text_ptr(MSG_BTL_CHECK_ANTI_FIRE);
    }
}
```

### Layer 4: Build System Integration

## Multi-Phase Development Strategy

### **Phase 1: SNES Foundation (Current Focus)**
**Goal**: Functional SNES ROM with clean C codebase

**Build System:**
```
C files → CC65 → C Objects → ld65 → SNES ROM (.sfc)
ROM Data (.bin) → .incbin → Fixed ROM segments ┘
```

**Data Strategy:**
- Keep existing binary formats (`.lzhal`, `.bin`, `.pal`, etc.)
- Use `.incbin` inclusion for minimal disruption
- Create clean C data access layer over raw binary data
- Focus on **functional equivalence**, not bit-identical reproduction

**Current Work:** All ASM→C conversion work is **Phase 1** development

**Phase 1 Implementation:** Pure C build system (see BUILD_PHASE1.md for detailed documentation)

#### Pure C Build Architecture:
```
User ROM → Extract Tool → Generated Data → Pure C Build → SNES ROM
donor.sfc    ebbinex      src/bin/       CC65 only     earthbound.sfc
(not in repo)             (gitignored)   (no ASM mixing)
```

**Copyright Compliance:**
- ✅ No ROM data in repository
- ✅ User-provided ROM extraction  
- ✅ Generated files only (src/bin/ gitignored)
- ✅ Pure C source code committed to repo

### **Phase 2: Cross-Platform Preparation (Future)**
**Goal**: Modern development experience and tooling

**Enhanced Build System:**
```
C files → CC65 → SNES ROM (.sfc)          # SNES target
C files → GCC  → SDL Executable           # Development target
```

**Data Transformation:**
- Convert binary assets to developer-friendly formats
- Implement asset conversion tools (`tools/convert_*.py`)
- Add structured data formats for maps, sprites, text

**Examples:**
```c
// Graphics: Binary → Structured
typedef struct {
    unsigned char width, height;
    unsigned char* pixel_data;
    char filename[64];  // SDL: load PNG files
} sprite_frame_t;

// Text: Symbol files → JSON  
// messages.json - Easy localization
{
  "battle": {
    "hp_drain": "HP was drained!",
    "miss": "The attack missed!"
  }
}

// Maps: Binary arrangements → Structured data
typedef struct {
    unsigned short width, height;
    unsigned short tileset_id;
    unsigned short* tile_data;
    collision_data_t* collisions;
    npc_spawn_t* npcs;
} map_t;
```

### **Phase 3: Multi-Platform Release (Long-term)**
**Goal**: SDL executable for modern platforms

**Multi-Platform Build:**
```
C files → GCC/Clang → Native Executable (Linux/Windows/Mac/mobile/web)
PNG/JSON/OGG → Asset Loader → Runtime resources ┘
```

**Cross-Platform Asset System:**
```c
// Unified asset loading
#ifdef SNES_BUILD
    const unsigned char* data = get_rom_asset(asset_id);
#else  
    asset_t* data = load_asset_file(asset_filename);
#endif
```

**Benefits:**
- **Easy localization** - JSON text files
- **Graphics modding** - PNG sprite replacement  
- **Audio modding** - OGG/WAV audio files
- **Map editing** - Human-readable data formats
- **Modern debugging** - Standard development tools

---

**Current Priority**: Phase 1 development - ASM→C conversion with `.incbin` data access

## Directory Structure

```
ebsrc/
├── src/                    # Source code (C + remaining ASM)
│   ├── battle.c           # Battle system functions
│   ├── audio.c            # Audio/music functions  
│   ├── system.c           # Core system utilities
│   ├── rom_data.c         # ROM data access layer
│   └── battle/actions/    # Battle actions (mixed C/ASM)
├── include/               # C header files
│   ├── hardware.h         # Hardware definitions
│   ├── structs.h          # Game data structures
│   ├── rom_data.h         # ROM data constants (auto-generated)
│   └── symbols/           # ASM symbol files
├── build/                 # Compiled objects and outputs
├── earthbound.yml         # ROM data extraction config
├── snes.cfg              # Memory layout and linking config
├── Makefile.c            # C compilation testing
├── Makefile.backup       # Main ASM build system
├── TODO.md               # Conversion progress tracking
├── BUILD.md              # Build system documentation
└── CLAUDE.md             # Project guidelines and quality standards
```

## Data Management Philosophy

### 1. ROM Data Integrity

**NEVER embed hardcoded strings/data in C code**. All game content must come from original ROM data:

❌ **Wrong:**
```c
display_text_wait("HP drained!", damage);
```

✅ **Correct:**
```c
display_text_wait(get_battle_message(MSG_BTL_HPSUCK_ON), damage);
```

### 2. Future Asset Organization

As the project matures, ROM data will be organized with meaningful names:

```c
// Current (symbol-based)
extern const unsigned char* const MSG_BTL_HPSUCK_ON;

// Future (semantic naming)
extern const char* const BATTLE_MSG_HP_DRAIN_SUCCESS;
extern const sprite_data_t SPRITE_NESS_WALKING;
extern const sprite_data_t SPRITE_NESS_BATTLE;
extern const sprite_data_t SPRITE_GIYGAS_FORM_1;
extern const tileset_data_t TILESET_ONETT_OVERWORLD;
extern const audio_track_t MUSIC_BATTLE_BOSS;
```

### 3. Localization Support

The architecture supports multiple ROM variants:

```c
// Conditional compilation for different ROM variants
#ifdef JPN
    #define MSG_BATTLE_WIN MSG_BTL_KATTA_JP
#elif defined(USA)
    #define MSG_BATTLE_WIN MSG_BTL_WON_USA  
#elif defined(PROTOTYPE19950327)
    #define MSG_BATTLE_WIN MSG_BTL_WON_PROTO
#endif
```


---

**This architecture provides a solid foundation for converting EarthBound's 65816 assembly to maintainable C code while preserving the original game's data integrity and supporting future enhancements like semantic asset naming.**