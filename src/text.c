#pragma code-name ("BANK07")

#include "hardware.h"
#include "text.h"

// Forward declarations
extern unsigned short get_active_window_address(void);

// Clear instant printing mode
void clear_instant_printing(void) {
    INSTANT_PRINTING = 0;
}

// Unlock text input
void unlock_input(void) {
    TEXT_PROMPT_WAITING_FOR_INPUT = 0;
}

// Clear blinking prompt
void clear_blinking_prompt(void) {
    BLINKING_TRIANGLE_FLAG = 0;
}

// Get blinking prompt state
unsigned short get_blinking_prompt(void) {
    return BLINKING_TRIANGLE_FLAG;
}

// Enable blinking triangle
void enable_blinking_triangle(void) {
    BLINKING_TRIANGLE_FLAG = 1;
}

// Get window focus
unsigned short get_window_focus(void) {
    return WINDOW_FOCUS;
}

// Set window focus
void set_window_focus(unsigned short window_id) {
    CURRENT_FOCUS_WINDOW = window_id;
}

// Set text sound mode
void set_text_sound_mode(unsigned short mode) {
    TEXT_SOUND_MODE = mode;
}

// Change current window font
void change_current_window_font(unsigned short font_id) {
    unsigned short window_index;
    unsigned short font_value;
    
    // Check if there's a focused window
    if (CURRENT_FOCUS_WINDOW == 0xFFFF) {
        return;
    }
    
    // Determine font value based on input
    if (font_id == 30) {  // WINDOW::UNKNOWN30 constant
        font_value = 0;
    } else {
        font_value = 1;
    }
    
    // Get window stats index and set font
    window_index = OPEN_WINDOW_TABLE[CURRENT_FOCUS_WINDOW];
    WINDOW_STATS[window_index].font = font_value;
}

// Set working memory for active window
unsigned long set_working_memory(unsigned long value) {
    unsigned short window_addr = get_active_window_address();
    unsigned long* working_memory_ptr = (unsigned long*)&WINDOW_STATS[window_addr].working_memory;
    
    *working_memory_ptr = value;
    return value;
}

// Increment secondary memory for active window
void increment_secondary_memory(void) {
    unsigned short window_addr = get_active_window_address();
    WINDOW_STATS[window_addr].secondary_memory++;
}