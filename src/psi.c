#include "include/hardware.h"
#include "include/battle.h"
#include "include/psi.h"
#include "include/rom_data.h"

// Forward declarations for external functions
extern unsigned short shields_common(unsigned char target, unsigned char shield_type);

// PSI Shield Alpha - provides PSI protection
void btlact_psi_shield_a(void) {
    unsigned short result = shields_common(CURRENT_TARGET, STATUS_6_PSI_SHIELD);
    
    if (result == 0) {
        display_battle_text_ptr(MSG_BTL_PSYCO_ON);   // "PSI Shield activated"
    } else {
        display_battle_text_ptr(MSG_BTL_PSYCO_ADD);  // "PSI Shield reinforced"
    }
}

// PSI Flash Alpha - causes status effects
void btlact_psi_flash_a(void) {
    if (fail_attack_on_npcs() != 0) {
        return;  // Attack failed on NPCs
    }
    
    if (flash_immunity_test() == 0) {
        return;  // Target is immune
    }
    
    // 1/8 chance to inflict "feeling strange", otherwise "crying"
    if ((rand() & 0x0007) == 0) {
        flash_inflict_feeling_strange();
    } else {
        flash_inflict_crying();
    }
    
    weaken_shield();
}

// Flash inflict paralysis - specific status effect for flash attacks
void flash_inflict_paralysis(void) {
    unsigned char result = inflict_status_battle(CURRENT_TARGET, STATUS_GROUP_PERSISTENT_EASYHEAL, STATUS_0_PARALYZED);
    
    if (result != 0) {
        display_battle_text_ptr(MSG_BTL_SHIBIRE_ON);  // "Paralyzed!"
    } else {
        display_battle_text_ptr(MSG_BTL_KIKANAI);     // "Had no effect"
    }
}

// Inflict "feeling strange" status effect  
void flash_inflict_feeling_strange(void) {
    // Placeholder - specific status constants needed
    display_battle_text_ptr(MSG_BTL_HEN_ON);
}

// Inflict "crying" status effect
void flash_inflict_crying(void) {
    if (inflict_status_battle(CURRENT_TARGET, STATUS_2_CRYING, STATUS_2_CRYING) != 0) {
        display_battle_text_ptr(MSG_BTL_NAMIDA_ON);
    } else {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
    }
}