# EarthBound Data Management Strategy

## Overview

This document outlines the multi-phase approach for managing game assets (graphics, audio, text, maps) throughout the EarthBound decompilation project development lifecycle.

## Current Data Inventory

### Graphics Data
- **Compressed**: `.lzhal` files (LZ compressed graphics)
- **Raw tiles**: `.bin` files (uncompressed tile data) 
- **Palettes**: `.pal` files (color data)
- **Locations**: `src/bin/maps/`, `src/bin/town_maps/`, `src/bin/graphics/`

### Map Data
- **Arrangements**: `.arr.lzhal` (compressed map layouts)
- **Tiles**: Chunk files in `src/bin/maps/tiles/`
- **Palettes**: Per-map color schemes

### Audio Data  
- **Music**: Various binary formats (need analysis)
- **Sound effects**: Binary audio data
- **Locations**: `src/bin/` subdirectories

### Text Data
- **Messages**: Symbol files in `src/bin/US/text_data/`
- **Dialogue**: ROM-extracted text data
- **Battle text**: MSG_BTL constants in symbol files

## Multi-Phase Data Strategy

### Phase 1: SNES Foundation (Current)
**Goal**: Minimal disruption, functional SNES ROM

#### Data Access Pattern:
```c
// src/rom_data.c - Clean access layer over binary data
#include "rom_data_maps.inc"     // .incbin map data
#include "rom_data_graphics.inc" // .incbin graphics data  
#include "rom_data_audio.inc"    // .incbin audio data
#include "rom_data_text.inc"     // .incbin text data

const unsigned char* get_map_tileset(unsigned short tileset_id) {
    return &MAP_TILESETS_DATA[tileset_id * TILESET_SIZE];
}

const unsigned char* get_sprite_frame(unsigned short sprite_id, unsigned char frame) {
    return &SPRITE_DATA[sprite_id * FRAMES_PER_SPRITE + frame];
}

const char* get_battle_message(unsigned short msg_id) {
    return (const char*)&BATTLE_MESSAGES_DATA[msg_id];
}
```

#### Build Integration:
```asm
; rom_data_graphics.inc - Include binary graphics
.segment "ROMDATA_GRAPHICS"
MAP_TILESETS_DATA:
.incbin "src/bin/maps/tiles/chunk_01.bin"
.incbin "src/bin/maps/tiles/chunk_02.bin"
; ... continue for all graphics data

SPRITE_DATA:
.incbin "src/bin/graphics/sprites/ness.gfx.lzhal"
; ... continue for all sprite data
```

#### Benefits:
- ✅ **Zero code changes** - existing C code continues working
- ✅ **Functional SNES ROM** - same behavior as original
- ✅ **Clean interface** - hide binary complexity behind C functions
- ✅ **Progress continuity** - ASM→C conversion work continues uninterrupted

### Phase 2: Developer-Friendly Transformation (Future)

#### Structured Graphics Data:
```c
// Transformation: Binary → Structured data
typedef struct {
    unsigned short width, height;
    unsigned char frame_count;
    unsigned char* pixel_data;
    palette_t* palette;
    char source_file[64];        // For debugging/modding
} sprite_set_t;

typedef struct {
    unsigned short colors[256];
    char name[32];
    char source_file[64];        // "palettes/overworld.pal"
} palette_t;

// Enhanced access with metadata
sprite_set_t* get_sprite_set(sprite_id_t id);
palette_t* get_palette(palette_id_t id);
```

#### Human-Readable Text Data:
```json
// messages.json - Easy localization and editing
{
  "battle": {
    "hp_drain": "HP was drained!",
    "miss": "The attack missed!",
    "victory": "You won!",
    "kikanai": "It had no effect!"
  },
  "dialogue": {
    "ness_mom_morning": "Good morning, Ness!",
    "ness_mom_bedtime": "Time for bed, Ness."
  },
  "items": {
    "hamburger": "A delicious hamburger.",
    "baseball_bat": "Ness's favorite bat."
  }
}
```

#### Structured Map Data:
```c
// Enhanced map format with gameplay metadata
typedef struct {
    unsigned short width, height;
    unsigned short tileset_id;
    unsigned short* tile_data;
    
    // Gameplay data
    collision_data_t collision_map;
    door_t doors[MAX_DOORS];
    npc_spawn_t npcs[MAX_NPCS];
    trigger_zone_t triggers[MAX_TRIGGERS];
    
    // Metadata
    char map_name[32];
    char source_file[64];        // "maps/onett_overworld.json"
} map_t;

// JSON map format for easy editing
{
  "name": "Onett Overworld",
  "width": 64, "height": 48,
  "tileset": "onett_tiles",
  "npcs": [
    {"id": "mayor", "x": 32, "y": 16, "dialogue": "welcome_to_onett"}
  ],
  "doors": [
    {"x": 30, "y": 20, "destination": "ness_house_interior"}
  ]
}
```

#### Asset Conversion Tools:
```bash
# Asset conversion utilities
tools/convert_graphics.py   # Binary → PNG + metadata  
tools/convert_text.py       # Symbol files → JSON
tools/convert_maps.py       # Binary arrangements → JSON
tools/convert_audio.py      # Binary → OGG/WAV

# Development workflow
make convert-assets         # Transform all binary data
make snes-rom              # Build SNES ROM (Phase 1 compatible)
make sdl-executable        # Build modern platform version
```

#### Dual Build System:
```c
// Cross-platform asset loading
#ifdef SNES_BUILD
    // Phase 1: Load from ROM .incbin data
    const unsigned char* sprite_data = get_rom_sprite(SPRITE_NESS);
#else
    // Phase 2+: Load from modern file formats
    sprite_set_t* sprite = load_sprite_png("sprites/ness.png");
#endif
```

### Phase 3: Multi-Platform Assets (Long-term)

#### Modern Asset Formats:
- **Graphics**: PNG files with JSON metadata
- **Audio**: OGG Vorbis, WAV files  
- **Text**: JSON with full localization support
- **Maps**: JSON with visual editing tool support

#### Runtime Asset Manager:
```c
// Unified cross-platform asset system
typedef struct {
    void* data;
    asset_type_t type;
    char filename[128];
    size_t size;
} asset_t;

// Platform-agnostic loading
asset_t* load_asset(const char* asset_path);
void free_asset(asset_t* asset);

// Easy modding support
void reload_asset(const char* asset_path);  // Hot reload for development
void register_asset_mod(const char* original, const char* replacement);
```

#### Benefits:
- 🎮 **Easy game modding** - Replace PNG/JSON files
- 🌍 **Full localization** - Complete translation support  
- 🛠️ **Modern tools** - Visual editors, standard formats
- 🚀 **Performance** - Optimized loading, caching, compression
- 📱 **Multi-platform** - Desktop, mobile, web deployment

## Implementation Timeline

### Phase 1 (Current - 6 months)
1. **Complete ASM→C conversion** with current binary data approach
2. **Implement .incbin build system** for ROM data inclusion
3. **Create clean C data access layer** over existing binary files
4. **Achieve functional SNES ROM** with all features working

### Phase 2 (Future - 6 months)  
1. **Develop asset conversion tools** (Python scripts)
2. **Transform critical assets** (text, graphics metadata)
3. **Implement dual build system** (SNES + development builds)
4. **Add SDL platform support** with modern asset loading

### Phase 3 (Long-term - 12+ months)
1. **Full multi-platform release** (Linux/Windows/Mac/mobile)
2. **Complete asset modernization** (PNG, JSON, OGG)
3. **Modding framework** with hot-reload and asset replacement
4. **Advanced features** (higher resolutions, enhanced audio, etc.)

---

**Current Focus**: Phase 1 development - all ASM→C conversion work uses existing binary data with `.incbin` inclusion approach.