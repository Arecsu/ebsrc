#include "include/hardware.h"
#include "include/audio.h"

// Forward declarations for external functions
extern unsigned short unknown_c0ac20(void);
extern void unknown_c12e42(void);
extern void load_spc700_data(unsigned char bank, unsigned short addr);
extern unsigned char get_bank_from_ptr(unsigned char* ptr);

// External global variables
extern unsigned char* STEREO_MONO_DATA;
extern unsigned short SEQUENCE_PACK_MASK;
#ifdef JPN
extern unsigned char AUDIO_PACK_108_BANK;
#endif

// Pause music playback
void pause_music(void) {
    DISABLE_HPPP_ROLLING = 1;
}

// Resume music playback  
void resume_music(void) {
    HALF_HPPP_METER_SPEED = 0;
    DISABLE_HPPP_ROLLING = 0;
}

// Stop music playback
void stop_music(void) {
    *(volatile unsigned char*)APUIO0 = 0;
    
    // Wait for APU to acknowledge
    while (unknown_c0ac20() != 0) {
        // Wait loop
    }
    
    CURRENT_MUSIC_TRACK = 0xFFFF;
}

// Play sound effect
void play_sound(unsigned short sound_id) {
    unsigned char queue_index;
    
    if (sound_id == 0) {
        // Special case - play sound 0x57 to APUIO3
        *(volatile unsigned char*)APUIO3 = 0x57;
        return;
    }
    
    queue_index = SOUND_EFFECT_QUEUE_END_INDEX;
    SOUND_EFFECT_QUEUE[queue_index] = (unsigned char)(sound_id | SOUND_EFFECT_UPPER_BIT_FLIPPER);
    
    // Update queue index (wrap at 8)
    SOUND_EFFECT_QUEUE_END_INDEX = (queue_index + 1) & 0x07;
    
    // Flip the upper bit flipper
    SOUND_EFFECT_UPPER_BIT_FLIPPER ^= 0x80;
}

// Wait for SPC700 to be ready
void wait_for_spc700(void) {
    *(volatile unsigned short*)APUIO2 = 0;
    *(volatile unsigned short*)APUIO0 = 0;
    
    // Wait for SPC700 ready signal
    do {
        *(volatile unsigned short*)APUIO0 = 0x00FF;
    } while (*(volatile unsigned short*)APUIO0 != 0xBBAA);
}

// Play sound and call additional processing
void play_sound_and_unknown(unsigned short sound_id) {
    play_sound(sound_id);
    unknown_c12e42();
}

// Set audio channels (stereo/mono)
void set_audio_channels(unsigned short stereo_flag) {
    unsigned char* data_ptr;
    
    if (stereo_flag == 0) {
        data_ptr = STEREO_MONO_DATA;  // Mono data
    } else {
        data_ptr = STEREO_MONO_DATA + 7;  // Stereo data offset
    }
    
    load_spc700_data(get_bank_from_ptr(data_ptr), (unsigned short)data_ptr);
}

// Get audio bank for pack loading
unsigned short get_audio_bank(unsigned short pack_id) {
    SEQUENCE_PACK_MASK = 0xFFFF;  // Set to -1
    
#ifdef JPN
    // Mother 2's audio pack addresses are relative to first bank
    return pack_id + AUDIO_PACK_108_BANK;
#else
    return pack_id;
#endif
}

// Initialize music subsystem
void initialize_music_subsystem(void) {
    unsigned short pack_id;
    unsigned short pack_offset;
    unsigned char bank;
    unsigned short addr;
    
    // Reset sequence and sample packs
    CURRENT_SEQUENCE_PACK = 0xFFFF;
    CURRENT_PRIMARY_SAMPLE_PACK = 0xFFFF;
    
    // Get sequence pack from music dataset table
    pack_id = MUSIC_DATASET_TABLE[0].sequence_pack & 0x00FF;
    UNKNOWN_7EB543 = pack_id;
    CURRENT_SECONDARY_SAMPLE_PACK = pack_id;
    
    // Calculate pack pointer table offset  
    pack_offset = pack_id * 3; // sizeof(music_pack_pointer) = 3 bytes
    
    // Get bank from pack pointer table
    bank = ((unsigned char*)MUSIC_PACK_POINTER_TABLE)[pack_offset] & 0x00FF;
    bank = get_audio_bank(bank);
    
    // Get address from pack pointer table and apply mask
    addr = *((unsigned short*)&((unsigned char*)MUSIC_PACK_POINTER_TABLE)[pack_offset + 1]);
    addr &= SEQUENCE_PACK_MASK;
    
    // Load SPC700 data
    load_spc700_data(bank, addr);
    
    // Enable automatic sector music changes
    ENABLE_AUTO_SECTOR_MUSIC_CHANGES = 1;
}

// Change music track (simplified implementation)
void change_music(unsigned short track_id) {
    extern void play_sound_unknown0(void);
    extern void unknown_c0ac0c(unsigned short param);
    extern void unknown_c0abbd(unsigned short track);
    
    // Don't change if it's already playing
    if (track_id == CURRENT_MUSIC_TRACK) {
        return;
    }
    
    // Play transition sound if enabled
    if (DISABLED_TRANSITIONS == 0) {
        play_sound_unknown0();
    }
    
    // Stop current music for most tracks
    // Sound stone recordings (tracks >= certain ID) should play seamlessly
    if (track_id < 100) {  // Simplified - real game has specific ranges
        unknown_c0ac0c(1);
        stop_music();
    }
    
    // Set new track
    CURRENT_MUSIC_TRACK = track_id;
    
    // Load music data packs (simplified)
    if (track_id > 0) {
        unsigned short pack_index = track_id - 1;
        unsigned char pack_id = MUSIC_DATASET_TABLE[pack_index].sequence_pack;
        
        // Load primary sample pack if different
        if (pack_id != CURRENT_SEQUENCE_PACK && pack_id != 0xFF) {
            CURRENT_SEQUENCE_PACK = pack_id;
            
            unsigned short pack_offset = pack_id * 3;
            unsigned char bank = ((unsigned char*)MUSIC_PACK_POINTER_TABLE)[pack_offset] & 0xFF;
            bank = get_audio_bank(bank);
            
            unsigned short addr = *((unsigned short*)&((unsigned char*)MUSIC_PACK_POINTER_TABLE)[pack_offset + 1]);
            addr &= SEQUENCE_PACK_MASK;
            
            load_spc700_data(bank, addr);
        }
        
        // Start playback
        unknown_c0abbd(pack_index + 1);
    }
}