#include "platform/input.h"
#include <string.h>

// Input state variables (compatible with SNES code)
unsigned short PAD_RAW = 0;
unsigned short PAD_RAW_2 = 0;
unsigned short JOYPAD_1_DATA = 0;
unsigned short JOYPAD_2_DATA = 0;

// Previous frame input state for detecting "just pressed"
static unsigned short prev_joypad_1 = 0;
static unsigned short prev_joypad_2 = 0;

void input_init(void) {
    PAD_RAW = 0;
    PAD_RAW_2 = 0;
    JOYPAD_1_DATA = 0;
    JOYPAD_2_DATA = 0;
    prev_joypad_1 = 0;
    prev_joypad_2 = 0;
}

void handle_key_down(SDL_Keycode key) {
    unsigned short button = 0;
    
    // Map keyboard keys to EarthBound buttons
    switch (key) {
        case SDLK_z:     button = EB_BUTTON_A; break;      // Z = A button
        case SDLK_x:     button = EB_BUTTON_B; break;      // X = B button
        case SDLK_a:     button = EB_BUTTON_X; break;      // A = X button
        case SDLK_s:     button = EB_BUTTON_Y; break;      // S = Y button
        case SDLK_q:     button = EB_BUTTON_L; break;      // Q = L button
        case SDLK_w:     button = EB_BUTTON_R; break;      // W = R button
        case SDLK_BACKSPACE: button = EB_BUTTON_SELECT; break;
        case SDLK_RETURN: button = EB_BUTTON_START; break;
        case SDLK_UP:    button = EB_BUTTON_UP; break;
        case SDLK_DOWN:  button = EB_BUTTON_DOWN; break;
        case SDLK_LEFT:  button = EB_BUTTON_LEFT; break;
        case SDLK_RIGHT: button = EB_BUTTON_RIGHT; break;
        default: return;
    }
    
    JOYPAD_1_DATA |= button;
    PAD_RAW = JOYPAD_1_DATA;
}

void handle_key_up(SDL_Keycode key) {
    unsigned short button = 0;
    
    switch (key) {
        case SDLK_z:     button = EB_BUTTON_A; break;
        case SDLK_x:     button = EB_BUTTON_B; break;
        case SDLK_a:     button = EB_BUTTON_X; break;
        case SDLK_s:     button = EB_BUTTON_Y; break;
        case SDLK_q:     button = EB_BUTTON_L; break;
        case SDLK_w:     button = EB_BUTTON_R; break;
        case SDLK_BACKSPACE: button = EB_BUTTON_SELECT; break;
        case SDLK_RETURN: button = EB_BUTTON_START; break;
        case SDLK_UP:    button = EB_BUTTON_UP; break;
        case SDLK_DOWN:  button = EB_BUTTON_DOWN; break;
        case SDLK_LEFT:  button = EB_BUTTON_LEFT; break;
        case SDLK_RIGHT: button = EB_BUTTON_RIGHT; break;
        default: return;
    }
    
    JOYPAD_1_DATA &= ~button;
    PAD_RAW = JOYPAD_1_DATA;
}

void update_input_state(void) {
    // Store previous frame state
    prev_joypad_1 = JOYPAD_1_DATA;
    prev_joypad_2 = JOYPAD_2_DATA;
    
    // PAD_RAW reflects current frame input
    PAD_RAW = JOYPAD_1_DATA;
    PAD_RAW_2 = JOYPAD_2_DATA;
}

unsigned short get_joypad_state(int player) {
    return (player == 1) ? JOYPAD_1_DATA : JOYPAD_2_DATA;
}

bool is_button_pressed(int player, eb_button_t button) {
    unsigned short state = get_joypad_state(player);
    return (state & button) != 0;
}

bool is_button_just_pressed(int player, eb_button_t button) {
    unsigned short current = get_joypad_state(player);
    unsigned short previous = (player == 1) ? prev_joypad_1 : prev_joypad_2;
    
    return ((current & button) != 0) && ((previous & button) == 0);
}