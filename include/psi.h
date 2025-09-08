#ifndef PSI_H
#define PSI_H

// PSI (Psychic) system functions

// Shield PSI
void btlact_psi_shield_a(void);

// Flash PSI spells
void btlact_psi_flash_a(void);
void flash_inflict_paralysis(void);
void flash_inflict_feeling_strange(void);
void flash_inflict_crying(void);

// PSI constants
#define STATUS_6_PSI_SHIELD 0x02
// All battle messages now use real ROM data from rom_data.h - no more placeholders!

// Status effect constants
// STATUS_0_PARALYZED now defined in battle.h (include battle.h)
#define STATUS_GROUP_PERSISTENT_EASYHEAL 0x01

// Forward declarations for external functions
extern unsigned short CURRENT_TARGET;
extern void display_battle_text_ptr(const unsigned char* msg_ptr);
extern unsigned short shields_common(unsigned char target, unsigned char shield_type);
extern unsigned short fail_attack_on_npcs(void);
extern unsigned short flash_immunity_test(void);
extern void weaken_shield(void);
extern unsigned short inflict_status_battle(unsigned char target, unsigned char status_group, unsigned char status_type);

#endif