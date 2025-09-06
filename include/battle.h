#ifndef BATTLE_H
#define BATTLE_H

// Battle system functions

// Battle actions
void btlact_null(void);
void btlact_null2(void);
void btlact_null3(void);
void btlact_null4(void);

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

// Forward declarations for external functions
extern unsigned short twenty_five_percent_variance(unsigned short value);
extern void recover_hp(unsigned char target, unsigned short amount);
extern void recover_pp(unsigned char target, unsigned short amount);
extern unsigned char CURRENT_TARGET;
extern void call_for_help_common(unsigned short param);
extern unsigned short fifty_percent_variance(unsigned short value);
extern void reduce_pp(unsigned short character_id, unsigned short amount);
extern void display_text_wait(const char* text, unsigned long param);

#endif