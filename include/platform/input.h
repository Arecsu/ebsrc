#ifndef PLATFORM_INPUT_H
#define PLATFORM_INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// EarthBound button mapping
typedef enum {
    EB_BUTTON_A = 0x01,
    EB_BUTTON_B = 0x02,
    EB_BUTTON_X = 0x04,
    EB_BUTTON_Y = 0x08,
    EB_BUTTON_L = 0x10,
    EB_BUTTON_R = 0x20,
    EB_BUTTON_SELECT = 0x40,
    EB_BUTTON_START = 0x80,
    EB_BUTTON_UP = 0x100,
    EB_BUTTON_DOWN = 0x200,
    EB_BUTTON_LEFT = 0x400,
    EB_BUTTON_RIGHT = 0x800
} eb_button_t;

// Input state
extern unsigned short PAD_RAW;
extern unsigned short PAD_RAW_2;
extern unsigned short JOYPAD_1_DATA;
extern unsigned short JOYPAD_2_DATA;

// Input functions
void input_init(void);
void handle_key_down(SDL_Keycode key);
void handle_key_up(SDL_Keycode key);
void update_input_state(void);

// Compatibility functions for SNES joypad emulation
unsigned short get_joypad_state(int player);
bool is_button_pressed(int player, eb_button_t button);
bool is_button_just_pressed(int player, eb_button_t button);

#endif