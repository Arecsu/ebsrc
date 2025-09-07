#ifdef __CC65__
#pragma code-name ("GLOBALS")
#endif

#include "hardware.h"

// =============================================================================
// CRITICAL GLOBAL VARIABLES FOR MINIMAL BOOTABLE BUILD
// =============================================================================
// These variables are referenced by our C code and must be defined
// for linking to succeed. Values are initialized to safe defaults.

// Graphics/Display Variables
unsigned char BG1SC_MIRROR = 0;
unsigned char BG2SC_MIRROR = 0; 
unsigned char BG3SC_MIRROR = 0;
unsigned char BG4SC_MIRROR = 0;
unsigned char BG12NBA_MIRROR = 0;
unsigned char BG34NBA_MIRROR = 0;

unsigned short BG1_X_POS = 0;
unsigned short BG1_Y_POS = 0;
unsigned short BG2_X_POS = 0;
unsigned short BG2_Y_POS = 0;
unsigned short BG3_X_POS = 0;
unsigned short BG3_Y_POS = 0;
unsigned short BG4_X_POS = 0;
unsigned short BG4_Y_POS = 0;

// Color averaging (for palette effects)
unsigned char COLOUR_AVERAGE_RED = 0;
unsigned char COLOUR_AVERAGE_GREEN = 0;
unsigned char COLOUR_AVERAGE_BLUE = 0;

// Battle System Variables
unsigned short CURRENT_ATTACKER = 0;
unsigned short CURRENT_TARGET = 0;

// Audio System Variables  
unsigned short CURRENT_MUSIC_TRACK = 0xFFFF;  // No music initially
unsigned char CURRENT_SEQUENCE_PACK = 0xFF;
unsigned char CURRENT_PRIMARY_SAMPLE_PACK = 0xFF;
unsigned char CURRENT_SECONDARY_SAMPLE_PACK = 0xFF;

// Audio Hardware State
unsigned char DISABLE_HPPP_ROLLING = 0;
unsigned char HALF_HPPP_METER_SPEED = 0;
unsigned char SOUND_EFFECT_QUEUE[8] = {0};
unsigned char SOUND_EFFECT_QUEUE_END_INDEX = 0;
unsigned char SOUND_EFFECT_UPPER_BIT_FLIPPER = 0;

// System State Variables
unsigned char NMITIMEN_MIRROR = 0;
unsigned short IRQ_CALLBACK = 0;
unsigned short DEFAULT_IRQ_CALLBACK = 0;

// Game State Variables (minimal)
struct game_state {
    unsigned long money;           // Player money
    unsigned long bank_balance;    // ATM balance
    unsigned char party_count;     // Number of party members
    unsigned char active_hotspot_modes[32]; // Hotspot states
} GAME_STATE = {0};

// Party and Character Data (minimal)
struct character {
    unsigned char level;
    unsigned short hp_max;
    unsigned short hp_current;
    unsigned char items[14];
    unsigned char equipped_weapon;
    unsigned char equipped_body;
    unsigned char equipped_arms;
    unsigned char equipped_other;
} PARTY_CHARACTERS[4] = {0}; // 4 party members

// Input/Controls
unsigned short PAD_RAW = 0;
unsigned short PAD_RAW_2 = 0;
unsigned short JOYPAD_1_DATA = 0;
unsigned short JOYPAD_2_DATA = 0;

// Entity/NPC System (minimal)
unsigned short CURRENT_ENTITY_INDEX = 0;
unsigned short ENTITY_COLLIDED_OBJECTS[32] = {0};
unsigned short ENTITY_COLLISION_NO_OBJECT = 0;
unsigned short ENTITY_COLLISION_DISABLED = 0xFFFF;

// Overworld State
unsigned char MUSHROOMIZED_WALKING_FLAG = 0;
unsigned short ENABLE_AUTO_SECTOR_MUSIC_CHANGES = 0;
unsigned char PARTY_MEMBER_TARGET = 0;
unsigned short NEXT_MAP_MUSIC_TRACK = 0;
unsigned short ENTITY_TICK_CALLBACK_LOW[64] = {0};
unsigned short ENTITY_TICK_CALLBACK_HIGH[64] = {0};

// Text/Display System
unsigned char INSTANT_PRINTING = 0;
unsigned char TEXT_PROMPT_WAITING_FOR_INPUT = 0;
unsigned char BLINKING_TRIANGLE_FLAG = 0;
unsigned short WINDOW_FOCUS = 0;
unsigned short CURRENT_FOCUS_WINDOW = 0xFFFF;
unsigned short TEXT_SOUND_MODE = 0;

// Window System (minimal)
struct window_stat {
    unsigned short x_pos;
    unsigned short y_pos;
    unsigned short width;
    unsigned short height;
    unsigned char font;
    unsigned long working_memory;
    unsigned char secondary_memory;
} WINDOW_STATS[8] = {0}; // Up to 8 windows

unsigned short OPEN_WINDOW_TABLE[8] = {0};

// Sprite/OAM System
struct oam_entry {
    unsigned char x_coord;
    unsigned char y_coord;
    unsigned char tile;
    unsigned char flags;
} OAM1[128] = {0}, OAM2[128] = {0};

unsigned char OAM1_HIGH_TABLE[32] = {0};
unsigned char OAM2_HIGH_TABLE[32] = {0};

unsigned short NEXT_FRAME_BUF_ID = 0;
unsigned short OAM_ADDR = 0;
unsigned short OAM_END_ADDR = 0;
unsigned short OAM_HIGH_TABLE_ADDR = 0;
unsigned char OAM_HIGH_TABLE_BUFFER = 0;

unsigned char PRIORITY_0_SPRITE_OFFSET = 0;
unsigned char PRIORITY_1_SPRITE_OFFSET = 0;
unsigned char PRIORITY_2_SPRITE_OFFSET = 0;
unsigned char PRIORITY_3_SPRITE_OFFSET = 0;

unsigned char SPRITE_VRAM_TABLE[88] = {0};

// Memory Test Variables
unsigned char SRAM_SIZE_1_SCRATCH = 0;
unsigned char SRAM_SIZE_2_SCRATCH = 0;
unsigned char SRAM_SIZE_3_SCRATCH = 0;
unsigned char LAST_SRAM_BANK = 0;

// Fade System
struct fade_params {
    unsigned char step;
    unsigned char delay;
} FADE_PARAMETERS = {0};

unsigned char FADE_DELAY_FRAMES_LEFT = 0;

// Palette Animation
struct palette_anim {
    unsigned char timer;
    unsigned char index;
    unsigned char delays[32];
} OVERWORLD_PALETTE_ANIM = {0};

// Audio Data Pointers (will be set by ROM data extraction)
const unsigned char* STEREO_MONO_DATA = (const unsigned char*)0x808000; // Safe ROM address
const unsigned char* MUSIC_DATASET_TABLE = (const unsigned char*)0x808100;
const unsigned char* MUSIC_PACK_POINTER_TABLE = (const unsigned char*)0x808200;

// Audio State Variables  
unsigned short SEQUENCE_PACK_MASK = 0xFFFF;
unsigned short UNKNOWN_7EB543 = 0;
unsigned short DISABLED_TRANSITIONS = 0;

// Additional Battle Variables (basic)
unsigned char BATTLE_MENU_CURRENT_OPTION = 0;
unsigned char BATTLE_MENU_OPTION_COUNT = 0;

// Hotspot System
struct hotspot {
    unsigned short mode;
    unsigned short x;
    unsigned short y;
} ACTIVE_HOTSPOTS[32] = {0};

// =============================================================================
// PLACEHOLDER EXTERNAL FUNCTIONS
// =============================================================================
// These are called by our C code but not yet implemented
// They're stubbed out to prevent linking errors

// External functions our C code calls but aren't implemented yet
void unknown_c0ac20(void) { /* TODO: Implement */ }
void unknown_c12e42(void) { /* TODO: Implement */ }
void unknown_c2db3f(void) { /* TODO: Implement */ } 
void movement_data_read8(void) { /* TODO: Implement */ }
void get_position_of_party_member(void) { /* TODO: Implement */ }
void get_bank_from_ptr(unsigned char* ptr) { (void)ptr; /* TODO: Implement */ }
void change_music(unsigned short track_id) { (void)track_id; /* TODO: Implement */ }
void play_sound_unknown0(void) { /* TODO: Implement */ }
void unknown_c0ac0c(unsigned short param) { (void)param; /* TODO: Implement */ }
void unknown_c0abbd(unsigned short track) { (void)track; /* TODO: Implement */ }
void get_active_window_address(void) { /* TODO: Implement */ }
void irq(void) { /* TODO: Implement */ }
void unknown_c0a1f2(unsigned char index) { (void)index; /* TODO: Implement */ }

// Hardware initialization placeholder
void set_inidisp(unsigned char value) {
    *(volatile unsigned char*)INIDISP = value;
}