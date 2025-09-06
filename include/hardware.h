#ifndef HARDWARE_H
#define HARDWARE_H

#include "include/structs.h"

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

// Text system variables
extern unsigned char INSTANT_PRINTING;
extern unsigned char TEXT_PROMPT_WAITING_FOR_INPUT;
extern unsigned char BLINKING_TRIANGLE_FLAG;
extern unsigned short WINDOW_FOCUS;

// Forward declarations for external functions
extern void set_inidisp(unsigned char value);
extern int rand(void);

#endif