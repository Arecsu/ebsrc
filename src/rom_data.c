#include <stddef.h>  // For NULL
#include "hardware.h"
#include "rom_data.h"

// =============================================================================
// ROM DATA ACCESS FUNCTIONS
// =============================================================================

// Text/Message Access Functions
// These functions provide safe access to ROM text data with proper type handling

const char* get_battle_message(const unsigned char* const message_ptr) {
    // For now, simply cast the message pointer to char*
    // In the future, this could handle decompression, encoding, etc.
    if (message_ptr == NULL) {
        return "";  // Safe fallback for null pointers
    }
    
    return (const char*)message_ptr;
}

const char* get_event_message(const unsigned char* const message_ptr) {
    // Event messages may have different encoding or compression
    if (message_ptr == NULL) {
        return "";  // Safe fallback for null pointers
    }
    
    return (const char*)message_ptr;
}

const char* get_global_message(const unsigned char* const message_ptr) {
    // Global messages - system text, menus, etc.
    if (message_ptr == NULL) {
        return "";  // Safe fallback for null pointers  
    }
    
    return (const char*)message_ptr;
}

// Audio Data Access Functions
// These functions provide access to audio packs and music tracks

const unsigned char* get_audio_pack(unsigned char pack_id) {
    // Audio packs are loaded at runtime based on pack_id
    // This is a placeholder - actual implementation would look up
    // the pack_id in the audio pack table and return the data pointer
    
    // For now, return NULL - will be implemented when audio system is converted
    (void)pack_id;  // Suppress unused parameter warning
    return NULL;
}

const unsigned char* get_music_track(unsigned short track_id) {
    // Music tracks are referenced by track_id  
    // This would access the music dataset table to get track data
    
    // For now, return NULL - will be implemented when audio system is converted
    (void)track_id;  // Suppress unused parameter warning
    return NULL;
}

// Graphics Data Access Functions
// These functions provide access to sprites, tilesets, and graphics data

const unsigned char* get_sprite_data(unsigned short sprite_id) {
    // Sprite data access by sprite ID
    // This would look up sprite_id in the sprite table and return graphics data
    
    // For now, return NULL - will be implemented when graphics system is converted
    (void)sprite_id;  // Suppress unused parameter warning
    return NULL;
}

const unsigned char* get_tileset_data(unsigned char tileset_id) {
    // Tileset data access by tileset ID
    // This would look up tileset_id in the tileset table and return tile data
    
    // For now, return NULL - will be implemented when graphics system is converted  
    (void)tileset_id;  // Suppress unused parameter warning
    return NULL;
}

// =============================================================================
// ROM DATA UTILITY FUNCTIONS
// =============================================================================

// Helper function to safely access ROM pointers with bounds checking
const unsigned char* safe_rom_access(const unsigned char* const base_ptr, unsigned short offset) {
    if (base_ptr == NULL) {
        return NULL;
    }
    
    // In the future, this could include ROM bounds checking
    // For now, just return the offset pointer
    return base_ptr + offset;
}

// Helper function to get ROM data length (when header includes size info)
unsigned short get_rom_data_length(const unsigned char* const data_ptr) {
    if (data_ptr == NULL) {
        return 0;
    }
    
    // This is a placeholder - actual implementation would depend on
    // how the ROM data is structured (header format, etc.)
    
    // For now, return 0 - will be implemented based on ROM analysis
    (void)data_ptr;  // Suppress unused parameter warning
    return 0;
}