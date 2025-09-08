#ifndef BATTLE_H
#define BATTLE_H

#include "structs.h"

// Status effect constants
#define STATUS_0_UNCONSCIOUS 0x00
#define STATUS_0_COLD 0x01
#define STATUS_0_DIAMONDIZED 0x02
#define STATUS_0_PARALYZED 0x03
#define STATUS_0_SUNSTROKE 0x04
#define STATUS_0_POISONED 0x05
#define STATUS_0_NAUSEOUS 0x06
#define STATUS_1_MUSHROOMIZED 0x08
#define STATUS_2_ASLEEP 0x00
#define STATUS_2_CRYING 0x01
#define STATUS_2_SOLIDIFIED 0x02
#define STATUS_2_IMMOBILIZED 0x03
#define STATUS_3_STRANGE 0x04
#define STATUS_4_CANT_CONCENTRATE4 0x04
#define STATUS_GROUP_PERSISTENT_EASYHEAL 0x01
#define STATUS_GROUP_STRANGENESS 0x03
#define STATUS_GROUP_SHIELD 0x04
#define STATUS_GROUP_HOMESICKNESS 0x05
#define STATUS_GROUP_CONCENTRATION 0x06
#define STATUS_GROUP_TEMPORARY 0x02
#define STATUS_GROUP_PERSISTENT_HARDHEAL 0x07

// External variables for battle
extern unsigned long BATTLE_EXP_SCRATCH;
extern unsigned short BATTLE_MONEY_SCRATCH;

// Battle system functions

// Battle actions
void btlact_null(void);
void btlact_null2(void);
void btlact_null3(void);
void btlact_null4(void);
void btlact_redirect_null(void);
void btlact_level_2_attack_poison(void);
void bottle_rocket_common(unsigned short count);
void btlact_brainshock_alpha(void);
void btlact_brainshock_alpha_redirect_copy(void);
void btlact_lifeup_alpha(void);
void btlact_pray_mysterious(void);
void insect_spray_common(unsigned short damage);
void btlact_shield_beta(void);
void btlact_level_1_atk(void);
void btlact_hp_sucker(void);
void btlact_steal(void);

// HP recovery functions
void btlact_hp_recovery_10(void);
void btlact_hp_recovery_50(void);
void btlact_hp_recovery_100(void);
void btlact_hp_recovery_200(void);
void btlact_hp_recovery_300(void);
void btlact_hp_recovery_1d4(void);
void btlact_hp_recovery_10000(void);

// PP recovery functions
void btlact_pp_recovery_20(void);
void btlact_pp_recovery_80(void);

// Damage functions
void btlact_350_fire_damage(void);

// Attack functions
void btlact_bash(void);
void btlact_level_1_atk(void);
void btlact_level_2_atk(void);

// PSI functions
void btlact_psi_fire_b(void);
void btlact_healing_b(void);

// Item functions
void btlact_bottle_rocket(void);
void btlact_big_bottle_rocket(void);

// More battle actions
void btlact_reduce_pp(void);
void btlact_call_for_help(void);
void btlact_pray_warm(void);
void btlact_pray_golden(void);
void btlact_crying2(void);
void btlact_inflict_solidification(void);
void btlact_defense_down_a(void);
void btlact_switch_armor(void);
void btlact_lifeup_b(void);
void btlact_sow_seeds(void);
void redirect_btlact_brainshock_a(void);
void btlact_random_stat_up_1d4(void);
void btlact_switch_weapons(void);
void btlact_psi_fire_a(void);
void btlact_psi_rockin_b(void);
void redirect_btlact_psi_shield_b(void);
void redirect_btlact_level_3_atk(void);

// Forward declarations for external functions
extern unsigned short twenty_five_percent_variance(unsigned short value);
extern void apply_condiment(void);
extern void recover_hp(unsigned char target, unsigned short amount);
extern void recover_pp(unsigned char target, unsigned short amount);
extern unsigned short CURRENT_TARGET;
extern void call_for_help_common(unsigned short param);
extern unsigned short fifty_percent_variance(unsigned short value);
extern void reduce_hp(unsigned char target, unsigned short amount);
extern void set_hp(unsigned char target, unsigned short amount);
extern void reduce_pp(unsigned short character_id, unsigned short amount);
extern void set_pp(unsigned char character_id, unsigned short amount);
extern unsigned short autohealing(unsigned short status_id, unsigned short status_group);
extern unsigned short boss_battle_check(void);
extern void display_text_wait(const char* text, unsigned long param);
extern void target_all(void);
extern void target_allies(void);
extern void target_battler(unsigned short battler_id);
extern unsigned char success_500(unsigned short value);
extern unsigned char is_char_targetted(unsigned short battler_index);
extern void remove_target(unsigned short battler_index);
extern void remove_dead_targetting(void);
extern void decrease_offense_16th(unsigned short battler_ptr);
extern unsigned short return_battle_target_address(void);
extern void target_all_enemies(void);
extern unsigned char check_if_valid_target(unsigned short battler_index);
extern unsigned char get_shield_targetting(unsigned short action);
extern void swap_attacker_with_target(void);
extern void fix_attacker_name(unsigned short param);
extern void fix_target_name(void);
extern unsigned short miss_calc(unsigned short miss_message);
extern unsigned short get_battle_sprite_height(unsigned short sprite);
extern unsigned short count_chars(unsigned short side);
extern void recover_pp(unsigned char target, unsigned short amount);
extern unsigned short return_battle_attacker_address(void);
extern void redirect_btlact_hypnosis_a(void);
extern void btlact_psi_fire_gamma(void);
extern void btlact_lifeup_gamma(void);
extern void psi_fire_common(unsigned short damage);
extern void lifeup_common(unsigned short amount);
extern void btlact_hypnosis_a(void);
extern void btlact_psi_freeze_beta(void);
extern void btlact_psi_freeze_gamma(void);
extern void btlact_insecticide_spray(void);
extern void psi_freeze_common(unsigned short damage);
extern void psi_thunder_common(unsigned short damage, unsigned short hits);
extern void insect_spray_common(unsigned short damage);
extern void bomb_common(unsigned short damage);
extern unsigned char success_luck80(void);

// Item and enemy structures (basic definitions)
typedef struct {
    unsigned char special;
    // More fields TBD
} item_parameters;

typedef struct {
    item_parameters params;
    // More fields TBD  
} item;

typedef struct {
    unsigned char miss_rate;
    // More fields TBD
} enemy_data;

// External arrays
extern unsigned char ENEMY_CONFIGURATION_TABLE[];

// Battle constants
#define BATTLER_COUNT 32

// Party member IDs 
#define PARTY_MEMBER_NESS 1
#define PARTY_MEMBER_PAULA 2
#define PARTY_MEMBER_JEFF 3
#define PARTY_MEMBER_POO 4
#define PARTY_MEMBER_POKEY 5  // NPC party member

// Battle action constants (PSI Shield types)
#define BATTLE_ACTIONS_PSI_SHIELD_SIGMA 0x15A
#define BATTLE_ACTIONS_PSI_SHIELD_OMEGA 0x15B  
#define BATTLE_ACTIONS_PSI_PSI_SHIELD_SIGMA 0x15C
#define BATTLE_ACTIONS_PSI_PSI_SHIELD_OMEGA 0x15D

// PSI damage/healing constants from config.asm
#define FIRE_GAMMA_DAMAGE 240
#define FREEZE_BETA_DAMAGE 360
#define FREEZE_GAMMA_DAMAGE 540
#define FREEZE_OMEGA_DAMAGE 720
#define THUNDER_GAMMA_DAMAGE 120
#define THUNDER_GAMMA_HITS 3
#define LIFEUP_GAMMA_HEALING 10000

// Global battle variables
extern unsigned long BATTLER_TARGET_FLAGS;
extern unsigned long POWERS_OF_TWO_32BIT[32];

void btlact_double_bash(void);
void btlact_bomb(void);
void btlact_cold(void);
void btlact_diamondize(void);
void btlact_feel_strange(void);
void btlact_paralyze(void);
void btlact_poison(void);
void btlact_nauseate(void);
void btlact_mushroomize(void);
void btlact_crying(void);
void btlact_immobilize(void);
void btlact_neutralize(void);
void btlact_distract(void);
void btlact_inflict_poison(void);
void btlact_cut_guts(void);
void btlact_null1(void);
void btlact_luck_up_1d4(void);
void btlact_guts_up_1d4(void);
void btlact_iq_up_1d4(void);
void heal_poison(void);
void btlact_null5(void);
void btlact_null6(void);
void btlact_null10(void);
void btlact_null11(void);
void btlact_level_4_atk(void);
void btlact_null7(void);
void btlact_null12(void);

// Additional battle actions
void btlact_hp_recovery_10000(void);
void btlact_psi_freeze_omega(void);
void btlact_psi_thunder_gamma(void);
void btlact_yogurt_dispenser(void);
void btlact_solidify(void);
void btlact_xterminator_spray(void);
void btlact_pp_recovery_80(void);
void btlact_super_bomb(void);

// Food and item functions
void eat_food(void);
void instant_win_handler(void);
unsigned short autolifeup(void);

// Utility functions
void heal_strangeness(void);
void battle_init_enemy_stats(unsigned short enemy_id, battler* target);
unsigned short find_targettable_npc(void);

#endif