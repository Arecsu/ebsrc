#include "include/hardware.h"
#include "include/misc.h"

// Forward declarations for external functions
extern void recalc_character_postmath_offense(unsigned short character);
extern void recalc_character_postmath_defense(unsigned short character);
extern void recalc_character_postmath_guts(unsigned short character);
extern void recalc_character_postmath_luck(unsigned short character);
extern void recalc_character_miss_rate(unsigned short character);
extern void calc_resistances(unsigned short character);

// Decrease wallet balance by specified amount
// Returns 1 if insufficient funds, 0 if successful
unsigned short decrease_wallet_balance(unsigned long amount) {
    if (GAME_STATE.money_carried < amount) {
        return 1; // Insufficient funds
    }
    GAME_STATE.money_carried -= amount;
    return 0; // Success
}

// Find inventory space for character
// Returns character index + 1 if space found, 0 if no space
unsigned short find_inventory_space(unsigned short character) {
    unsigned short slot;
    char_struct* party_member = &PARTY_CHARACTERS[character - 1];
    
    for (slot = 0; slot < 14; slot++) {
        if (party_member->items[slot] == 0) {
            return character;
        }
    }
    return FALSE;
}

// ATM deposit function
unsigned long deposit_into_atm(unsigned long amount) {
    unsigned long new_balance = GAME_STATE.bank_balance + amount;
    unsigned long actual_deposit;
    
    if (new_balance > ATM_ACCOUNT_LIMIT) {
        new_balance = ATM_ACCOUNT_LIMIT;
    }
    
    actual_deposit = new_balance - GAME_STATE.bank_balance;
    GAME_STATE.bank_balance = new_balance;
    
    return amount - actual_deposit;
}

// Change equipped weapon and recalculate stats
unsigned short change_equipped_weapon(unsigned short character, unsigned short new_weapon) {
    char_struct* party_member = &PARTY_CHARACTERS[character - 1];
    unsigned short old_weapon = party_member->equipment[WEAPON];
    
    party_member->equipment[WEAPON] = (unsigned char)new_weapon;
    
    // Recalculate character stats
    recalc_character_postmath_offense(character);
    recalc_character_postmath_guts(character);
    recalc_character_miss_rate(character);
    
    return old_weapon;
}

// Change equipped arms and recalculate stats
unsigned short change_equipped_arms(unsigned short character, unsigned short new_arms) {
    char_struct* party_member = &PARTY_CHARACTERS[character - 1];
    unsigned short old_arms = party_member->equipment[ARMS];
    
    party_member->equipment[ARMS] = (unsigned char)new_arms;
    
    // Recalculate character stats
    recalc_character_postmath_defense(character);
    recalc_character_postmath_luck(character);
    calc_resistances(character);
    
    return old_arms;
}

// Change equipped body and recalculate stats  
unsigned short change_equipped_body(unsigned short character, unsigned short new_body) {
    char_struct* party_member = &PARTY_CHARACTERS[character - 1];
    unsigned short old_body = party_member->equipment[BODY];
    
    party_member->equipment[BODY] = (unsigned char)new_body;
    
    // Recalculate character stats
    recalc_character_postmath_defense(character);
    recalc_character_postmath_luck(character);
    calc_resistances(character);
    
    return old_body;
}

// Change equipped other item and recalculate stats
unsigned short change_equipped_other(unsigned short character, unsigned short new_other) {
    char_struct* party_member = &PARTY_CHARACTERS[character - 1];
    unsigned short old_other = party_member->equipment[OTHER];
    
    party_member->equipment[OTHER] = (unsigned char)new_other;
    
    // Recalculate character stats
    recalc_character_postmath_defense(character);
    recalc_character_postmath_luck(character);
    calc_resistances(character);
    
    return old_other;
}

// Check if item is equipped by character
unsigned short check_item_equipped(unsigned char character_id, unsigned char item_slot) {
    char_struct* character = &PARTY_CHARACTERS[character_id - 1];
    
    // Check each equipment slot
    if (character->equipment[WEAPON] == item_slot) return 1;
    if (character->equipment[BODY] == item_slot) return 1;
    if (character->equipment[ARMS] == item_slot) return 1;
    if (character->equipment[OTHER] == item_slot) return 1;
    
    return 0;
}