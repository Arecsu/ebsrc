# EarthBound Decompilation TODO

## Project Overview
Converting 3,455 executable assembly files to maintainable C code with functional equivalence to the original game. Focus on cross-platform compatibility and modern development practices.

## Architecture Change: Functional Equivalence Strategy ✅
**Decision**: Abandon bit-identical ROM reproduction in favor of:
- **Functional equivalence** - same behavior, modern C practices
- **Flexible memory layout** - allow compiler optimization
- **Cross-platform foundation** - support SDL ports to Linux/Windows/Mac
- **ROM data integrity** - preserve assets via .incbin inclusion

## Current Status  
- **8 C modules created** with 1500+ lines of code
- **143+ functions converted** from ASM to C (143/3455 files = 4.1% complete)
- **Complete C compilation system working** with ROM data access layer
- **Zero warnings with strict GCC flags**
- **Updated architecture documentation** for hybrid build approach
- Organized header structure with proper includes
- Clean build system with separate object directory

## Completed ✅

### Core Infrastructure
- [x] Project structure analysis and organization
- [x] Working C compilation system (Makefile.c) with zero warnings
- [x] Clean build system with `build/` directory separation
- [x] Comprehensive C structure definitions (structs.h) 
- [x] Hardware definitions header (hardware.h)
- [x] Organized header files for all modules

### Audio System (280+ lines) 
- [x] Music control (pause/resume/stop music, redirects)
- [x] Sound effects (play_sound, play_sound_and_unknown)
- [x] SPC700 communication (wait_for_spc700, load_spc700_data)
- [x] Audio configuration (set_audio_channels, get_audio_bank)
- [x] Music subsystem initialization (initialize_music_subsystem)
- [x] Music track management (change_music)
- [x] SPC700 data loading (complete hardware interface)

### Battle System (500+ lines)
- [x] HP/PP recovery functions (10, 50, 100, 200, 300, 1d4, 10000, 20, 80)
- [x] Battle actions (null actions, bash, level 1/2 attacks)
- [x] Damage calculations (350 fire damage, resistance handling)
- [x] Attack mechanics (miss calculation, dodge, critical hits)
- [x] Additional battle actions (reduce_pp, call_for_help, pray_warm)
- [x] New battle actions (null8/9, defense_shower, enemyextend, psi_flash_omega, bag_of_dragonite, mummy_wrap)
- [x] Status effect actions (crying2, inflict_solidification with proper ROM data integration)
- [x] Prayer actions (pray_golden for HP recovery based on missing HP)
- [x] Stat modification actions (defense_down_alpha for reducing target defense)
- [x] Equipment management (switch_armor, switch_weapons with full stat recalculation and weapon type detection)
- [x] PSI/magic actions (lifeup_beta healing, psi_fire_alpha, psi_rockin_beta spells)
- [x] Utility actions (sow_seeds summon, brainshock_alpha_redirect, random_stat_up_1d4)
- [x] Redirect functions (psi_shield_beta_redirect, level_3_attack_copy)

### System Utilities (220+ lines)
- [x] Memory management (memset24, memcpy16, memset16)
- [x] String utilities (eb_strlen)
- [x] DMA utilities (wait_dma_finished)
- [x] Graphics utilities (BG2 VRAM location, color modes)
- [x] IRQ callback management (reset, set callbacks)
- [x] Random number utilities (rand_0_3, rand_0_7)
- [x] Math utilities (mult8, mult16, division16, modulus8, rand_limit, truncate_16_to_8)
- [x] Advanced math functions (asl16, division8s, division8 with SNES hardware interface)
- [x] Palette animation (animate_palette with timer and frame management)

### Game Logic (130+ lines misc + 71 overworld + 64 text)
- [x] Character equipment management (weapon, arms, body, other)
- [x] ATM and wallet functions (deposit, withdraw, balance)
- [x] Equipment checking utilities (check_item_equipped)
- [x] Movement state management (mushroomized walking)
- [x] Hotspot management (disable hotspots)
- [x] Entity collision handling
- [x] Music control (change_music_5dd6)
- [x] Entity callback management (set_party_tick_callbacks)
- [x] Text system (window focus, sound mode, font control)

### Inventory System (38 lines)
- [x] Item classification (get_item_subtype, get_item_subtype_2)
- [x] Item configuration system

### PSI System (61 lines)
- [x] Shield PSI (psi_shield_alpha)
- [x] Flash PSI spells (psi_flash_alpha, paralysis, crying, feeling strange)

## Immediate Priority - Phase 1 Architecture Implementation 🚨

**All current work is Phase 1 development** - see ARCHITECTURE.md for full multi-phase strategy

### Build System Modernization (Phase 1) ✅
- [x] **Pure C build system implemented** - Makefile.phase1 with CC65-only compilation
- [x] **C bank generator created** - tools/generate_c_banks.py for ASM→C transition  
- [x] **Memory layout designed** - snes_phase1.cfg with flexible C segments
- [x] **Copyright-safe ROM handling** - extraction-based approach, no ROM data in repo
- [x] **System vectors preserved** - critical SNES boot code stays as ASM
- [ ] **Test Pure C build system** - validate ROM generation and functionality

### Phase 1 Validation and Testing
- [ ] **Test Pure C build system** - validate with donor ROM extraction
- [ ] **Compare functional equivalence** - verify behavior matches ASM build  
- [ ] **Cross-validate ROM generation** - test different ROM variants (US/JP/Proto)

### Critical System Files (.critical.asm) ✅
- [x] **System vectors renamed** - .critical.asm extension marks permanently ASM-only
- [x] **Hardware boot requirements preserved** - RESET/NMI/IRQ vectors stay fixed
- [x] **Memory layout finalized** - snes_phase1.cfg handles flexible C + fixed vectors

## High Priority - Core Systems 🔴

### Battle System
- [ ] Convert remaining battle actions (~90 files in src/battle/actions/)
- [ ] Battle calculation functions (damage, resistance, miss calc)
- [ ] Enemy AI and behavior functions
- [ ] Battle UI and display functions
- [ ] Status effect and ailment functions

### System/Hardware Layer
- [ ] Memory management functions (DMA, VRAM operations)
- [ ] Graphics and sprite functions
- [ ] Input handling and controller functions
- [ ] Save/load game functionality
- [ ] Hardware initialization and setup

### Audio System Extended
- [ ] Music track management and transitions
- [ ] Advanced sound effect processing
- [ ] SPC700 communication protocols
- [ ] Audio streaming and buffering

## Medium Priority - Game Logic 🟡

### Overworld System
- [ ] Character movement and collision detection
- [ ] Map loading and rendering
- [ ] NPC interaction and dialogue system
- [ ] Event triggers and scripting
- [ ] Door/transition handling

### Inventory & Items
- [ ] Item usage and effects
- [ ] Shop and purchase mechanics
- [ ] Item combination and cooking
- [ ] Equipment stat calculations
- [ ] Inventory UI functions

### Character Management
- [ ] Experience and leveling system
- [ ] PSI/magic spell system
- [ ] Character stats recalculation
- [ ] Party management functions
- [ ] Character creation/naming

## Lower Priority - Content & Data 🟢

### Text and Dialogue
- [ ] Text rendering and formatting
- [ ] Dialogue box management
- [ ] Text compression/decompression
- [ ] Localization support

### Data Processing
- [ ] Map data parsing and loading
- [ ] Sprite and graphics data handling
- [ ] Enemy and item data structures
- [ ] Event and script data processing

### Special Features
- [ ] Photo system functionality
- [ ] Telephone and communication features
- [ ] Mini-games and special events
- [ ] Ending sequence and credits

## Technical Infrastructure 📋

### Build System
- [x] Create C compiler configuration (Makefile.c working)
- [x] Set up compilation testing system 
- [ ] Set up proper linking and memory mapping for target hardware
- [ ] Create debug and release build configurations
- [ ] Implement automated testing framework

### Code Organization
- [ ] Create comprehensive header files for all modules
- [ ] Establish consistent naming conventions
- [ ] Add proper function documentation
- [ ] Organize files into logical directory structure

### Quality Assurance
- [ ] Implement unit tests for converted functions
- [ ] Create integration tests for major systems
- [ ] Performance optimization and profiling
- [ ] Memory usage analysis and optimization

## File Conversion Progress

### By Directory (Using .converted.asm tracking):
- **audio/**: 12/12 files converted (100%)
- **battle/actions**: 80/182 files converted (43%) ⬆️ +36 files
- **system/math/**: 22/29 files converted (75%) ⬆️ +5 files
- **text/**: 9/85 files converted (10%)
- **misc/**: 7/58 files converted (12%) - Equipment & wallet
- **system/**: 8/63 files converted (12%) - Core utilities  
- **overworld/**: 3/96 files converted (3%) - Basic functions
- **inventory/**: 2/4 files converted (50%)

### Total Progress: 143/3,455 executable ASM files converted (4.1%)

**Progress Tracking System**: 
- `.converted.asm` - Successfully converted to C
- `.later.asm` - Requires dependencies, marked for later conversion
- Check progress: `./check_progress.sh`
- Find ready files: `find src -name "*.asm" | grep -v ".converted" | grep -v ".later"`

## Phases
- **Phase 1** - Core Systems
- **Phase 2** - Game Logic
- **Phase 3** - Content & Polish
- **Phase 4** - Testing & Optimization

## Next Immediate Steps
1. **Text/Dialogue System** - Convert text rendering and dialogue functions
2. **More Battle Actions** - Convert remaining battle action functions
3. **Overworld Movement** - Implement complex movement and collision functions  
4. **PSI/Magic System** - Convert spell and magic functions
5. **Menu Systems** - Convert UI and menu handling functions
6. **Save/Load System** - Implement game state persistence


## Notes
- Keep C code simple and human-readable
- Avoid verbose ASM references in comments
- Unknown function names stay as "unknown" unless obvious
- Focus on compilation without ASM dependencies
- Maintain original functionality and behavior