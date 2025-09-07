#pragma code-name ("BANK05")

#include "hardware.h"
#include "inventory.h"

// Forward declarations for external functions
extern unsigned short mult168(unsigned short a, unsigned short b);

// Get item subtype classification (4 categories)
unsigned short get_item_subtype(unsigned short item_id) {
    unsigned short offset = mult168(item_id, ITEM_CONFIG_SIZE);
    unsigned char item_flags = ITEM_CONFIGURATION_TABLE[offset + ITEM_TYPE_OFFSET];
    
    // Extract bits 2-3 (mask 0x0C) for subtype classification
    unsigned short subtype_bits = item_flags & 0x0C;
    
    switch (subtype_bits) {
        case 0x00: return 1;  // Category 1
        case 0x04: return 2;  // Category 2  
        case 0x08: return 3;  // Category 3
        case 0x0C: return 4;  // Category 4
        default: return 0;    // Unknown/invalid
    }
}

// Get item subtype classification variant (3 categories)
unsigned short get_item_subtype_2(unsigned short item_id) {
    unsigned short offset = mult168(item_id, ITEM_CONFIG_SIZE);
    unsigned char item_flags = ITEM_CONFIGURATION_TABLE[offset + ITEM_TYPE_OFFSET];
    
    // Extract bits 2-3 (mask 0x0C) for subtype classification
    unsigned short subtype_bits = item_flags & 0x0C;
    
    switch (subtype_bits) {
        case 0x00:
        case 0x0C: return 1;  // Category 1 (combined 0x00 and 0x0C)
        case 0x04: return 2;  // Category 2
        case 0x08: return 3;  // Category 3
        default: return 0;    // Unknown/invalid
    }
}