#ifndef PLATFORM_GRAPHICS_H
#define PLATFORM_GRAPHICS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Graphics initialization
bool graphics_init(SDL_Renderer* renderer);
void graphics_cleanup(void);

// Screen management
void clear_screen(void);
void present_screen(void);

// Basic drawing functions for demo
void draw_rectangle(int x, int y, int w, int h, int r, int g, int b);
void draw_text_simple(int x, int y, const char* text, int r, int g, int b);

// Sprite/tile rendering (will replace SNES PPU functions)
void render_sprite(int x, int y, int tile_id, int palette);
void render_background_tile(int layer, int x, int y, int tile_id);

// Color/palette management
void set_palette_color(int palette, int index, int r, int g, int b);
void apply_color_effects(void);

// Screen effects
void set_screen_brightness(float brightness);
void apply_screen_fade(int fade_level);

#endif