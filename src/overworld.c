#pragma code-name ("BANK06")

#include "hardware.h"
#include "overworld.h"

// Forward declarations for external functions
extern void unknown_c2db3f(void);
extern unsigned char movement_data_read8(void);
extern void get_position_of_party_member(void);

// Reset mushroomized walking state
void reset_mushroomized_walking(void) {
    MUSHROOMIZED_WALKING_FLAG = 0;
}

// Enable or disable automatic sector music changes
void set_auto_sector_music_changes(unsigned short enable) {
    ENABLE_AUTO_SECTOR_MUSIC_CHANGES = enable;
}

// Actionscript animated background callback
void actionscript_animated_background_callback(void) {
    unknown_c2db3f();
}

// Disable a hotspot by ID
void disable_hotspot(unsigned short id) {
    unsigned short hotspot_index;
    
    hotspot_index = id - 1;
    
    // Clear the hotspot mode
    ACTIVE_HOTSPOTS[hotspot_index].mode = 0;
    
    // Clear the corresponding game state flag
    GAME_STATE.active_hotspot_modes[hotspot_index] = 0;
}

// Clear current entity collision
void clear_current_entity_collision(void) {
    ENTITY_COLLIDED_OBJECTS[CURRENT_ENTITY_INDEX] = ENTITY_COLLISION_NO_OBJECT;
}

// Disable current entity collision
void disable_current_entity_collision(void) {
    ENTITY_COLLIDED_OBJECTS[CURRENT_ENTITY_INDEX] = ENTITY_COLLISION_DISABLED;
}

// Actionscript get position of party member
void actionscript_get_position_of_party_member(void) {
    unsigned char member_id;
    
    member_id = movement_data_read8();
    PARTY_MEMBER_TARGET = member_id;
    get_position_of_party_member();
}

// Change music to next map track
void change_music_5dd6(void) {
    change_music(NEXT_MAP_MUSIC_TRACK);
}

// Set party tick callbacks for entity
void set_party_tick_callbacks(unsigned short entity_id, unsigned short callback1, unsigned short callback2) {
    unsigned short index;
    int i;
    
    index = entity_id * 2;
    
    // Set first callback
    ENTITY_TICK_CALLBACK_LOW[index] = callback1;
    ENTITY_TICK_CALLBACK_HIGH[index] = callback2;
    
    // Set callbacks for next 6 entities (party members)
    for (i = 0; i < 6; i++) {
        index += 2;
        ENTITY_TICK_CALLBACK_LOW[index] = callback1;
        ENTITY_TICK_CALLBACK_HIGH[index] = callback2;
    }
}