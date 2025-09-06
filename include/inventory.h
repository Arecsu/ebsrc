#ifndef INVENTORY_H
#define INVENTORY_H

// Inventory and item management functions

// Item classification functions
unsigned short get_item_subtype(unsigned short item_id);
unsigned short get_item_subtype_2(unsigned short item_id);

// Item configuration structure (partial)
typedef struct {
    unsigned char type;
    // Additional fields TBD
} item_config;

// Constants
#define ITEM_TYPE_OFFSET 0
#define ITEM_CONFIG_SIZE 16  // Size TBD - placeholder

// External global variables
extern unsigned char ITEM_CONFIGURATION_TABLE[];

// Forward declarations for external functions
extern unsigned short mult168(unsigned short a, unsigned short b);

#endif