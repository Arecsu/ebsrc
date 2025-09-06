#include "include/hardware.h"
#include "include/battle.h"

// Forward declarations for external functions
extern unsigned short twenty_five_percent_variance(unsigned short value);
extern void recover_hp(unsigned char target, unsigned short amount);
extern void recover_pp(unsigned char target, unsigned short amount);
extern unsigned short rand_limit(unsigned short limit);
extern void calc_resist_damage(unsigned short damage, unsigned char resistance);
extern void btlact_hp_recovery_1d4(void);
extern unsigned short miss_calc(unsigned short flags);
extern unsigned short smaaaash(void);
extern unsigned short determine_dodge(void);
extern void heal_strangeness(void);
extern void display_battle_text_ptr(unsigned short msg_id);
extern void btlact_level_2_atk(void);
extern void psi_fire_common(unsigned short damage);
extern void bottle_rocket_common(unsigned short count);
extern void btlact_healing_a(void);
extern unsigned char CURRENT_TARGET;
extern unsigned char CURRENT_ATTACKER;

// No need to redefine battler struct - it's in structs.h

extern battler* get_battler(unsigned char target);

// Party member constants
#define PARTY_MEMBER_POO 4

// Battle message constants
#define MSG_BTL_TATAKU_YOKETA 0x1234  // "Attack was dodged" message ID (placeholder)
#define MSG_BTL_MODOKU_OFF 0x1235     // "Poison removed" message
#define MSG_BTL_KIMOCHI_OFF 0x1236    // "Nausea removed" message  
#define MSG_BTL_NAMIDA_OFF 0x1237     // "Crying stopped" message
#define MSG_BTL_HEN_OFF 0x1238        // "Strangeness removed" message

// PSI and item constants
#define FIRE_BETA_DAMAGE 200
#define BOTTLE_ROCKET_COUNT 3
#define BIG_BOTTLE_ROCKET_COUNT 6

// Status group constants  
#define STATUS_GROUP_PERSISTENT_EASYHEAL 0
#define STATUS_GROUP_TEMPORARY 2
#define STATUS_GROUP_STRANGENESS 3

// Status effect constants
#define STATUS_0_POISONED 1
#define STATUS_0_NAUSEOUS 2
#define STATUS_2_CRYING 1
#define STATUS_3_STRANGE 1

// Null battle action - does nothing
void btlact_null(void) {
    // Empty function - no operation
}

// HP recovery functions
void btlact_hp_recovery_50(void) {
    unsigned short recovery_amount = twenty_five_percent_variance(50);
    recover_hp(CURRENT_TARGET, recovery_amount);
}

void btlact_hp_recovery_100(void) {
    unsigned short recovery_amount = twenty_five_percent_variance(100);
    recover_hp(CURRENT_TARGET, recovery_amount);
}

void btlact_hp_recovery_200(void) {
    unsigned short recovery_amount = twenty_five_percent_variance(200);
    recover_hp(CURRENT_TARGET, recovery_amount);
}

void btlact_hp_recovery_300(void) {
    unsigned short recovery_amount = twenty_five_percent_variance(300);
    recover_hp(CURRENT_TARGET, recovery_amount);
}

void btlact_hp_recovery_10(void) {
    unsigned short recovery_amount = twenty_five_percent_variance(10);
    recover_hp(CURRENT_TARGET, recovery_amount);
}

// PP recovery functions
void btlact_pp_recovery_20(void) {
    unsigned short recovery_amount = twenty_five_percent_variance(20);
    recover_pp(CURRENT_TARGET, recovery_amount);
}

void btlact_pp_recovery_80(void) {
    unsigned short recovery_amount = twenty_five_percent_variance(80);
    recover_pp(CURRENT_TARGET, recovery_amount);
}

// Additional null battle actions
void btlact_null2(void) {
    // Empty function - no operation
}

void btlact_null3(void) {
    // Empty function - no operation
}

// HP recovery with dice roll (1d4)
void btlact_hp_recovery_1d4(void) {
    unsigned short recovery_amount = rand_limit(4) + 1;
    recover_hp(CURRENT_TARGET, recovery_amount);
}

// Special HP recovery for Poo (10000) or fallback to 1d4
void btlact_hp_recovery_10000(void) {
    battler* current_battler = get_battler(CURRENT_TARGET);
    
    if (current_battler->id == PARTY_MEMBER_POO) {
        recover_hp(CURRENT_TARGET, 10000);
    } else {
        btlact_hp_recovery_1d4();
    }
}

// Fire damage attack with resistance calculation
void btlact_350_fire_damage(void) {
    unsigned short damage = twenty_five_percent_variance(350);
    battler* current_battler = get_battler(CURRENT_TARGET);
    
    calc_resist_damage(damage, current_battler->fire_resist);
}

// Bash attack action
void btlact_bash(void) {
    if (miss_calc(0) != 0) {
        return;  // Attack missed
    }
    
    if (smaaaash() != 0) {
        return;  // Critical hit handled elsewhere
    }
    
    if (determine_dodge() != 0) {
        display_battle_text_ptr(MSG_BTL_TATAKU_YOKETA);  // "Attack was dodged"
        return;
    }
    
    // Execute level 2 attack
    btlact_level_2_atk();
    heal_strangeness();
}

// Level 1 attack - basic physical attack
void btlact_level_1_atk(void) {
    if (miss_calc(0) != 0) {
        return;  // Attack missed
    }
    
    if (smaaaash() != 0) {
        return;  // Critical hit handled elsewhere
    }
    
    if (determine_dodge() != 0) {
        display_battle_text_ptr(MSG_BTL_TATAKU_YOKETA);  // "Attack was dodged"
        return;
    }
    
    // Calculate damage: attacker offense - target defense
    battler* attacker = get_battler(CURRENT_ATTACKER);
    battler* target = get_battler(CURRENT_TARGET);
    
    short damage = attacker->offense - target->defense;
    
    if (damage > 0) {
        damage = twenty_five_percent_variance(damage);
    }
    
    if (damage < 1) {
        damage = 1;  // Minimum damage
    }
    
    calc_resist_damage(damage, 0xFF);  // No special resistance
    heal_strangeness();
}

// Level 2 attack - stronger physical attack
void btlact_level_2_atk(void) {
    // Calculate damage: (attacker offense * 2) - target defense
    battler* attacker = get_battler(CURRENT_ATTACKER);
    battler* target = get_battler(CURRENT_TARGET);
    
    short damage = (attacker->offense * 2) - target->defense;
    
    if (damage > 0) {
        damage = twenty_five_percent_variance(damage);
    }
    
    if (damage < 1) {
        damage = 1;  // Minimum damage
    }
    
    calc_resist_damage(damage, 0xFF);  // No special resistance
}

// Additional null battle actions
void btlact_null4(void) {
    // Empty function - no operation
}

// PSI Fire Beta - fire damage spell
void btlact_psi_fire_b(void) {
    psi_fire_common(FIRE_BETA_DAMAGE);
}

// Bottle rocket item attack
void btlact_bottle_rocket(void) {
    bottle_rocket_common(BOTTLE_ROCKET_COUNT);
}

// Big bottle rocket item attack  
void btlact_big_bottle_rocket(void) {
    bottle_rocket_common(BIG_BOTTLE_ROCKET_COUNT);
}

// Healing Beta PSI - cures status ailments
void btlact_healing_b(void) {
    battler* target = get_battler(CURRENT_TARGET);
    unsigned char* persistent_status = &((unsigned char*)target)[sizeof(battler) + STATUS_GROUP_PERSISTENT_EASYHEAL];
    unsigned char* temp_status = &((unsigned char*)target)[sizeof(battler) + STATUS_GROUP_TEMPORARY];
    unsigned char* strange_status = &((unsigned char*)target)[sizeof(battler) + STATUS_GROUP_STRANGENESS];
    
    if (*persistent_status == STATUS_0_POISONED) {
        *persistent_status = 0;
        display_battle_text_ptr(MSG_BTL_MODOKU_OFF);
    } else if (*persistent_status == STATUS_0_NAUSEOUS) {
        *persistent_status = 0;
        display_battle_text_ptr(MSG_BTL_KIMOCHI_OFF);
    } else if (*temp_status == STATUS_2_CRYING) {
        *temp_status = 0;
        display_battle_text_ptr(MSG_BTL_NAMIDA_OFF);
    } else if (*strange_status == STATUS_3_STRANGE) {
        *strange_status = 0;
        display_battle_text_ptr(MSG_BTL_HEN_OFF);
    } else {
        btlact_healing_a();  // Fall back to basic healing
    }
}

// Reduce PP battle action
void btlact_reduce_pp(void) {
    battler* target = get_battler(CURRENT_TARGET);
    
    if (target->pp_target == 0) {
        display_battle_text_ptr(0x1240);  // MSG_BTL_PPSUCK_ZERO placeholder
        return;
    }
    
    unsigned short pp_drain = target->pp_max >> 4;  // Divide by 16
    if (pp_drain == 0) {
        display_battle_text_ptr(0x1241);  // MSG_BTL_KIKANAI placeholder
        return;
    }
    
    pp_drain = fifty_percent_variance(pp_drain);
    reduce_pp(CURRENT_TARGET, pp_drain);
    display_text_wait("PP reduced by %d", pp_drain);  // Simplified for now
}

// Call for help battle action
void btlact_call_for_help(void) {
    call_for_help_common(0);
}

// Pray warm battle action (heals 1/8 max HP)
void btlact_pray_warm(void) {
    battler* target = get_battler(CURRENT_TARGET);
    unsigned short heal_amount = target->hp_max >> 3;  // Divide by 8
    recover_hp(CURRENT_TARGET, heal_amount);
}