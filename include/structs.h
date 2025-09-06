#ifndef STRUCTS_H
#define STRUCTS_H

// Fixed point number
typedef struct {
    unsigned short fraction;  // 0
    unsigned short integer;   // 2
} fixed_point;

// 2D coordinates
typedef struct {
    unsigned short x_coord;   // 0
    unsigned short y_coord;   // 2
} coords;

#define AFFLICTION_GROUP_COUNT 7

// Character structure
typedef struct {
#ifdef JPN
    unsigned char name[4];    // 0
#else
    unsigned char name[5];    // 0
#endif
    unsigned char level;      // 5
    unsigned long exp;        // 6
    unsigned short max_hp;    // 10
    unsigned short max_pp;    // 12
    unsigned char afflictions[AFFLICTION_GROUP_COUNT]; // 14
    unsigned char offense;    // 21
    unsigned char defense;    // 22
    unsigned char speed;      // 23
    unsigned char guts;       // 24
    unsigned char luck;       // 25
    unsigned char vitality;   // 26
    unsigned char iq;         // 27
    unsigned char base_offense;  // 28
    unsigned char base_defense;  // 29
    unsigned char base_speed;    // 30
    unsigned char base_guts;     // 31
    unsigned char base_luck;     // 32
    unsigned char base_vitality; // 33
    unsigned char base_iq;       // 34
    unsigned char fire_resist;   // 35
    unsigned char freeze_resist; // 36
    unsigned char flash_resist;  // 37
    unsigned char paralysis_resist; // 38
    unsigned char hypnosis_brainshock_resist; // 39
    unsigned char items[14];     // 40
    unsigned char equipment[4];  // 54
    unsigned short unknown58;    // 58
    unsigned short unknown60;    // 60
    unsigned short unknown62;    // 62
    unsigned short unknown64;    // 64
    unsigned short position_index; // 66
    unsigned short unknown68;    // 68
    unsigned short unknown70;    // 70
    unsigned short current_hp_fraction; // 72
    unsigned short current_hp;   // 74
} char_struct;

// Equipment slot constants
typedef enum {
    WEAPON = 0,
    BODY = 1,
    ARMS = 2,
    OTHER = 3
} EQUIPMENT_SLOT;

// Active hotspot structure
typedef struct {
    unsigned short mode;
    // Additional hotspot fields TBD
} active_hotspot;

// Global game state structure (partial)
typedef struct {
    unsigned long money_carried;
    unsigned long bank_balance;
    unsigned char party_status;
    unsigned char walking_style;
    unsigned char active_hotspot_modes[32];  // Array size TBD
} game_state;

// Constants
#define FALSE 0
#define TRUE 1
#define ATM_ACCOUNT_LIMIT 999999
#define ENTITY_COLLISION_NO_OBJECT 0
#define ENTITY_COLLISION_DISABLED 0xFF

// External global variables
extern char_struct PARTY_CHARACTERS[4];
extern game_state GAME_STATE;
extern unsigned char __BSS_START__[];
extern active_hotspot ACTIVE_HOTSPOTS[32];  // Array size TBD
extern unsigned short ENTITY_COLLIDED_OBJECTS[256];  // Array size TBD
extern unsigned short CURRENT_ENTITY_INDEX;
extern unsigned char PARTY_MEMBER_TARGET;

// Battle system structures
typedef struct {
    unsigned char id;
    unsigned short hp;           // Current HP (renamed from hp_current)
    unsigned short hp_max;
    unsigned short pp_current;
    unsigned short pp_max;
    unsigned short pp_target;
    unsigned short hp_target;    // Added missing field
    unsigned short offense;
    unsigned short defense;
    unsigned char fire_resist;
    unsigned char freeze_resist; // Added missing field
    unsigned char flash_resist;  // Added missing field
    unsigned char paralysis_resist; // Added missing field
    unsigned char hypnosis_resist;  // Added missing field
    unsigned char brainshock_resist; // Added missing field
    unsigned char ally_or_enemy; // Added missing field (0=ally, 1=enemy)
    unsigned char npc_id;        // Added missing field
    unsigned char current_action_argument; // Added missing field
    unsigned char row;           // Party member row/index
    unsigned char base_offense;  // Base stats without equipment bonuses
    unsigned char base_defense;
    unsigned char base_speed;
    unsigned char base_luck;
    unsigned char speed;         // Current stats with equipment
    unsigned char luck;
    unsigned char action_item_slot; // Item slot being used in action
    unsigned char guts;          // Current guts stat
    unsigned char base_guts;     // Base guts without equipment
    // More fields TBD
} battler;

#endif