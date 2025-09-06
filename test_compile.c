#include "include/hardware.h"
#include "include/audio.h"
#include "include/battle.h"
#include "include/system.h"
#include "include/misc.h"
#include "include/overworld.h"
#include "include/inventory.h"
#include "include/text.h"
#include "include/psi.h"

// Test compilation by referencing some of our converted functions
int main() {
    // Test that all headers include properly
    // Note: We won't actually call these functions as they depend on 
    // hardware-specific globals that don't exist in a test environment
    
    // Just reference the functions to ensure they compile
    void (*audio_funcs[])(void) = {
        pause_music,
        resume_music,
        stop_music
    };
    
    void (*battle_funcs[])(void) = {
        btlact_null,
        btlact_hp_recovery_50,
        btlact_pp_recovery_20
    };
    
    void (*system_funcs[])(void) = {
        redirect_stop_music,
        wait_dma_finished
    };
    
    void (*overworld_funcs[])(void) = {
        reset_mushroomized_walking,
        actionscript_animated_background_callback,
        clear_current_entity_collision
    };
    
    // Test inventory functions (need to cast since they return values)
    get_item_subtype(1);
    get_item_subtype_2(1);
    
    void (*text_funcs[])(void) = {
        clear_instant_printing,
        unlock_input,
        clear_blinking_prompt
    };
    
    (void)text_funcs;
    
    // Suppress unused variable warnings
    (void)audio_funcs;
    (void)battle_funcs; 
    (void)system_funcs;
    (void)overworld_funcs;
    
    return 0;
}