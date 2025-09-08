// Modern C version - no more banking pragma
#include "hardware.h"
#include "battle.h"
#include "system.h"
#include "rom_data.h"
#include "inventory.h"
#include <stddef.h>
#include <stdbool.h>

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
extern void display_battle_text_ptr(const unsigned char* msg_ptr);
extern void btlact_level_2_atk(void);
extern void btlact_level_3_atk(void);
extern void psi_fire_common(unsigned short damage);
extern void bottle_rocket_common(unsigned short count);
extern void btlact_healing_a(void);
extern void btlact_defense_spray(void);
extern unsigned char fail_attack_on_npcs(void);
extern unsigned char flash_immunity_test(void);
extern void ko_target(unsigned char target);
extern void flash_inflict_paralysis(void);
extern void flash_inflict_feeling_strange(void);
extern void flash_inflict_crying(void);
extern void weaken_shield(void);
extern unsigned char success_speed(unsigned short threshold);
extern unsigned short mult168(unsigned short a, unsigned short b);
extern unsigned char inflict_status_battle(unsigned char target, unsigned char status_group, unsigned char status);
extern unsigned char success_luck80(void);
extern unsigned char get_enemy_type(unsigned short enemy_id);
extern unsigned char shields_common(unsigned char target, unsigned char shield_type);
extern void enable_blinking_triangle(unsigned char enabled);
extern void clear_blinking_prompt(void);
extern unsigned char unknown_c3ee14(unsigned char character_id, unsigned char item_slot);
extern void equip_item(unsigned char character_id, unsigned char item_slot);
extern void calc_psi_dmg_modifiers(unsigned char base_resist);
extern void calc_psi_res_modifiers(unsigned char base_resist);
extern void btlact_psi_shield_a(void);
extern void btlact_psi_shield_b(void);
extern void btlact_hypnosis_a(void);
extern void btlact_brainshock_a(void);
extern void psi_rockin_common(unsigned short damage);
extern void psi_freeze_common(unsigned short damage);
extern void btlact_healing_g(void);
extern void revive_target(unsigned char target, unsigned short hp_amount);
extern void btlact_magnet_a(void);
extern unsigned char success_luck80(void);
extern unsigned char success_255(unsigned char resistance);
extern void hexadecimate_defense(unsigned char target);
extern char_struct* get_character_data(unsigned char row);
extern unsigned char get_calc_result(void);
extern void lifeup_common(unsigned short healing_amount);
extern void btlact_speed_up_1d4(void);
extern void btlact_guts_up_1d4(void);
extern void btlact_vitality_up_1d4(void);
extern void btlact_iq_up_1d4(void);
extern void btlact_luck_up_1d4(void);
extern void btlact_bash(void);
extern void bomb_common(unsigned short damage);
extern unsigned char success_luck40(void);
extern unsigned short get_item_type(unsigned char item_id);
extern void execute_battle_action(unsigned char action_id);
extern void psi_fire_common(unsigned short damage);
extern void psi_rockin_common(unsigned short damage);
extern void reduce_hp(unsigned char target, unsigned short amount);
extern void set_hp(unsigned char target, unsigned short amount);
extern void take_item_from_character(unsigned char character, unsigned short item_id);
// display_text_wait is declared in battle.h
extern unsigned short ITEM_DROPPED;
extern unsigned char MIRROR_ENEMY;
extern unsigned short CURRENT_TARGET;
extern unsigned short CURRENT_ATTACKER;

// No need to redefine battler struct - it's in structs.h

extern battler* get_battler(unsigned char target);

// Party member constants
#define PARTY_MEMBER_POO 4

// All battle messages now use real ROM data from rom_data.h - no more placeholders!

// PSI and item constants
#define FIRE_BETA_DAMAGE 200
#define BOTTLE_ROCKET_COUNT 3
#define BIG_BOTTLE_ROCKET_COUNT 6
#define MUMMY_WRAP_BASE_DAMAGE 180
#define BAG_OF_DRAGONITE_DAMAGE 800

// Status group constants now defined in battle.h

// Status effect constants now defined in battle.h
#define LIFEUP_BETA_HEALING 400
#define LIFEUP_OMEGA_HEALING 400
#define FIRE_ALPHA_DAMAGE 60
#define FIRE_OMEGA_DAMAGE 320
#define ROCKIN_BETA_DAMAGE 240
#define ROCKIN_GAMMA_DAMAGE 320
#define FREEZE_ALPHA_DAMAGE 180
#define MULTI_BOTTLE_ROCKET_COUNT 20
#define PARTY_MEMBER_JEFF 3
// Status constants now defined in battle.h

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
    battler* attacker;
    battler* target;
    short damage;
    
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
    attacker = get_battler(CURRENT_ATTACKER);
    target = get_battler(CURRENT_TARGET);
    
    damage = attacker->offense - target->defense;
    
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
    battler* attacker;
    battler* target;
    short damage;
    
    // Calculate damage: (attacker offense * 2) - target defense
    attacker = get_battler(CURRENT_ATTACKER);
    target = get_battler(CURRENT_TARGET);
    
    damage = (attacker->offense * 2) - target->defense;
    
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
    battler* target;
    unsigned char* persistent_status;
    unsigned char* temp_status;
    unsigned char* strange_status;
    
    target = get_battler(CURRENT_TARGET);
    persistent_status = &((unsigned char*)target)[sizeof(battler) + STATUS_GROUP_PERSISTENT_EASYHEAL];
    temp_status = &((unsigned char*)target)[sizeof(battler) + STATUS_GROUP_TEMPORARY];
    strange_status = &((unsigned char*)target)[sizeof(battler) + STATUS_GROUP_STRANGENESS];
    
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
    battler* target;
    unsigned short pp_drain;
    
    target = get_battler(CURRENT_TARGET);
    
    if (target->pp_target == 0) {
        display_battle_text_ptr(MSG_BTL_PPSUCK_ZERO);
        return;
    }
    
    pp_drain = target->pp_max >> 4;  // Divide by 16
    if (pp_drain == 0) {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
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
    battler* target;
    unsigned short heal_amount;
    
    target = get_battler(CURRENT_TARGET);
    heal_amount = target->hp_max >> 3;  // Divide by 8
    recover_hp(CURRENT_TARGET, heal_amount);
}

// Additional null battle actions
void btlact_null8(void) {
    // Empty function - no operation
}

void btlact_null9(void) {
    // Empty function - no operation  
}

// Defense shower - calls defense spray
void btlact_defense_shower(void) {
    btlact_defense_spray();
}

// Enemy extend - empty action
void btlact_enemyextend(void) {
    // Empty function - no operation
}

// PSI Flash Omega - powerful flash attack with random effects
void btlact_psi_flash_o(void) {
    unsigned char effect;
    
    if (fail_attack_on_npcs() != 0) {
        return;  // Failed on NPCs
    }
    
    if (flash_immunity_test() == 0) {
        return;  // Target is immune
    }
    
    // Random effect (0-7)
    effect = rand_limit(8);
    
    if (effect <= 2) {
        // 3/8 chance to KO target
        ko_target(CURRENT_TARGET);
    } else if (effect == 3) {
        // 1/8 chance to inflict paralysis
        flash_inflict_paralysis();
    } else if (effect == 4) {
        // 1/8 chance to inflict feeling strange
        flash_inflict_feeling_strange();
    } else {
        // 3/8 chance to inflict crying
        flash_inflict_crying();
    }
    
    weaken_shield();
}

// Bag of dragonite - fire damage item
void btlact_bag_of_dragonite(void) {
    unsigned short damage;
    battler* target;
    
    damage = twenty_five_percent_variance(BAG_OF_DRAGONITE_DAMAGE);
    target = get_battler(CURRENT_TARGET);
    calc_resist_damage(damage, target->fire_resist);
}

// Mummy wrap - physical attack that can solidify
void btlact_mummy_wrap(void) {
    battler* target;
    short damage;
    
    if (fail_attack_on_npcs() != 0) {
        return;  // Failed on NPCs
    }
    
    if (success_speed(250) == 0) {
        // Use ROM message data through battle text system
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    target = get_battler(CURRENT_TARGET);
    damage = MUMMY_WRAP_BASE_DAMAGE - target->defense;
    
    if (damage <= 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    calc_resist_damage(damage, 0xFF);  // No special resistance
    
    // Try to inflict solidified status
    inflict_status_battle(CURRENT_TARGET, STATUS_GROUP_TEMPORARY, STATUS_2_SOLIDIFIED);
    display_text_wait(get_battle_message(MSG_BTL_KOORI_ON), 0);
}

// PSI Fire Beta (already implemented as btlact_psi_fire_b, but different name)
void btlact_psi_fire_beta(void) {
    psi_fire_common(FIRE_BETA_DAMAGE);
}

// PSI Shield Alpha redirect - calls main shield function
void redirect_btlact_psi_shield_a(void) {
    btlact_psi_shield_a();
}

// HP Sucker battle action - drains HP from target and heals attacker
void btlact_hp_sucker(void) {
    battler* attacker;
    battler* target;
    unsigned short damage;
    unsigned short new_hp;
    
    if (success_luck80() == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    attacker = get_battler(CURRENT_ATTACKER);
    if (attacker->hp_target == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    if (CURRENT_TARGET == CURRENT_ATTACKER) {
        display_text_wait(get_battle_message(MSG_BTL_HPSUCK_ME), 0);
        return;
    }
    
    target = get_battler(CURRENT_TARGET);
    damage = fifty_percent_variance(target->hp_max) >> 3;  // Divide by 8
    
    reduce_hp(CURRENT_TARGET, damage);
    // Use ROM data access layer - no hardcoded strings
    display_text_wait(get_battle_message(MSG_BTL_HPSUCK_ON), damage);
    
    // Heal attacker
    new_hp = attacker->hp + damage;
    set_hp(CURRENT_ATTACKER, new_hp);
    
    // Check if target was KO'd
    target = get_battler(CURRENT_TARGET);  // Refresh after HP change
    if (target->hp == 0) {
        ko_target(CURRENT_TARGET);
    }
}

// Steal battle action - attempts to steal item
void btlact_steal(void) {
    battler* target;
    battler* attacker;
    unsigned char item_id;
    
    target = get_battler(CURRENT_TARGET);
    
    // Check if target is an enemy (ally_or_enemy == 1)
    if (target->ally_or_enemy != 1) {
        return;  // Can't steal from allies
    }
    
    // Check NPC ID
    if (target->npc_id != 0) {
        return;  // Can't steal from certain NPCs
    }
    
    // Mirror enemy check
    if (MIRROR_ENEMY != 0) {
        battler* attacker = get_battler(CURRENT_ATTACKER);
        if (attacker->ally_or_enemy == 0) {
            // Additional check - if attacker is party member 4 (Poo), allow
            if (attacker->id != 4) {
                return;
            }
        }
    }
    
    // Get item to steal from action argument
    attacker = get_battler(CURRENT_ATTACKER);
    item_id = attacker->current_action_argument & 0xFF;
    
    if (item_id == 0) {
        return;  // No item to steal
    }
    
    // Attempt to steal item
    take_item_from_character(item_id, 0xFF);
}

// Spy battle action - displays enemy stats and resistances  
void btlact_spy(void) {
    battler* target;
    
    target = get_battler(CURRENT_TARGET);
    
    // Display offense using ROM data access layer
    display_text_wait(get_battle_message(MSG_BTL_CHECK_OFFENSE), target->offense);
    
    // Display defense using ROM data access layer  
    display_text_wait(get_battle_message(MSG_BTL_CHECK_DEFENSE), target->defense);
    
    // Check resistances and display messages if fully resistant (0xFF)
    if (target->fire_resist == 0xFF) {
        display_text_wait(get_battle_message(MSG_BTL_CHECK_ANTI_FIRE), 0);
    }
    
    if (target->freeze_resist == 0xFF) {
        display_text_wait(get_battle_message(MSG_BTL_CHECK_ANTI_FREEZE), 0);
    }
    
    if (target->flash_resist == 0xFF) {
        display_text_wait(get_battle_message(MSG_BTL_CHECK_ANTI_FLASH), 0);
    }
    
    if (target->paralysis_resist == 0xFF) {
        display_text_wait(get_battle_message(MSG_BTL_CHECK_ANTI_PARALYSIS), 0);
    }
    
    if (target->hypnosis_resist == 0xFF) {
        display_text_wait(get_battle_message(MSG_BTL_CHECK_BRAIN_LEVEL_0), 0);
    }
    
    if (target->brainshock_resist == 0xFF) {
        display_text_wait(get_battle_message(MSG_BTL_CHECK_BRAIN_LEVEL_3), 0);
    }
    
    // Check for item drop from enemies
    if (target->ally_or_enemy == 1) {  // Enemy
        // Simplified version - would need find_inventory_space2 implementation
        if (ITEM_DROPPED != 0) {
            display_text_wait(get_battle_message(MSG_BTL_CHECK_PRESENT_GET), 0);
            ITEM_DROPPED = 0;
        }
    }
}

// Hypnosis Alpha redirect - calls main hypnosis function
void redirect_btlact_hypnosis_a_copy(void) {
    btlact_hypnosis_a();
}

// Pray golden action - recovers HP equal to attacker's missing HP
void btlact_pray_golden(void) {
    battler* target;
    battler* attacker;
    unsigned short missing_hp;
    
    target = get_battler(CURRENT_TARGET);
    attacker = get_battler(CURRENT_ATTACKER);
    
    missing_hp = target->hp_max - attacker->hp_target;
    recover_hp(CURRENT_TARGET, missing_hp);
}


// Crying 2 action - inflicts crying status
void btlact_crying2(void) {
    if (fail_attack_on_npcs() == 0) {
        if (inflict_status_battle(CURRENT_TARGET, STATUS_2_CRYING, STATUS_2_CRYING) != 0) {
            display_text_wait(get_battle_message(MSG_BTL_NAMIDA_ON), 0);
        } else {
            display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        }
    }
}

// Inflict solidification - freezes target
void btlact_inflict_solidification(void) {
    battler* target;
    
    if (success_luck80() != 0) {
        target = get_battler(CURRENT_TARGET);
        if (success_255(target->paralysis_resist) != 0) {
            if (inflict_status_battle(CURRENT_TARGET, STATUS_GROUP_TEMPORARY, STATUS_2_SOLIDIFIED) != 0) {
                display_text_wait(get_battle_message(MSG_BTL_KOORI_ON), 0);
                return;
            }
        }
    }
    display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
}

// Defense down alpha - reduces target defense
void btlact_defense_down_a(void) {
    battler* target;
    unsigned short original_defense;
    unsigned short defense_reduction;
    
    if (fail_attack_on_npcs() == 0) {
        if (success_luck80() != 0) {
            target = get_battler(CURRENT_TARGET);
            original_defense = target->defense;
            
            // Reduce defense by 1/16th (hexadecimate)
            hexadecimate_defense(CURRENT_TARGET);
            
            // Calculate defense reduction amount
            defense_reduction = original_defense - target->defense;
            
            // Ensure we don't show negative reduction
            if (defense_reduction > 0) {
                display_text_wait(get_battle_message(MSG_BTL_DEFENSE_DOWN), defense_reduction);
            }
        } else {
            display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        }
    }
}

// Switch armor - changes armor during battle and recalculates stats  
void btlact_switch_armor(void) {
    battler* attacker;
    unsigned char item_slot;
    char_struct* character;
    unsigned char defense_bonus;
    unsigned char speed_bonus;
    unsigned char luck_bonus;
    unsigned char brainshock_base;
    
    attacker = get_battler(CURRENT_ATTACKER);
    
    // Enable blinking triangle for equipment UI
    enable_blinking_triangle(1);
    
    // Check if the item slot is valid for this character
    item_slot = attacker->current_action_argument & 0xFF;
    if (unknown_c3ee14(attacker->id, item_slot) == 0) {
        display_text_wait(get_battle_message(MSG_BTL_EQUIP_NG_WEAPON), 0);
        clear_blinking_prompt();
        return;
    }
    
    // Get character data pointer
    character = get_character_data(attacker->row);
    
    // Store stat bonuses from current equipment before change
    defense_bonus = attacker->defense - attacker->base_defense;
    speed_bonus = attacker->speed - attacker->base_speed;
    luck_bonus = attacker->luck - attacker->base_luck;
    
    // Equip the new armor
    equip_item(attacker->id, attacker->action_item_slot & 0xFF);
    display_text_wait(get_battle_message(MSG_BTL_EQUIP_OK), 0);
    
    // Update base stats from character data
    attacker->base_defense = character->defense;
    attacker->base_speed = character->speed;
    attacker->base_luck = character->luck;
    
    // Reapply stat bonuses
    attacker->defense = attacker->base_defense + defense_bonus;
    attacker->speed = attacker->base_speed + speed_bonus;
    attacker->luck = attacker->base_luck + luck_bonus;
    
    // Update elemental resistances with modifiers
    calc_psi_dmg_modifiers(character->fire_resist);
    attacker->fire_resist = get_calc_result();
    
    calc_psi_dmg_modifiers(character->freeze_resist);
    attacker->freeze_resist = get_calc_result();
    
    calc_psi_res_modifiers(character->flash_resist);
    attacker->flash_resist = get_calc_result();
    
    calc_psi_res_modifiers(character->paralysis_resist);
    attacker->paralysis_resist = get_calc_result();
    
    calc_psi_res_modifiers(character->hypnosis_brainshock_resist);
    attacker->hypnosis_resist = get_calc_result();
    
    // Brainshock resistance is calculated as (3 - base resistance)
    brainshock_base = 3 - character->hypnosis_brainshock_resist;
    calc_psi_res_modifiers(brainshock_base);
    attacker->brainshock_resist = get_calc_result();
    
    clear_blinking_prompt();
}

// Lifeup Beta - healing spell
void btlact_lifeup_b(void) {
    lifeup_common(LIFEUP_BETA_HEALING);
}

// Sow seeds - summons help
void btlact_sow_seeds(void) {
    call_for_help_common(1);
}

// Brainshock Alpha redirect - redirects to main brainshock function
void redirect_btlact_brainshock_a(void) {
    btlact_brainshock_a();
}

// Random stat up 1d4 - randomly increases one stat by 1-4 points
void btlact_random_stat_up_1d4(void) {
    battler* target;
    unsigned short stat_choice;
    unsigned short increase_amount;
    
    target = get_battler(CURRENT_TARGET);
    stat_choice = rand_limit(7);
    increase_amount = rand_limit(4) + 1; // 1-4
    
    switch (stat_choice) {
        case 0: // Defense
            target->defense += increase_amount;
            display_text_wait(get_battle_message(MSG_BTL_DEFENSE_UP), increase_amount);
            break;
            
        case 1: // Offense
            target->offense += increase_amount;
            display_text_wait(get_battle_message(MSG_BTL_OFFENSE_UP), increase_amount);
            break;
            
        case 2: // Speed
            btlact_speed_up_1d4();
            break;
            
        case 3: // Guts
            btlact_guts_up_1d4();
            break;
            
        case 4: // Vitality
            btlact_vitality_up_1d4();
            break;
            
        case 5: // IQ
            btlact_iq_up_1d4();
            break;
            
        case 6: // Luck
            btlact_luck_up_1d4();
            break;
    }
}

void btlact_double_bash(void) {
    btlact_bash();
    btlact_bash();
}

void btlact_bomb(void) {
    bomb_common(90);
}

void btlact_cold(void) {
    battler* target;
    
    if (fail_attack_on_npcs() != 0) return;
    
    target = get_battler(CURRENT_TARGET);
    if (success_255(target->freeze_resist) == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    if (inflict_status_battle(CURRENT_TARGET, STATUS_0_COLD, STATUS_GROUP_PERSISTENT_EASYHEAL) != 0) {
        display_text_wait((const char*)MSG_BTL_KAZE_ON, 0);
    } else {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    }
}

void btlact_diamondize(void) {
    battler* target;
    
    if (fail_attack_on_npcs() != 0) return;
    
    target = get_battler(CURRENT_TARGET);
    if (success_255(target->paralysis_resist) == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    if (inflict_status_battle(CURRENT_TARGET, STATUS_0_DIAMONDIZED, STATUS_GROUP_PERSISTENT_EASYHEAL) == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    // TODO: Clear all status effects when diamondized (need to add afflictions field to battler struct)
    // target->afflictions[STATUS_GROUP_SHIELD] = 0;
    // target->afflictions[STATUS_GROUP_HOMESICKNESS] = 0;
    // target->afflictions[STATUS_GROUP_CONCENTRATION] = 0;
    // target->afflictions[STATUS_GROUP_STRANGENESS] = 0;
    // target->afflictions[STATUS_GROUP_TEMPORARY] = 0;
    // target->afflictions[STATUS_GROUP_PERSISTENT_HARDHEAL] = 0;
    
    // TODO: Add EXP and money to battle totals (need to add exp/money fields)
    // BATTLE_EXP_SCRATCH += target->exp;
    // BATTLE_MONEY_SCRATCH += target->money;
    
    display_text_wait((const char*)MSG_BTL_DAIYA_ON, 0);
}

void btlact_feel_strange(void) {
    if (fail_attack_on_npcs() != 0) return;
    
    if (inflict_status_battle(CURRENT_TARGET, STATUS_3_STRANGE, STATUS_GROUP_STRANGENESS) != 0) {
        display_text_wait((const char*)MSG_BTL_HEN_ON, 0);
    } else {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    }
}

void btlact_paralyze(void) {
    battler* target;
    
    if (fail_attack_on_npcs() != 0) return;
    
    if (success_luck80() == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    target = get_battler(CURRENT_TARGET);
    if (success_255(target->paralysis_resist) == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    if (inflict_status_battle(CURRENT_TARGET, STATUS_0_PARALYZED, STATUS_GROUP_PERSISTENT_EASYHEAL) != 0) {
        display_text_wait((const char*)MSG_BTL_SHIBIRE_ON, 0);
    } else {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    }
}

void btlact_poison(void) {
    if (fail_attack_on_npcs() != 0) return;
    
    if (inflict_status_battle(CURRENT_TARGET, STATUS_0_POISONED, STATUS_GROUP_PERSISTENT_EASYHEAL) != 0) {
        display_text_wait((const char*)MSG_BTL_MODOKU_ON, 0);
    } else {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    }
}

void btlact_nauseate(void) {
    if (fail_attack_on_npcs() != 0) return;
    
    if (inflict_status_battle(CURRENT_TARGET, STATUS_0_NAUSEOUS, STATUS_GROUP_PERSISTENT_EASYHEAL) != 0) {
        display_text_wait((const char*)MSG_BTL_KIMOCHI_ON, 0);
    } else {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    }
}

void btlact_mushroomize(void) {
    if (fail_attack_on_npcs() != 0) return;
    
    // Status group is same as status id for mushroomized
    if (inflict_status_battle(CURRENT_TARGET, STATUS_1_MUSHROOMIZED, STATUS_1_MUSHROOMIZED) != 0) {
        display_text_wait((const char*)MSG_BTL_KINOKO_ON, 0);
    } else {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    }
}

void btlact_crying(void) {
    battler* target;
    
    if (fail_attack_on_npcs() != 0) return;
    
    target = get_battler(CURRENT_TARGET);
    if (success_255(target->flash_resist) == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    // Status ID is same as status group for crying
    if (inflict_status_battle(CURRENT_TARGET, STATUS_2_CRYING, STATUS_2_CRYING) != 0) {
        display_text_wait((const char*)MSG_BTL_NAMIDA_ON, 0);
    } else {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    }
}

void btlact_immobilize(void) {
    if (inflict_status_battle(CURRENT_TARGET, STATUS_2_IMMOBILIZED, STATUS_GROUP_TEMPORARY) != 0) {
        display_text_wait((const char*)MSG_BTL_SHIBARA_ON, 0);
    } else {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    }
}

void btlact_neutralize(void) {
    battler* target;
    
    target = get_battler(CURRENT_TARGET);
    
    // Reset all stats to base values
    target->offense = target->base_offense;
    target->defense = target->base_defense; 
    target->speed = target->base_speed;
    target->guts = target->base_guts;
    target->luck = target->base_luck;
    
    // TODO: Clear shield HP and shield afflictions (need shield_hp field in battler)
    // target->shield_hp = 0;
    // target->afflictions[STATUS_GROUP_SHIELD] = 0;
    
    display_text_wait((const char*)MSG_BTL_NEUTRALIZE_RESULT, 0);
}

void btlact_distract(void) {
    battler* target;
    
    if (fail_attack_on_npcs() != 0) return;
    
    if (success_luck40() == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    target = get_battler(CURRENT_TARGET);
    if (success_255(target->paralysis_resist) == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    // TODO: Check concentration afflictions (need afflictions field)
    // For now, assume no concentration status and apply distraction
    // if (target->afflictions[STATUS_GROUP_CONCENTRATION] != 0) {
    //     display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    //     return;
    // }
    
    // TODO: Apply can't concentrate status (need afflictions field)
    // target->afflictions[STATUS_GROUP_CONCENTRATION] = STATUS_4_CANT_CONCENTRATE4;
    
    display_text_wait((const char*)MSG_BTL_FUUIN_ON, 0);
}

void btlact_inflict_poison(void) {
    battler* target;
    
    target = get_battler(CURRENT_TARGET);
    if (success_255(target->paralysis_resist) == 0) {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
        return;
    }
    
    if (inflict_status_battle(CURRENT_TARGET, STATUS_0_POISONED, STATUS_GROUP_PERSISTENT_EASYHEAL) != 0) {
        display_text_wait((const char*)MSG_BTL_MODOKU_ON, 0);
    } else {
        display_text_wait((const char*)MSG_BTL_KIKANAI, 0);
    }
}

void btlact_cut_guts(void) {
    battler* target;
    unsigned char original_guts;
    unsigned char minimum_guts;
    unsigned char guts_lost;
    
    if (fail_attack_on_npcs() != 0) return;
    
    target = get_battler(CURRENT_TARGET);
    original_guts = target->guts;
    
    // Reduce guts to 75% of current value
    target->guts = (target->guts * 3) / 4;
    
    // Ensure guts doesn't go below half of base guts
    minimum_guts = target->base_guts / 2;
    if (target->guts < minimum_guts) {
        target->guts = minimum_guts;
    }
    
    // Calculate and display the amount lost
    guts_lost = original_guts - target->guts;
    display_text_wait((const char*)MSG_BTL_GUTS_DOWN, guts_lost);
}

// Additional null battle actions
void btlact_null1(void) {
    // Empty function - no operation
}

void btlact_luck_up_1d4(void) {
    unsigned char increase;
    battler* target;
    
    increase = rand_limit(4) + 1; // 1-4 random increase
    target = get_battler(CURRENT_TARGET);
    target->luck += increase;
    display_text_wait((const char*)MSG_BTL_LUCK_UP, increase);
}

void btlact_guts_up_1d4(void) {
    unsigned char increase;
    battler* target;
    
    increase = rand_limit(4) + 1; // 1-4 random increase
    target = get_battler(CURRENT_TARGET);
    target->guts += increase;
    display_text_wait((const char*)MSG_BTL_GUTS_UP, increase);
}

// Batch converted stat up functions using template pattern
void btlact_speed_up_1d4(void) {
    unsigned char increase;
    battler* target;
    
    increase = rand_limit(4) + 1; // 1-4 random increase
    target = get_battler(CURRENT_TARGET);
    target->speed += increase;
    display_text_wait((const char*)MSG_BTL_SPEED_UP, increase);
}

void btlact_vitality_up_1d4(void) {
    unsigned char increase;
    
    increase = rand_limit(4) + 1; // 1-4 random increase
    // TODO: Add vitality field to battler struct
    // battler* target = get_battler(CURRENT_TARGET);
    // target->vitality += increase;
    display_text_wait((const char*)MSG_BTL_VITA_UP, increase);
}

void btlact_iq_up_1d4(void) {
    unsigned char increase;
    
    increase = rand_limit(4) + 1; // 1-4 random increase
    // TODO: Add IQ field to battler struct to actually apply increase
    // battler* target = get_battler(CURRENT_TARGET);
    // target->iq += increase;
    display_text_wait((const char*)MSG_BTL_IQ_UP, increase);
}

void heal_poison(void) {
    // TODO: Need afflictions field in battler struct to properly implement
    // battler* target = get_battler(CURRENT_TARGET);
    // if (target->afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_POISONED) {
    //     target->afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] = 0;
    //     display_text_wait((const char*)MSG_BTL_MODOKU_OFF, 0);
    // }
    display_text_wait((const char*)MSG_BTL_MODOKU_OFF, 0);
}

void btlact_null5(void) {
    // Empty function - no operation
}

// Batch converted null functions - all empty implementations
void btlact_null7(void) {
    // Empty function - no operation
}

void btlact_null12(void) {
    // Empty function - no operation
}

void btlact_null6(void) {
    // Empty function - no operation
}

void btlact_null10(void) {
    // Empty function - no operation
}

void btlact_null11(void) {
    // Empty function - no operation
}

// Redirect to null action
void btlact_redirect_null(void) {
    btlact_null();
}

// Level 2 attack that inflicts poison
void btlact_level_2_attack_poison(void) {
    if (fail_attack_on_npcs() != 0) return;
    
    if (miss_calc(0) != 0) return;
    
    if (smaaaash() != 0) return;
    
    if (determine_dodge() != 0) {
        display_text_wait((const char*)MSG_BTL_TATAKU_YOKETA, 0);
        return;
    }
    
    btlact_level_2_atk();
    heal_strangeness();
    
    if (inflict_status_battle(CURRENT_TARGET, STATUS_0_POISONED, STATUS_GROUP_PERSISTENT_EASYHEAL) != 0) {
        display_battle_text_ptr(MSG_BTL_MODOKU_ON);
    }
}

// Bottle rocket common function - multiple hit calculation
void bottle_rocket_common(unsigned short count) {
    unsigned short hits;
    unsigned short i;
    unsigned short damage;
    
    hits = 0;
    
    for (i = 0; i < count; i++) {
        if (success_speed(100) != 0) {
            hits++;
        }
    }
    
    if (hits > 0) {
        damage = hits * 120;
        damage = twenty_five_percent_variance(damage);
        calc_resist_damage(damage, 0xFF);
    } else {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
    }
}

// Speed-based success check
unsigned char success_speed(unsigned short threshold) {
    battler* attacker;
    battler* target;
    unsigned short attacker_speed;
    unsigned short target_speed;
    unsigned short speed_diff;
    unsigned short random_val;
    
    attacker = get_battler(CURRENT_ATTACKER);
    target = get_battler(CURRENT_TARGET);
    
    attacker_speed = attacker->speed;
    target_speed = target->speed * 2;
    speed_diff = 0;
    
    if (target_speed >= attacker_speed) {
        speed_diff = target_speed - attacker_speed;
    }
    
    random_val = rand_limit(threshold);
    return (random_val < speed_diff) ? 1 : 0;
}

// Brainshock alpha - inflicts strangeness
void btlact_brainshock_alpha(void) {
    battler* target;
    
    if (fail_attack_on_npcs() != 0) return;
    
    target = get_battler(CURRENT_TARGET);
    if (success_255(target->brainshock_resist) == 0) {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
        return;
    }
    
    if (inflict_status_battle(CURRENT_TARGET, STATUS_3_STRANGE, STATUS_GROUP_STRANGENESS) != 0) {
        display_battle_text_ptr(MSG_BTL_HEN_ON);
    } else {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
    }
}

// Redirect to brainshock alpha
void btlact_brainshock_alpha_redirect_copy(void) {
    btlact_brainshock_alpha();
}

// LifeUp Alpha - basic healing spell
void btlact_lifeup_alpha(void) {
    lifeup_common(100);  // LIFEUP_ALPHA_HEALING value
}

// Pray mysterious - recovers 5±2.5 PP
void btlact_pray_mysterious(void) {
    unsigned short amount;
    
    amount = fifty_percent_variance(5);
    if (amount == 0) amount = 1;
    recover_pp(CURRENT_TARGET, amount);
}

// Insect spray common - damages insect-type enemies
void insect_spray_common(unsigned short damage) {
    battler* target;
    unsigned short final_damage;
    
    if (success_luck80() == 0) {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
        return;
    }
    
    target = get_battler(CURRENT_TARGET);
    if (target->ally_or_enemy != 1) {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
        return;
    }
    
    if (get_enemy_type(target->id) != 1) {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
        return;
    }
    
    final_damage = fifty_percent_variance(damage);
    calc_resist_damage(final_damage, 0xFF);
}

// Shield Beta - PSI Shield with power boost
void btlact_shield_beta(void) {
    if (shields_common(CURRENT_TARGET, 0x20) != 0) {  // STATUS_6_SHIELD_POWER value
        display_battle_text_ptr(MSG_BTL_POWER_ADD);
    } else {
        display_battle_text_ptr(MSG_BTL_POWER_ON);
    }
}


void btlact_level_4_atk(void) {
    battler* attacker;
    battler* target;
    short damage;
    
    // Check for miss
    if (miss_calc(0) != 0) return;
    
    // Check for SMAAAASH critical hit
    if (smaaaash() != 0) return;
    
    // Check for dodge
    if (determine_dodge() != 0) {
        display_text_wait((const char*)MSG_BTL_TATAKU_YOKETA, 0);
        return;
    }
    
    // Calculate damage: (attacker offense * 4) - target defense
    attacker = get_battler(CURRENT_ATTACKER);
    target = get_battler(CURRENT_TARGET);
    
    damage = (attacker->offense * 4) - target->defense;
    
    if (damage > 0) {
        damage = twenty_five_percent_variance(damage);
    }
    
    if (damage < 1) {
        damage = 1; // Minimum damage
    }
    
    // Apply resistance and deal damage
    calc_resist_damage(damage, 0xFF); // No specific resistance
    heal_strangeness(); // Clear strange status on hit
}

// Switch weapon - changes weapon during battle and handles special attack behavior
void btlact_switch_weapons(void) {
    battler* attacker;
    unsigned char item_slot;
    char_struct* character;
    unsigned char offense_bonus;
    unsigned char guts_bonus;
    unsigned char weapon_slot;
    unsigned char weapon_item_id;
    unsigned short item_type;
    
    attacker = get_battler(CURRENT_ATTACKER);
    
    // Enable blinking triangle for equipment UI
    enable_blinking_triangle(1);
    
    // Check if the item slot is valid for this character
    item_slot = attacker->current_action_argument & 0xFF;
    if (unknown_c3ee14(attacker->id, item_slot) == 0) {
        display_text_wait(get_battle_message(MSG_BTL_EQUIP_NG_WEAPON), 0);
        clear_blinking_prompt();
        return;
    }
    
    // Get character data pointer
    character = get_character_data(attacker->id - 1);
    
    // Store stat bonuses from current equipment before change
    offense_bonus = attacker->offense - attacker->base_offense;
    guts_bonus = attacker->guts - attacker->base_guts;
    
    // Equip the new weapon
    equip_item(attacker->id, attacker->action_item_slot & 0xFF);
    
    // Update base stats from character data
    attacker->base_offense = character->offense;
    attacker->base_guts = character->guts;
    
    // Reapply stat bonuses
    attacker->offense = attacker->base_offense + offense_bonus;
    attacker->guts = attacker->base_guts + guts_bonus;
    
    display_text_wait(get_battle_message(MSG_BTL_EQUIP_OK), 0);
    
    // Check if new weapon is a shooting weapon (type 1)
    weapon_slot = character->equipment[WEAPON];
    if (weapon_slot > 0) {
        weapon_item_id = character->items[weapon_slot - 1];
        if (weapon_item_id > 0) {
            // Check item type from configuration table
            item_type = get_item_type(weapon_item_id);
            
            if ((item_type & 0x03) == 1) { // Shooting weapon
                // Use shooting attack (battle action 5)
                execute_battle_action(5);
                clear_blinking_prompt();
                return;
            }
        }
    }
    
    // Use regular bash attack (battle action 4)  
    execute_battle_action(4);
    clear_blinking_prompt();
}

// PSI Fire Alpha - fire damage spell
void btlact_psi_fire_a(void) {
    psi_fire_common(FIRE_ALPHA_DAMAGE);
}

// PSI Fire Omega - powerful fire damage spell
void btlact_psi_fire_omega(void) {
    psi_fire_common(FIRE_OMEGA_DAMAGE);
}

// Lifeup Omega - powerful healing spell
void btlact_lifeup_omega(void) {
    lifeup_common(LIFEUP_OMEGA_HEALING);
}

// PSI Rockin Gamma - powerful physical damage spell
void btlact_psi_rockin_gamma(void) {
    psi_rockin_common(ROCKIN_GAMMA_DAMAGE);
}

// PSI Freeze Alpha - ice damage spell
void btlact_psi_freeze_alpha(void) {
    psi_freeze_common(FREEZE_ALPHA_DAMAGE);
}

// Shield Beta redirect - redirects to main Shield function
void redirect_btlact_shield_b(void) {
    btlact_shield_beta();
}

// Healing Alpha - cures specific status ailments
void btlact_healing_a(void) {
    battler* target;
    
    target = get_battler(CURRENT_TARGET);
    
    if (target->afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_COLD) {
        target->afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] = 0;
        display_battle_text_ptr(MSG_BTL_KAZE_OFF);
    } else if (target->afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_SUNSTROKE) {
        target->afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] = 0;
        display_battle_text_ptr(MSG_BTL_NISSYA_OFF);
    } else if (target->afflictions[STATUS_GROUP_TEMPORARY] == STATUS_2_ASLEEP) {
        target->afflictions[STATUS_GROUP_TEMPORARY] = 0;
        display_battle_text_ptr(MSG_BTL_NEMURI_OFF);
    } else {
        display_battle_text_ptr(MSG_BTL_HEAL_NG);
    }
}

// Healing Omega - revives unconscious targets or cures all ailments
void btlact_healing_omega(void) {
    battler* target;
    
    target = get_battler(CURRENT_TARGET);
    
    if (target->afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_UNCONSCIOUS) {
        revive_target(CURRENT_TARGET, target->hp_max);
    } else {
        btlact_healing_g();
    }
}

// Multi Bottle Rocket - fires multiple bottle rockets
void btlact_multi_bottle_rocket(void) {
    bottle_rocket_common(MULTI_BOTTLE_ROCKET_COUNT);
}

// Sudden Guts Pill - doubles target's guts stat
void btlact_sudden_guts_pill(void) {
    battler* target;
    unsigned short new_guts;
    
    if (fail_attack_on_npcs() != 0) return;
    
    target = get_battler(CURRENT_TARGET);
    new_guts = target->guts * 2;
    if (new_guts > 255) new_guts = 255;
    
    target->guts = new_guts;
    display_text_wait((const char*)MSG_BTL_2GUTS_UP, new_guts);
}

// Magnet Omega - PP drain spell (Jeff-specific behavior)
void btlact_magnet_omega(void) {
    battler* target;
    
    target = get_battler(CURRENT_TARGET);
    
    if (target->ally_or_enemy == 0 && target->id == PARTY_MEMBER_JEFF) {
        // Jeff can't use this on himself
        return;
    }
    
    btlact_magnet_a();
}

// PSI Rockin Beta - physical damage spell
void btlact_psi_rockin_b(void) {
    psi_rockin_common(ROCKIN_BETA_DAMAGE);
}

// PSI Shield Beta redirect - redirects to main PSI Shield function
void redirect_btlact_psi_shield_b(void) {
    btlact_psi_shield_b();
}

// Level 3 attack copy - redirects to main level 3 attack function
void redirect_btlact_level_3_atk(void) {
    btlact_level_3_atk();
}

// Hypnosis Alpha - inflicts sleep status
void btlact_hypnosis_a(void) {
    battler* target;
    
    if (fail_attack_on_npcs() != 0) return;
    
    target = get_battler(CURRENT_TARGET);
    if (success_255(target->hypnosis_resist) == 0) {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
        return;
    }
    
    if (inflict_status_battle(CURRENT_TARGET, STATUS_2_ASLEEP, STATUS_GROUP_TEMPORARY) != 0) {
        display_battle_text_ptr(MSG_BTL_NEMURI_ON);
    } else {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
    }
}

// Convert ASM function REDUCE_PP
void reduce_pp(unsigned short character_id, unsigned short amount) {
    battler* target = get_battler(character_id);
    unsigned short current_pp = target->pp_target;
    unsigned short new_pp;
    
    // If amount >= current_pp, set to 0, otherwise subtract
    if (amount >= current_pp) {
        new_pp = 0;
    } else {
        new_pp = current_pp - amount;
    }
    
    set_pp(character_id, new_pp);
}

// Convert AUTOHEALING function  
unsigned short autohealing(unsigned short status_id, unsigned short status_group) {
    unsigned short lowest_hp = 9999;
    unsigned short target_character = 0;
    unsigned short i;
    unsigned short char_index;
    char_struct* character;
    
    // TODO: Need to fix game state party member access
    // This is a partial conversion - needs proper party iteration
    for (i = 0; i < 4; i++) {
        character = &PARTY_CHARACTERS[i];
        
        // Check if character has the specified status condition
        if (character->afflictions[status_group] == status_id) {
            // Check if this character has lower HP and no unknown flag
            if (character->current_hp < lowest_hp) {
                lowest_hp = character->current_hp;
                target_character = i + 1; // Convert to 1-based index
            }
        }
    }
    
    // Mark target for autohealing if found
    if (target_character != 0) {
        char_index = target_character - 1;
        // TODO: Need proper field name for marking autohealing target
        // PARTY_CHARACTERS[char_index].unknown_flag = 1;
    }
    
    return target_character;
}

// Convert BOSS_BATTLE_CHECK function
unsigned short boss_battle_check(void) {
    unsigned short i;
    battler* current_battler;
    
    // Check all 32 battle slots for boss enemies
    for (i = 0; i < 32; i++) {
        current_battler = &BATTLERS_TABLE[i];
        
        // Skip if not conscious
        if (current_battler->consciousness == 0) continue;
        
        // Skip if not an enemy (allies are 0, enemies are 1)
        if (current_battler->ally_or_enemy != 1) continue;
        
        // Check if this enemy ID corresponds to a boss
        // TODO: Need ENEMY_CONFIGURATION_TABLE access
        // For now, simplified check - just return 0 if we find any enemy
        // Real implementation would check enemy_data::boss flag
        return 0; // Boss found - not a regular battle
    }
    
    return 1; // No bosses found - regular battle
}

// Random variance function - applies ±50% variance to a value
unsigned short fifty_percent_variance(unsigned short value) {
    unsigned char rand1, rand2;
    short offset1, offset2;
    unsigned char smaller_offset;
    
    // Get two random bytes  
    rand1 = (unsigned char)(rand_long() & 0xFF);
    rand2 = (unsigned char)(rand_long() & 0xFF);
    
    // Convert to signed offsets (-128 to +127)
    offset1 = (short)rand1 - 128;
    offset2 = (short)rand2 - 128;
    
    // Take absolute values to find smaller offset
    if (offset1 < 0) offset1 = -offset1;
    if (offset2 < 0) offset2 = -offset2;
    
    // Use the smaller absolute offset
    smaller_offset = (offset1 < offset2) ? rand2 : rand1;
    
    // Apply offset based on direction (sign check against 128)
    if (smaller_offset < 128) {
        // Subtract from value
        if (value > (128 - smaller_offset)) {
            return value - (128 - smaller_offset);
        } else {
            return 0; // Prevent underflow
        }
    } else {
        // Add to value  
        return value + (smaller_offset - 128);
    }
}

// Reduce HP of target battler by specified amount
void reduce_hp(unsigned char target, unsigned short amount) {
    unsigned short current_hp;
    unsigned short new_hp;
    
    // Get current HP from battler table
    current_hp = BATTLERS_TABLE[target].hp_target;
    
    // Calculate new HP (prevent underflow)
    if (amount >= current_hp) {
        new_hp = 0;
    } else {
        new_hp = current_hp - amount;
    }
    
    // Set the new HP value
    set_hp(target, new_hp);
}

// Set targeting flags for all conscious battlers
void target_all(void) {
    unsigned short i;
    
    // Clear target flags
    BATTLER_TARGET_FLAGS = 0;
    
    // Loop through all battlers
    for (i = 0; i < BATTLER_COUNT; i++) {
        // Check if battler is conscious (non-zero consciousness field)
        if (BATTLERS_TABLE[i].consciousness != 0) {
            // Set bit for this battler using powers of two table
            BATTLER_TARGET_FLAGS |= POWERS_OF_TWO_32BIT[i];
        }
    }
}

// Set targeting flags for all conscious allied battlers
void target_allies(void) {
    unsigned short i;
    
    // Clear target flags
    BATTLER_TARGET_FLAGS = 0;
    
    // Loop through all battlers
    for (i = 0; i < BATTLER_COUNT; i++) {
        // Check if battler is conscious
        if (BATTLERS_TABLE[i].consciousness == 0) continue;
        
        // Check if battler is an ally (ally_or_enemy == 0) OR is an NPC (npc_id != 0)
        if (BATTLERS_TABLE[i].ally_or_enemy == 0 || BATTLERS_TABLE[i].npc_id != 0) {
            // Set bit for this battler using powers of two table
            BATTLER_TARGET_FLAGS |= POWERS_OF_TWO_32BIT[i];
        }
    }
}

// Set targeting flag for a specific battler
void target_battler(unsigned short battler_id) {
    // Set bit for this specific battler using powers of two table
    BATTLER_TARGET_FLAGS |= POWERS_OF_TWO_32BIT[battler_id];
}

// Check if attack fails on NPCs - returns 1 if attack should fail
unsigned char fail_attack_on_npcs(void) {
    unsigned short target_index;
    
    target_index = CURRENT_TARGET;
    
    // Check if target is an NPC (npc_id != 0)
    if (BATTLERS_TABLE[target_index].npc_id != 0) {
        // Display "It doesn't work" message
        display_battle_text_ptr(MSG_BTL_KIKANAI);
        return 1; // Attack fails
    }
    
    return 0; // Attack proceeds normally
}

// Determine if target dodges attack - returns 1 if attack is dodged
unsigned short determine_dodge(void) {
    unsigned short target_index;
    unsigned short speed_diff;
    
    target_index = CURRENT_TARGET;
    
    // Check if target is paralyzed - cannot dodge
    if (BATTLERS_TABLE[target_index].afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_PARALYZED) {
        return 0;
    }
    
    // Check if target is asleep - cannot dodge
    if (BATTLERS_TABLE[target_index].afflictions[STATUS_GROUP_TEMPORARY] == STATUS_2_ASLEEP) {
        return 0;
    }
    
    // Check if target is immobilized - cannot dodge
    if (BATTLERS_TABLE[target_index].afflictions[STATUS_GROUP_TEMPORARY] == STATUS_2_IMMOBILIZED) {
        return 0;
    }
    
    // Check if target is solidified - cannot dodge
    if (BATTLERS_TABLE[target_index].afflictions[STATUS_GROUP_TEMPORARY] == STATUS_2_SOLIDIFIED) {
        return 0;
    }
    
    // Calculate speed difference (target's speed * 2 - attacker's speed)
    speed_diff = (BATTLERS_TABLE[target_index].speed * 2) - BATTLERS_TABLE[CURRENT_ATTACKER].speed;
    
    // If speed difference is negative or zero, cannot dodge
    if (speed_diff <= 0) {
        return 0;
    }
    
    // Use success check with speed difference
    return success_500(speed_diff);
}

// Success check function - returns 1 if random(500) < value
unsigned char success_500(unsigned short value) {
    unsigned short random_val;
    
    random_val = rand_limit(500);
    
    return (random_val < value) ? 1 : 0;
}

// Check if a battler is currently targetted - returns 1 if targetted
unsigned char is_char_targetted(unsigned short battler_index) {
    unsigned long battler_bit;
    
    // Get the bit flag for this battler
    battler_bit = POWERS_OF_TWO_32BIT[battler_index];
    
    // Check if this bit is set in the target flags
    return ((BATTLER_TARGET_FLAGS & battler_bit) != 0) ? 1 : 0;
}

// Remove a battler from targeting (clear their bit)
void remove_target(unsigned short battler_index) {
    unsigned long battler_bit;
    
    // Get the bit flag for this battler
    battler_bit = POWERS_OF_TWO_32BIT[battler_index];
    
    // Clear this bit from the target flags (bitwise AND with inverted bit)
    BATTLER_TARGET_FLAGS &= ~battler_bit;
}

// Remove all unconscious battlers from targeting
void remove_dead_targetting(void) {
    unsigned short i;
    
    for (i = 0; i < BATTLER_COUNT; i++) {
        // Check if this battler is currently targetted
        if (is_char_targetted(i)) {
            // Check if battler is unconscious
            if (BATTLERS_TABLE[i].afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_UNCONSCIOUS) {
                // Remove from targeting
                remove_target(i);
            }
        }
    }
}

// Decrease offense by 1/16th (hexadecimate - divide by 16)
void decrease_offense_16th(unsigned short battler_ptr) {
    unsigned short reduction;
    unsigned short min_offense;
    unsigned short* offense_ptr;
    
    // Calculate offense pointer
    offense_ptr = (unsigned short*)((unsigned char*)battler_ptr + offsetof(battler, offense));
    
    // Calculate reduction (offense >> 4, minimum 1)
    reduction = (*offense_ptr) >> 4;
    if (reduction == 0) {
        reduction = 1;
    }
    
    // Apply reduction
    (*offense_ptr) -= reduction;
    
    // Calculate minimum allowed offense (base_offense * 3/4)
    min_offense = (((battler*)battler_ptr)->base_offense * 3) >> 2;
    
    // Ensure offense doesn't go below minimum
    if ((*offense_ptr) < min_offense) {
        (*offense_ptr) = min_offense;
    }
}

// Return pointer to battle target name buffer
unsigned short return_battle_target_address(void) {
    return (unsigned short)BATTLE_TARGET_NAME;
}

// Set targeting flags for all conscious enemy battlers
void target_all_enemies(void) {
    unsigned short i;
    
    // Clear target flags
    BATTLER_TARGET_FLAGS = 0;
    
    // Loop through all battlers
    for (i = 0; i < BATTLER_COUNT; i++) {
        // Check if battler is conscious
        if (BATTLERS_TABLE[i].consciousness == 0) continue;
        
        // Check if battler is an enemy (ally_or_enemy == 1)
        if (BATTLERS_TABLE[i].ally_or_enemy == 1) {
            // Set bit for this battler using powers of two table
            BATTLER_TARGET_FLAGS |= POWERS_OF_TWO_32BIT[i];
        }
    }
}

// Check if battler is a valid target - returns 1 if valid
unsigned char check_if_valid_target(unsigned short battler_index) {
    // Check if battler is conscious (non-zero consciousness)
    if (BATTLERS_TABLE[battler_index].consciousness == 0) {
        return 0; // Invalid: unconscious
    }
    
    // Check if battler is an NPC (npc_id != 0) - NPCs cannot be targeted
    if (BATTLERS_TABLE[battler_index].npc_id != 0) {
        return 0; // Invalid: NPC
    }
    
    // Check if battler is unconscious via status
    if (BATTLERS_TABLE[battler_index].afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_UNCONSCIOUS) {
        return 0; // Invalid: unconscious
    }
    
    // Check if battler is diamondized
    if (BATTLERS_TABLE[battler_index].afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_DIAMONDIZED) {
        return 0; // Invalid: diamondized
    }
    
    return 1; // Valid target
}

// Get shield targeting type - returns 1 for single target, 0 for multi-target
unsigned char get_shield_targetting(unsigned short action) {
    // Single target shield actions
    if (action == BATTLE_ACTIONS_PSI_SHIELD_SIGMA ||
        action == BATTLE_ACTIONS_PSI_SHIELD_OMEGA ||
        action == BATTLE_ACTIONS_PSI_PSI_SHIELD_SIGMA ||
        action == BATTLE_ACTIONS_PSI_PSI_SHIELD_OMEGA) {
        return 1; // Single target
    }
    
    return 0; // Multi-target
}

// Swap the current attacker and target
void swap_attacker_with_target(void) {
    unsigned short temp;
    
    // Swap CURRENT_ATTACKER and CURRENT_TARGET
    temp = CURRENT_ATTACKER;
    CURRENT_ATTACKER = CURRENT_TARGET;
    CURRENT_TARGET = temp;
    
    // Update name displays
    fix_attacker_name(0);
    fix_target_name();
}

// Calculate miss chance for attacks - returns 1 if attack misses
unsigned short miss_calc(unsigned short miss_message) {
    unsigned short miss_chance;
    unsigned short attacker_index;
    unsigned short random_val;
    unsigned short weapon_item_id;
    unsigned short item_config_offset;
    
    attacker_index = CURRENT_ATTACKER;
    
    // Check if attacker is a player character (not enemy)
    if (BATTLERS_TABLE[attacker_index].npc_id == 0 &&
        BATTLERS_TABLE[attacker_index].ally_or_enemy == 0) {
        
        // Get character ID and check for equipped weapon
        unsigned short char_id = BATTLERS_TABLE[attacker_index].id;
        if (char_id > 0) {
            // Calculate character struct offset
            unsigned short char_offset = mult168(char_id - 1, sizeof(char_struct));
            
            // Get equipped weapon slot
            unsigned char weapon_slot = PARTY_CHARACTERS[char_offset / sizeof(char_struct)].equipment[WEAPON];
            
            if (weapon_slot != 0) {
                // Get weapon item ID from inventory
                weapon_item_id = PARTY_CHARACTERS[char_offset / sizeof(char_struct)].items[weapon_slot - 1];
                
                // Get weapon miss rate from item configuration
                item_config_offset = mult168(weapon_item_id, sizeof(item));
                miss_chance = ITEM_CONFIGURATION_TABLE[item_config_offset + offsetof(item, params) + offsetof(item_parameters, special)];
                
                // Convert miss rate (subtract 128 and invert bits)
                miss_chance = (miss_chance - 128) ^ 0xFF80;
            } else {
                // No weapon equipped - base miss rate
                miss_chance = 1;
            }
        } else {
            miss_chance = 1;
        }
    } else {
        // Enemy attacker - get miss rate from enemy configuration
        unsigned short enemy_offset = mult168(BATTLERS_TABLE[attacker_index].id, sizeof(enemy_data));
        miss_chance = ENEMY_CONFIGURATION_TABLE[enemy_offset + offsetof(enemy_data, miss_rate)];
    }
    
    // Check for status effects that increase miss rate
    if (BATTLERS_TABLE[attacker_index].afflictions[STATUS_GROUP_TEMPORARY] == STATUS_2_CRYING ||
        BATTLERS_TABLE[attacker_index].afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_NAUSEOUS) {
        miss_chance += 8; // Increase miss chance
    }
    
    // No miss chance means automatic hit
    if (miss_chance == 0) {
        return 0;
    }
    
    // Roll random number (1-16) and compare to miss chance
    random_val = rand_limit(16);
    if ((miss_chance - 1) < random_val) {
        return 0; // Hit
    }
    
    // Attack misses - display appropriate message
    if (miss_message == 0) {
        display_battle_text_ptr(MSG_BTL_KARABURI);
    } else {
        display_battle_text_ptr(MSG_BTL_KARABURI_UTSU);
    }
    
    return 1; // Miss
}

// Get battle sprite height based on sprite type
unsigned short get_battle_sprite_height(unsigned short sprite) {
    unsigned short sprite_data_offset;
    unsigned char sprite_type;
    
    // Calculate offset into sprite pointers table
    sprite_data_offset = (sprite - 1) * 5 + 4; // (sprite-1)*5 + 4 offset
    
    // Get sprite type from battle sprites table
    sprite_type = BATTLE_SPRITES_POINTERS[sprite_data_offset];
    
    // Return height based on sprite type
    switch (sprite_type) {
        case 1:
        case 2:
            return 4;  // Small sprites
            
        case 3:
        case 4:
        case 5:
            return 8;  // Medium sprites
            
        case 6:
            return 16; // Large sprites
            
        default:
            return 0;  // Unknown/invalid
    }
}

// Count conscious characters on specific side (0=allies, 1=enemies)
unsigned short count_chars(unsigned short side) {
    unsigned short count;
    unsigned short i;
    
    count = 0;
    
    // Loop through all 32 battlers (#$0020 in ASM)
    for (i = 0; i < BATTLER_COUNT; i++) {
        // Check if conscious
        if (BATTLERS_TABLE[i].consciousness == 0) continue;
        
        // Check if on correct side
        if (BATTLERS_TABLE[i].ally_or_enemy != side) continue;
        
        // Check if not an NPC
        if (BATTLERS_TABLE[i].npc_id != 0) continue;
        
        // Check afflictions - skip unconscious or diamondized
        if (BATTLERS_TABLE[i].afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_UNCONSCIOUS) continue;
        if (BATTLERS_TABLE[i].afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_DIAMONDIZED) continue;
        
        count++;
    }
    
    return count;
}

// Recover PP for target battler
void recover_pp(unsigned char target, unsigned short amount) {
    unsigned short current_pp;
    unsigned short max_pp;
    unsigned short recovered_amount;
    
    // Check if battler is conscious and not unconscious
    if (BATTLERS_TABLE[target].consciousness != 1) return;
    if (BATTLERS_TABLE[target].afflictions[STATUS_GROUP_PERSISTENT_EASYHEAL] == STATUS_0_UNCONSCIOUS) return;
    
    // Get current and max PP
    current_pp = BATTLERS_TABLE[target].pp_target;
    max_pp = BATTLERS_TABLE[target].pp_max;
    
    // Calculate recovery amount - don't exceed max
    if (current_pp + amount >= max_pp) {
        recovered_amount = max_pp - current_pp;
    } else {
        recovered_amount = amount;
    }
    
    // Set new PP value
    set_pp(target, current_pp + amount);
    
    // Display recovery message
    display_text_wait((const char*)MSG_BTL_PP_KAIFUKU, (unsigned long)recovered_amount);
}

// Return pointer to battle attacker name buffer
unsigned short return_battle_attacker_address(void) {
    return (unsigned short)BATTLE_ATTACKER_NAME;
}

// Redirect to hypnosis alpha action
void redirect_btlact_hypnosis_a(void) {
    btlact_hypnosis_a();
}

// PSI Fire Gamma battle action
void btlact_psi_fire_gamma(void) {
    psi_fire_common(FIRE_GAMMA_DAMAGE);
}

// Lifeup Gamma battle action  
void btlact_lifeup_gamma(void) {
    lifeup_common(LIFEUP_GAMMA_HEALING);
}

// PSI Freeze Beta battle action
void btlact_psi_freeze_beta(void) {
    psi_freeze_common(FREEZE_BETA_DAMAGE);
}

// PSI Freeze Gamma battle action
void btlact_psi_freeze_gamma(void) {
    psi_freeze_common(FREEZE_GAMMA_DAMAGE);
}

// Insecticide spray battle action (100 damage)
void btlact_insecticide_spray(void) {
    insect_spray_common(100);
}

// Twenty-five percent variance calculation
unsigned short twenty_five_percent_variance(unsigned short value) {
    unsigned char rand1, rand2;
    signed char diff1, diff2;
    unsigned char abs_diff1, abs_diff2;
    unsigned short result;
    unsigned char variance;
    
    // Generate two random bytes
    rand1 = rand_long() & 0xFF;
    rand2 = rand_long() & 0xFF;
    
    // Convert to signed differences from 128
    diff1 = (signed char)(rand1 - 128);
    diff2 = (signed char)(rand2 - 128);
    
    // Get absolute values
    abs_diff1 = (diff1 < 0) ? -diff1 : diff1;
    abs_diff2 = (diff2 < 0) ? -diff2 : diff2;
    
    // Use the smaller absolute difference
    if (abs_diff1 <= abs_diff2) {
        variance = abs_diff1;
        // If original rand1 was < 128, subtract; if >= 128, add
        if (rand1 < 128) {
            result = value - (variance >> 1); // /2 for 25% vs 50%
        } else {
            result = value + (variance >> 1);
        }
    } else {
        variance = abs_diff2;
        // If original rand2 was < 128, subtract; if >= 128, add  
        if (rand2 < 128) {
            result = value - (variance >> 1);
        } else {
            result = value + (variance >> 1);
        }
    }
    
    return result;
}

// Common lifeup healing function with 25% variance
void lifeup_common(unsigned short healing_amount) {
    unsigned short varied_amount;
    
    // Apply 25% variance to healing amount
    varied_amount = twenty_five_percent_variance(healing_amount);
    
    // Recover HP for current target
    recover_hp(CURRENT_TARGET, varied_amount);
}

// PSI Freeze Omega battle action
void btlact_psi_freeze_omega(void) {
    psi_freeze_common(FREEZE_OMEGA_DAMAGE);
}

// PSI Thunder Gamma battle action
void btlact_psi_thunder_gamma(void) {
    psi_thunder_common(THUNDER_GAMMA_DAMAGE, THUNDER_GAMMA_HITS);
}

// Yogurt Dispenser - random 1-4 damage with speed success check
void btlact_yogurt_dispenser(void) {
    unsigned short damage;
    
    if (success_speed(250)) {
        // Random 1-4 damage, 255 resistance means no elemental resistance
        damage = rand_limit(4) + 1;
        calc_resist_damage(damage, 255);
    } else {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
    }
}

// Solidify status effect attack
void btlact_solidify(void) {
    if (fail_attack_on_npcs()) {
        return;
    }
    
    if (success_luck80()) {
        if (inflict_status_battle(CURRENT_TARGET, STATUS_GROUP_TEMPORARY, STATUS_2_SOLIDIFIED)) {
            display_battle_text_ptr(MSG_BTL_KOORI_ON);
        }
    } else {
        display_battle_text_ptr(MSG_BTL_KIKANAI);
    }
}

// Xterminator Spray - 200 damage to insects
void btlact_xterminator_spray(void) {
    insect_spray_common(200);
}

// Super Bomb - 270 damage
void btlact_super_bomb(void) {
    bomb_common(270);
}

