#ifdef __CC65__
// Modern C system
#endif

#include "hardware.h"
#include "structs.h"

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
unsigned short CURRENT_SEQUENCE_PACK = 0xFFFF;
unsigned short CURRENT_PRIMARY_SAMPLE_PACK = 0xFFFF;
unsigned short CURRENT_SECONDARY_SAMPLE_PACK = 0xFFFF;

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

// DMA System Variables
unsigned char DMA_QUEUE_INDEX = 0;
unsigned char LAST_COMPLETED_DMA_INDEX = 0;

// Game State Variables (minimal)
game_state GAME_STATE;

// Party and Character Data (minimal)
char_struct PARTY_CHARACTERS[4]; // 4 party members

// Battle System Data
battler BATTLERS_TABLE[32]; // Battle participants

// Input/Controls
unsigned short PAD_RAW = 0;
unsigned short PAD_RAW_2 = 0;
unsigned short JOYPAD_1_DATA = 0;
unsigned short JOYPAD_2_DATA = 0;

// Entity/NPC System (minimal)
unsigned short CURRENT_ENTITY_INDEX = 0;
unsigned short ENTITY_COLLIDED_OBJECTS[256];

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
window_stats WINDOW_STATS[32]; // Match header declaration

unsigned short OPEN_WINDOW_TABLE[32];

// Sprite/OAM System
oam_entry OAM1[128], OAM2[128];

unsigned char OAM1_HIGH_TABLE[32];
unsigned char OAM2_HIGH_TABLE[32];

unsigned short NEXT_FRAME_BUF_ID = 0;
unsigned short OAM_ADDR = 0;
unsigned short OAM_END_ADDR = 0;
unsigned short OAM_HIGH_TABLE_ADDR = 0;
unsigned char OAM_HIGH_TABLE_BUFFER = 0;

unsigned short PRIORITY_0_SPRITE_OFFSET = 0;
unsigned short PRIORITY_1_SPRITE_OFFSET = 0;
unsigned short PRIORITY_2_SPRITE_OFFSET = 0;
unsigned short PRIORITY_3_SPRITE_OFFSET = 0;

unsigned char SPRITE_VRAM_TABLE[88];

// Inventory System
unsigned char ITEM_CONFIGURATION_TABLE[256]; // Item data table

// Memory Test Variables
unsigned char LAST_SRAM_BANK = 0;

// Fade System
fade_parameters FADE_PARAMETERS;

unsigned char FADE_DELAY_FRAMES_LEFT = 0;

// Palette Animation
overworld_palette_anim OVERWORLD_PALETTE_ANIM;

// Audio Data Pointers (will be set by ROM data extraction)
const unsigned char* STEREO_MONO_DATA = (const unsigned char*)0x808000; // Safe ROM address
const unsigned char* MUSIC_DATASET_TABLE = (const unsigned char*)0x808100;
const unsigned char* MUSIC_PACK_POINTER_TABLE = (const unsigned char*)0x808200;

// ROM data messages will be provided by data extraction system

// Audio State Variables  
unsigned short SEQUENCE_PACK_MASK = 0xFFFF;
unsigned short UNKNOWN_7EB543 = 0;
unsigned short DISABLED_TRANSITIONS = 0;

// Additional Battle Variables (basic)
unsigned char BATTLE_MENU_CURRENT_OPTION = 0;
unsigned char BATTLE_MENU_OPTION_COUNT = 0;
unsigned short ITEM_DROPPED = 0;
unsigned char MIRROR_ENEMY = 0;

// Hotspot System
active_hotspot ACTIVE_HOTSPOTS[32];

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
// change_music is implemented in audio.c
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