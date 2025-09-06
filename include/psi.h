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
#define MSG_BTL_PSYCO_ON 0x1240     // "PSI Shield activated"
#define MSG_BTL_PSYCO_ADD 0x1241    // "PSI Shield reinforced"

// Status effect constants
#define STATUS_0_PARALYZED 0x01
#define STATUS_GROUP_PERSISTENT_EASYHEAL 0x01
#define MSG_BTL_SHIBIRE_ON 0x1242   // "Paralyzed!"
#define MSG_BTL_KIKANAI 0x1243      // "No effect"

// Forward declarations for external functions
extern unsigned char CURRENT_TARGET;
extern void display_battle_text_ptr(unsigned short msg_id);
extern unsigned short shields_common(unsigned char target, unsigned char shield_type);
extern unsigned short fail_attack_on_npcs(void);
extern unsigned short flash_immunity_test(void);
extern void weaken_shield(void);
extern unsigned short inflict_status_battle(unsigned char target, unsigned char status_group, unsigned char status_type);

#endif