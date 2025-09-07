#ifndef HARDWARE_H
#define HARDWARE_H

#include "structs.h"

// SNES hardware addresses and constants
#define INIDISP     0x2100
#define OBSEL       0x2101
#define OAMADDL     0x2102
#define OAMADDH     0x2103
#define BG2SC       0x2109
#define BG12NBA     0x210B
#define CGWSEL      0x2130
#define CGADSUB     0x2131
#define APUIO0      0x2140
#define APUIO1      0x2141
#define APUIO2      0x2142
#define APUIO3      0x2143
#define NMITIMEN    0x4200

// Processor flags for 65816
#define PROC_CARRY              0x01
#define PROC_ZERO               0x02
#define PROC_INTERRUPT_DISABLE  0x04
#define PROC_DECIMAL            0x08
#define PROC_INDEX8             0x10
#define PROC_ACCUM8             0x20
#define PROC_OVERFLOW           0x40
#define PROC_NEGATIVE           0x80

// OAM entry structure
typedef struct {
    unsigned char x_coord;
    unsigned char y_coord;
    unsigned char starting_tile;
    unsigned char flags;
} oam_entry;

// Global variables (defined elsewhere)
extern unsigned char DISABLE_HPPP_ROLLING;
extern unsigned char HALF_HPPP_METER_SPEED;
extern unsigned short CURRENT_MUSIC_TRACK;
extern unsigned char SOUND_EFFECT_QUEUE_END_INDEX;
extern unsigned char SOUND_EFFECT_UPPER_BIT_FLIPPER;
extern unsigned char SOUND_EFFECT_QUEUE[8];

// DMA and graphics variables
extern unsigned char DMA_QUEUE_INDEX;
extern unsigned char LAST_COMPLETED_DMA_INDEX;
extern unsigned char BG2SC_MIRROR;
extern unsigned char BG12NBA_MIRROR;
extern unsigned short BG2_X_POS;
extern unsigned short BG2_Y_POS;
extern unsigned short BG3_X_POS;
extern unsigned short BG3_Y_POS;  
extern unsigned short BG4_X_POS;
extern unsigned short BG4_Y_POS;
extern unsigned char BG1SC_MIRROR;
extern unsigned char BG3SC_MIRROR;
extern unsigned char BG4SC_MIRROR;
extern unsigned char BG12NBA_MIRROR;
extern unsigned char BG34NBA_MIRROR;
extern unsigned short BG1_X_POS;
extern unsigned short BG1_Y_POS;

// Overworld variables
extern unsigned char MUSHROOMIZED_WALKING_FLAG;
extern unsigned short ENABLE_AUTO_SECTOR_MUSIC_CHANGES;

// Music system variables
extern unsigned short CURRENT_SEQUENCE_PACK;
extern unsigned short CURRENT_PRIMARY_SAMPLE_PACK;
extern unsigned short CURRENT_SECONDARY_SAMPLE_PACK;
extern unsigned short UNKNOWN_7EB543;
extern unsigned short SEQUENCE_PACK_MASK;
extern unsigned short NEXT_MAP_MUSIC_TRACK;
extern unsigned short DISABLED_TRANSITIONS;

// Entity callback arrays
extern unsigned short ENTITY_TICK_CALLBACK_LOW[64];  
extern unsigned short ENTITY_TICK_CALLBACK_HIGH[64];

// Text and window system variables
extern unsigned short TEXT_SOUND_MODE;
extern unsigned short CURRENT_FOCUS_WINDOW;
extern unsigned short OPEN_WINDOW_TABLE[32];  // Array size TBD

// Window system structures (TBD - need proper definitions)
typedef struct {
    unsigned short id;
    unsigned short x;
    unsigned short y; 
    unsigned short width;
    unsigned short height;
    unsigned short font;
    unsigned short next;
    unsigned short prev;
    unsigned long working_memory;     // Added missing field
    unsigned char secondary_memory;   // Added missing field
    // More fields TBD
} window_stats;

extern window_stats WINDOW_STATS[32];  // Array size TBD

// IRQ and system variables
extern unsigned short IRQ_CALLBACK;
extern unsigned short DEFAULT_IRQ_CALLBACK;
extern unsigned char NMITIMEN_MIRROR;

// Graphics/Display Variables
extern unsigned char BG1SC_MIRROR;
extern unsigned char BG2SC_MIRROR;
extern unsigned char BG3SC_MIRROR;
extern unsigned char BG4SC_MIRROR;
extern unsigned char BG12NBA_MIRROR;
extern unsigned char BG34NBA_MIRROR;

extern unsigned short BG1_X_POS;
extern unsigned short BG1_Y_POS;
extern unsigned short BG2_X_POS;
extern unsigned short BG2_Y_POS;
extern unsigned short BG3_X_POS;
extern unsigned short BG3_Y_POS;
extern unsigned short BG4_X_POS;
extern unsigned short BG4_Y_POS;

// Color averaging
extern unsigned char COLOUR_AVERAGE_RED;
extern unsigned char COLOUR_AVERAGE_GREEN;
extern unsigned char COLOUR_AVERAGE_BLUE;

// Battle System
extern unsigned short CURRENT_ATTACKER;
extern unsigned short CURRENT_TARGET;

// Audio System
extern unsigned short CURRENT_MUSIC_TRACK;
extern unsigned char CURRENT_SEQUENCE_PACK;
extern unsigned char CURRENT_PRIMARY_SAMPLE_PACK;
extern unsigned char CURRENT_SECONDARY_SAMPLE_PACK;
extern unsigned char DISABLE_HPPP_ROLLING;
extern unsigned char HALF_HPPP_METER_SPEED;
extern unsigned char SOUND_EFFECT_QUEUE[8];
extern unsigned char SOUND_EFFECT_QUEUE_END_INDEX;
extern unsigned char SOUND_EFFECT_UPPER_BIT_FLIPPER;

// Input/Controls  
extern unsigned short PAD_RAW;
extern unsigned short PAD_RAW_2;
extern unsigned short JOYPAD_1_DATA;
extern unsigned short JOYPAD_2_DATA;
extern unsigned char LAST_SRAM_BANK;
extern unsigned char SPRITE_VRAM_TABLE[88];

// SRAM test addresses
#define SRAM_SIZE_1_SCRATCH 0x700000
#define SRAM_SIZE_2_SCRATCH 0x710000  
#define SRAM_SIZE_3_SCRATCH 0x720000

// Fade system structures
typedef struct {
    unsigned char step;
    unsigned char delay;
} fade_parameters;

extern fade_parameters FADE_PARAMETERS;
extern unsigned char FADE_DELAY_FRAMES_LEFT;

// Overworld palette animation structure
typedef struct {
    unsigned char timer;
    unsigned char index;
    unsigned char delays[256]; // Animation frame delays
} overworld_palette_anim;

extern overworld_palette_anim OVERWORLD_PALETTE_ANIM;
extern void unknown_c0a1f2(unsigned char index);

// Text system variables
extern unsigned char INSTANT_PRINTING;
extern unsigned char TEXT_PROMPT_WAITING_FOR_INPUT;
extern unsigned char BLINKING_TRIANGLE_FLAG;
extern unsigned short WINDOW_FOCUS;

// OAM and sprite system (using existing oam_entry from line 37)

extern oam_entry OAM1[128];
extern oam_entry OAM2[128];
extern unsigned char OAM1_HIGH_TABLE[32];
extern unsigned char OAM2_HIGH_TABLE[32];
extern unsigned short OAM_ADDR;
extern unsigned short OAM_END_ADDR;
extern unsigned short OAM_HIGH_TABLE_ADDR;
extern unsigned char OAM_HIGH_TABLE_BUFFER;
extern unsigned short PRIORITY_0_SPRITE_OFFSET;
extern unsigned short PRIORITY_1_SPRITE_OFFSET;
extern unsigned short PRIORITY_2_SPRITE_OFFSET;
extern unsigned short PRIORITY_3_SPRITE_OFFSET;
extern unsigned short NEXT_FRAME_BUF_ID;

// Demo/joypad system
extern unsigned short DEMO_RECORDING_FLAGS;
extern unsigned char DEMO_FRAMES_LEFT;
extern void* DEMO_READ_SOURCE;
extern unsigned short PAD_RAW;
extern unsigned short PAD_RAW_2;
extern unsigned short JOYPAD_1_DATA;
extern unsigned short JOYPAD_2_DATA;

#define DEMO_RECORDING_FLAG_PLAYBACK 0x4000

// IRQ system (IRQ_CALLBACK already declared as unsigned short on line 106)
extern void irq(void);

// Save system
typedef struct {
    unsigned char data[32]; // Placeholder
} save_header;

typedef struct {
    save_header header;
    unsigned char game_state[1248]; // Save data
} save_block;

extern void* SAVE_BASE;
extern const char* SRAM_SIGNATURE;

// Color system
extern unsigned short COLOUR_AVERAGE_RED;
extern unsigned short COLOUR_AVERAGE_GREEN;
extern unsigned short COLOUR_AVERAGE_BLUE;

// Forward declarations for external functions
extern void set_inidisp(unsigned char value);
extern int rand(void);
extern void set_bg1_vram_location(unsigned char bg_size, unsigned short tilemap_base, unsigned short tile_base);

#endif