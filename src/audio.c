// Modern SDL2 audio system
#include "platform/audio.h"
#include "hardware.h"
#include "audio.h"
#include <stdio.h>
#include <stdbool.h>

// Audio state tracking
static bool audio_initialized = false;
static bool music_paused = false;

// SDL2-compatible audio functions
void pause_music_system(void) {
    if (audio_initialized) {
        pause_music();
        music_paused = true;
    }
    DISABLE_HPPP_ROLLING = 1;
}

void resume_music_system(void) {
    if (audio_initialized && music_paused) {
        resume_music();
        music_paused = false;
    }
    HALF_HPPP_METER_SPEED = 0;
    DISABLE_HPPP_ROLLING = 0;
}

void stop_music_system(void) {
    if (audio_initialized) {
        stop_music();
    }
    CURRENT_MUSIC_TRACK = 0xFFFF;
}

void play_sound_system(unsigned short sound_id) {
    if (audio_initialized) {
        if (sound_id == 0) {
            // Special case - play default sound
            play_sound_effect(0x57);
            return;
        }
        
        // Queue sound effect through SDL2 system
        queue_sound_effect(sound_id);
        
        // Update queue tracking
        unsigned char queue_index = SOUND_EFFECT_QUEUE_END_INDEX;
        SOUND_EFFECT_QUEUE[queue_index] = (unsigned char)(sound_id | SOUND_EFFECT_UPPER_BIT_FLIPPER);
        SOUND_EFFECT_QUEUE_END_INDEX = (queue_index + 1) & 0x07;
        SOUND_EFFECT_UPPER_BIT_FLIPPER ^= 0x80;
    }
}

void wait_for_audio_ready(void) {
    // SDL2 handles audio initialization synchronously
    // This is a no-op in the PC version
    printf("Audio system ready\n");
}

void play_sound_and_process(unsigned short sound_id) {
    play_sound_system(sound_id);
    // Process any additional audio queue operations
    process_sound_queue();
}

void set_audio_channels(unsigned short stereo_flag) {
    // SDL2 handles stereo/mono automatically based on system configuration
    printf("Audio channels set to %s\n", stereo_flag ? "stereo" : "mono");
    (void)stereo_flag; // Suppress unused parameter warning
}

unsigned short get_audio_bank(unsigned short pack_id) {
    // PC version doesn't need banking - return pack_id directly
    return pack_id;
}

void initialize_music_subsystem(void) {
    if (!audio_init()) {
        printf("Failed to initialize SDL2 audio system\n");
        return;
    }
    
    audio_initialized = true;
    
    // Reset sequence and sample packs (compatibility with original)
    CURRENT_SEQUENCE_PACK = 0xFFFF;
    CURRENT_PRIMARY_SAMPLE_PACK = 0xFFFF;
    
    // For PC version, we don't load specific packs - SDL2 handles this
    UNKNOWN_7EB543 = 0;
    CURRENT_SECONDARY_SAMPLE_PACK = 0;
    
    // Enable automatic sector music changes
    ENABLE_AUTO_SECTOR_MUSIC_CHANGES = 1;
    
    printf("Music subsystem initialized with SDL2\n");
}

void change_music(unsigned short track_id) {
    // Don't change if it's already playing
    if (track_id == CURRENT_MUSIC_TRACK) {
        return;
    }
    
    // For transitions, we could add a brief fade effect here
    if (DISABLED_TRANSITIONS == 0) {
        // Play transition sound effect
        play_sound_system(1); // Generic transition sound
    }
    
    // Stop current music for most tracks
    if (track_id < 100) {
        stop_music_system();
    }
    
    // Set new track
    CURRENT_MUSIC_TRACK = track_id;
    
    // Start new track through SDL2
    if (track_id > 0) {
        // In PC version, we use track_id directly
        play_music(track_id);
        
        // Update pack tracking for compatibility
        CURRENT_SEQUENCE_PACK = track_id;
        printf("Changed to music track %u\n", track_id);
    }
}

// PC-compatible data loading (replaces SNES SPC700 transfer)
void load_audio_data(unsigned short track_id, const char* filename) {
    // In PC version, audio files are loaded as needed by SDL2
    printf("Loading audio data for track %u from %s\n", track_id, filename);
    
    // SDL2 handles the actual file loading internally
    // This function exists for API compatibility
    (void)track_id;
    (void)filename;
}

// Legacy compatibility functions (now use SDL2 internally)
void load_spc700_data(unsigned char bank, unsigned short addr) {
    // Convert SNES bank/address to PC equivalent
    unsigned short track_id = (bank << 8) | (addr >> 8);
    
    printf("Legacy SPC700 call converted: bank=0x%02X addr=0x%04X -> track=%u\n", 
           bank, addr, track_id);
    
    // For now, just track that we attempted to load this
    // Real implementation would map these to actual audio files
    (void)track_id;
}