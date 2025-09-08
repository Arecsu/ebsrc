#include "platform/graphics.h"
#include <stdio.h>

static SDL_Renderer* g_renderer = NULL;

bool graphics_init(SDL_Renderer* renderer) {
    g_renderer = renderer;
    printf("Graphics system initialized\n");
    return true;
}

void graphics_cleanup(void) {
    printf("Graphics system cleaned up\n");
}

void clear_screen(void) {
    if (g_renderer) {
        SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);
        SDL_RenderClear(g_renderer);
    }
}

void present_screen(void) {
    if (g_renderer) {
        SDL_RenderPresent(g_renderer);
    }
}

// Basic drawing functions for demo
void draw_rectangle(int x, int y, int w, int h, int r, int g, int b) {
    if (g_renderer) {
        SDL_Rect rect = {x, y, w, h};
        SDL_SetRenderDrawColor(g_renderer, r, g, b, 255);
        SDL_RenderFillRect(g_renderer, &rect);
    }
}

void draw_text_simple(int x, int y, const char* text, int r, int g, int b) {
    // Simple text rendering using rectangles to form letters
    if (!g_renderer || !text) return;
    
    SDL_SetRenderDrawColor(g_renderer, r, g, b, 255);
    
    int char_width = 8;
    int char_height = 8;
    int current_x = x;
    
    for (const char* c = text; *c != '\0'; c++) {
        // Draw a simple rectangle for each character (placeholder)
        SDL_Rect char_rect = {current_x, y, char_width - 1, char_height - 1};
        SDL_RenderDrawRect(g_renderer, &char_rect);
        
        // Add a dot in the center to represent the character
        SDL_Rect dot = {current_x + 3, y + 3, 2, 2};
        SDL_RenderFillRect(g_renderer, &dot);
        
        current_x += char_width;
    }
}

// Sprite/tile rendering stubs
void render_sprite(int x, int y, int tile_id, int palette) {
    // For demo - draw a colored square representing the sprite
    int color = (tile_id * 40 + palette * 60) % 255;
    draw_rectangle(x, y, 16, 16, color, (color + 80) % 255, (color + 160) % 255);
}

void render_background_tile(int layer, int x, int y, int tile_id) {
    // For demo - draw background patterns
    int color = (layer * 30 + tile_id * 10) % 128 + 64; // Keep colors dim for background
    draw_rectangle(x * 8, y * 8, 8, 8, color/3, color/2, color);
}

// Color/palette management stubs
void set_palette_color(int palette, int index, int r, int g, int b) {
    // TODO: Implement palette color setting
    (void)palette; (void)index; (void)r; (void)g; (void)b;
}

void apply_color_effects(void) {
    // TODO: Implement color effects
}

// Screen effects stubs
void set_screen_brightness(float brightness) {
    // TODO: Implement screen brightness
    (void)brightness;
}

void apply_screen_fade(int fade_level) {
    // TODO: Implement screen fade
    (void)fade_level;
}