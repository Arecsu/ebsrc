// Modern C audio system

#include "hardware.h"
#include "audio.h"

extern void spc700_transfer_setup(void);
extern void spc700_transfer_restore(void);

// Forward declarations for external functions
extern unsigned short unknown_c0ac20(void);
extern void unknown_c12e42(void);
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
        unsigned short pack_index;
        unsigned char pack_id;
        unsigned short pack_offset;
        unsigned char bank;
        unsigned short addr;
        
        pack_index = track_id - 1;
        pack_id = MUSIC_DATASET_TABLE[pack_index].sequence_pack;
        
        // Load primary sample pack if different
        if (pack_id != CURRENT_SEQUENCE_PACK && pack_id != 0xFF) {
            CURRENT_SEQUENCE_PACK = pack_id;
            
            pack_offset = pack_id * 3;
            bank = ((unsigned char*)MUSIC_PACK_POINTER_TABLE)[pack_offset] & 0xFF;
            bank = get_audio_bank(bank);
            
            addr = *((unsigned short*)&((unsigned char*)MUSIC_PACK_POINTER_TABLE)[pack_offset + 1]);
            addr &= SEQUENCE_PACK_MASK;
            
            load_spc700_data(bank, addr);
        }
        
        // Start playback
        unknown_c0abbd(pack_index + 1);
    }
}

// Load data to SPC700 audio processor
void load_spc700_data(unsigned char bank, unsigned short addr) {
    unsigned short spc_data_ptr_low;
    unsigned char spc_data_ptr_bank;
    unsigned short y_index;
    unsigned char temp_a;
    unsigned short temp_x;
    unsigned char nmitimen_backup;
    unsigned short block_size;
    unsigned short target_addr;
    unsigned short data_byte;
    unsigned short i;
    
    spc_data_ptr_low = addr;
    spc_data_ptr_bank = bank;
    y_index = 0;
    
    
    // Save current bank/direct page and set to 0 (65816-specific)
#ifndef TEST_COMPILATION
    spc700_transfer_setup();
#endif
    
    // Check if SPC700 is ready (should be 0xBBAA)
    if (*(volatile unsigned short*)APUIO0 != 0xBBAA) {
        wait_for_spc700();
    }
    
    // Disable NMI during transfer
    nmitimen_backup = NMITIMEN_MIRROR;
    NMITIMEN_MIRROR = nmitimen_backup & 0x7F;
    *(volatile unsigned char*)NMITIMEN = NMITIMEN_MIRROR;
    
    temp_a = 0xCC;
    
    // Main data transfer loop
    while (1) {
        // Read block size from data
        block_size = *((volatile unsigned short*)((unsigned long)spc_data_ptr_bank << 16 | spc_data_ptr_low + y_index));
        y_index += 2;
        
        if (block_size == 0) {
            // End of data - send termination
            temp_x = 0;
            *(volatile unsigned short*)APUIO2 = 0x0500;
        } else {
            // Read target address
            temp_x = block_size;
            target_addr = *((volatile unsigned short*)((unsigned long)spc_data_ptr_bank << 16 | spc_data_ptr_low + y_index));
            y_index += 2;
            *(volatile unsigned short*)APUIO2 = target_addr;
        }
        
        // Set transfer mode in APUIO1
        *(volatile unsigned char*)APUIO1 = (temp_x >= 1) ? 1 : 0;
        temp_a += 0x7F;
        if ((temp_a & 0x80) != 0) temp_a |= 0x80; // Keep bit 7 set
        
        // Start transfer
        *(volatile unsigned char*)APUIO0 = temp_a;
        
        // Wait for SPC700 to acknowledge
        while (*(volatile unsigned char*)APUIO0 != temp_a) {
            // Wait loop
        }
        
        if (block_size == 0) break; // End of data
        
        // Transfer data bytes
        for (i = 0; i < temp_x; i++) {
            data_byte = *((volatile unsigned char*)((unsigned long)spc_data_ptr_bank << 16 | spc_data_ptr_low + y_index));
            y_index++;
            
            // Send byte and wait for acknowledgment
            data_byte <<= 8; // Move to high byte
            temp_a++;
            *(volatile unsigned short*)APUIO0 = (data_byte | temp_a);
            
            // Wait for acknowledgment
            while ((*(volatile unsigned char*)APUIO0) != temp_a) {
                // Wait loop  
            }
        }
        
        // Wait for block transfer completion
        while (*(volatile unsigned char*)APUIO0 == temp_a) {
            // Wait for change
        }
        
        // Check for completion signal
        temp_a += 3;
        while (temp_a == 0) {
            // Wait for non-zero
            temp_a += 3;
        }
    }
    
    // Wait for final completion  
    while (*(volatile unsigned short*)APUIO0 != 0) {
        // Wait for zero
    }
    
    // Restore NMI
    NMITIMEN_MIRROR = nmitimen_backup | 0x80;
    *(volatile unsigned char*)NMITIMEN = NMITIMEN_MIRROR;
    
    // Restore bank/direct page (65816-specific)
#ifndef TEST_COMPILATION
    spc700_transfer_restore();
#endif
}