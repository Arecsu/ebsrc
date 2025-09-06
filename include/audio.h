#ifndef AUDIO_H
#define AUDIO_H

// Audio system functions

// Music control
void pause_music(void);
void resume_music(void);
void stop_music(void);

// Sound effects
void play_sound(unsigned short sound_id);
void play_sound_and_unknown(unsigned short sound_id);

// SPC700 communication
void wait_for_spc700(void);
void load_spc700_data(unsigned char bank, unsigned short addr);

// Audio configuration
void set_audio_channels(unsigned short stereo_flag);
unsigned short get_audio_bank(unsigned short pack_id);

// Music subsystem initialization
void initialize_music_subsystem(void);

// Music track changing
void change_music(unsigned short track_id);

// Forward declarations for external functions
extern unsigned short unknown_c0ac20(void);

// Music data structures (partial)
typedef struct {
    unsigned char sequence_pack;
} pack_table_entry;

// External data tables
extern pack_table_entry MUSIC_DATASET_TABLE[];
extern unsigned char* MUSIC_PACK_POINTER_TABLE[];

#endif