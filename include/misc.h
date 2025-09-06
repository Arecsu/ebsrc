#ifndef MISC_H
#define MISC_H

// Miscellaneous game functions

// Money and ATM functions
unsigned short decrease_wallet_balance(unsigned long amount);
unsigned long deposit_into_atm(unsigned long amount);

// Inventory functions  
unsigned short find_inventory_space(unsigned short character);

// Equipment functions
unsigned short change_equipped_weapon(unsigned short character, unsigned short new_weapon);
unsigned short change_equipped_arms(unsigned short character, unsigned short new_arms);
unsigned short change_equipped_body(unsigned short character, unsigned short new_body);
unsigned short change_equipped_other(unsigned short character, unsigned short new_other);

// Forward declarations for external functions
extern void recalc_character_postmath_offense(unsigned short character);
extern void recalc_character_postmath_defense(unsigned short character);
extern void recalc_character_postmath_guts(unsigned short character);
extern void recalc_character_postmath_luck(unsigned short character);
extern void recalc_character_miss_rate(unsigned short character);
extern void calc_resistances(unsigned short character);
unsigned short check_item_equipped(unsigned char character_id, unsigned char item_slot);

#endif