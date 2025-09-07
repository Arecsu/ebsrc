#ifndef BATTLE_H
#define BATTLE_H

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
extern void recover_hp(unsigned char target, unsigned short amount);
extern void recover_pp(unsigned char target, unsigned short amount);
extern unsigned char CURRENT_TARGET;
extern void call_for_help_common(unsigned short param);
extern unsigned short fifty_percent_variance(unsigned short value);
extern void reduce_pp(unsigned short character_id, unsigned short amount);
extern void display_text_wait(const char* text, unsigned long param);

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

#endif