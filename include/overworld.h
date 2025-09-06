#ifndef OVERWORLD_H
#define OVERWORLD_H

// Overworld movement and interaction functions

// Movement state functions
void reset_mushroomized_walking(void);
void set_auto_sector_music_changes(unsigned short enable);

// Actionscript functions
void actionscript_animated_background_callback(void);

// Hotspot functions
void disable_hotspot(unsigned short id);

// Entity collision functions
void clear_current_entity_collision(void);
void disable_current_entity_collision(void);

// Actionscript functions
void actionscript_get_position_of_party_member(void);

// Position adjustment functions (complex - TBD)
// long adjust_position_horizontal(short direction, short flags, long position);  
// long adjust_position_vertical(short direction, short flags, long position);

// Music control functions
void change_music_5dd6(void);
void set_party_tick_callbacks(unsigned short entity_id, unsigned short callback1, unsigned short callback2);

// Forward declarations for external functions
extern void unknown_c2db3f(void);
extern void change_music(unsigned short track_id);
extern unsigned short NEXT_MAP_MUSIC_TRACK;

#endif