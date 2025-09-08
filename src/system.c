// Modern SDL2-compatible system functions
#include "platform/graphics.h"
#include "platform/audio.h"
#include "hardware.h"
#include "system.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>

// Modern memory utility functions 
void memset24(unsigned char* dest, unsigned char value, unsigned short size) {
    for (unsigned short i = 0; i < size; i++) {
        dest[i] = value;
    }
}

unsigned short eb_strlen(const unsigned char* str) {
    unsigned short length = 0;
    while (str[length] != 0) {
        length++;
    }
    return length;
}

void memcpy16(unsigned short* dest, const unsigned short* src, unsigned short word_count) {
    for (unsigned short i = 0; i < word_count; i++) {
        dest[i] = src[i];
    }
}

void memset16(unsigned short* dest, unsigned char value, unsigned short word_count) {
    unsigned short word_value = (value << 8) | value;
    
    for (unsigned short i = 0; i < word_count; i++) {
        dest[i] = word_value;
    }
}

// Modern random number generator (preserving EarthBound's RNG characteristics)
static unsigned short rand_a = 0x1234;
static unsigned short rand_b = 0x5678;

unsigned char rand_hw(void) {
    // Modern C replacement maintaining the same mathematical properties
    unsigned short temp_mult = ((rand_a >> 8) & 0xFF) * (rand_b & 0xFF);
    
    rand_b = (rand_b + 0x6D) & 0xFFFF;
    
    // Preserve original bit manipulation for gameplay compatibility
    temp_mult = (temp_mult >> 2) & 0x3FFF;
    rand_a = ((temp_mult & 0x0003) + rand_a) >> 1;
    if ((rand_a & 1) == 0) {
        rand_a |= 0x8000;
    }
    
    return (temp_mult >> 2) & 0xFF;
}

unsigned short rand_0_3(void) {
    return rand_hw() & 0x0003;
}

unsigned short rand_0_7(void) {
    return rand_hw() & 0x0007;
}

unsigned short rand_limit(unsigned short limit) {
    if (limit == 0) return 0;
    return rand() % limit;
}

unsigned long rand_long(void) {
    return ((unsigned long)rand() << 16) | rand();
}

// Modern math utilities
unsigned short mult8(unsigned char a, unsigned char b) {
    return (unsigned short)a * (unsigned short)b;
}

unsigned short mult16(unsigned short a, unsigned short b) {
    return a * b;
}

unsigned short mult168(unsigned short a, unsigned short b) {
    return a * b;
}

unsigned short division16(unsigned short dividend, unsigned short divisor) {
    if (divisor == 0) return 0;
    return dividend / divisor;
}

// SDL2-compatible graphics operations
void wait_dma_finished(void) {
    // SDL2 handles operations synchronously - no waiting needed
}

void set_bg1_vram_location(unsigned char bg_size, unsigned short tilemap_base, unsigned short tile_base) {
    render_background_tile(1, tilemap_base, tile_base, bg_size);
    
    // Update compatibility variables
    BG1SC_MIRROR = (bg_size & 0x03) | ((tilemap_base >> 8) & 0xFC);
    BG12NBA_MIRROR = (BG12NBA_MIRROR & 0x0F) | ((tile_base >> 8) & 0xF0);
    BG1_X_POS = 0;
    BG1_Y_POS = 0;
}

void set_bg2_vram_location(unsigned char tilemap_config, unsigned short tilemap_addr, unsigned short tile_addr) {
    render_background_tile(2, tilemap_addr, tile_addr, tilemap_config);
    
    BG2SC_MIRROR = (tilemap_config & 0x03) | ((tilemap_addr >> 8) & 0xFC);
    BG12NBA_MIRROR = (BG12NBA_MIRROR & 0x0F) | ((tile_addr >> 8) & 0xF0);
    BG2_X_POS = 0;
    BG2_Y_POS = 0;
}

void set_bg3_vram_location(unsigned char bg_size, unsigned short tilemap_base, unsigned short tile_base) {
    render_background_tile(3, tilemap_base, tile_base, bg_size);
    
    BG3SC_MIRROR = (bg_size & 0x03) | ((tilemap_base >> 8) & 0xFC);
    BG34NBA_MIRROR = (BG34NBA_MIRROR & 0x0F) | ((tile_base >> 8) & 0xF0);
    BG3_X_POS = 0;
    BG3_Y_POS = 0;
}

void set_bg4_vram_location(unsigned char bg_size, unsigned short tilemap_base, unsigned short tile_base) {
    render_background_tile(4, tilemap_base, tile_base, bg_size);
    
    BG4SC_MIRROR = (bg_size & 0x03) | ((tilemap_base >> 8) & 0xFC);
    BG34NBA_MIRROR = (BG34NBA_MIRROR & 0xF0) | ((tile_base >> 8) & 0x0F);
    BG4_X_POS = 0;
    BG4_Y_POS = 0;
}

// SDL2-compatible display control
void set_inidisp(unsigned char value) {
    float brightness = (value & 0x0F) / 15.0f;
    set_screen_brightness(brightness);
}

void set_inidisp_far(unsigned char value) {
    set_inidisp(value);
}

// Modern graphics effects
void set_colour_addsub_mode(unsigned char cgwsel, unsigned char cgadsub) {
    apply_color_effects();
    (void)cgwsel; (void)cgadsub; // Compatibility
}

// Modern callback system
void reset_irq_callback(void) {
    IRQ_CALLBACK = DEFAULT_IRQ_CALLBACK;
}

void set_irq_callback(unsigned short callback_addr) {
    IRQ_CALLBACK = callback_addr;
}

// Audio system integration
void redirect_stop_music(void) {
    stop_music();
}

// SDL2-compatible fade system
void set_fade_params(unsigned char step, unsigned char delay) {
    FADE_PARAMETERS.step = step;
    FADE_PARAMETERS.delay = delay;
    FADE_DELAY_FRAMES_LEFT = delay;
}

void unknown_c0a1f2(unsigned char index) {
    // Palette animation function - converted to SDL2
    OVERWORLD_PALETTE_ANIM.timer = index;
    printf("Palette animation updated: index=%u\n", index);
}

// Simplified save system interface
unsigned short calc_save_block_xor_checksum(unsigned short slot) {
    // Modern save system would use standard file I/O
    // For now, return a basic checksum
    (void)slot; // Suppress warning
    return 0;
}

unsigned char sram_test(void) {
    // PC version doesn't need SRAM testing
    return 1; // Always available
}

// Modern initialization
void initialize_system(void) {
    printf("Initializing modern EarthBound system...\n");
    
    // Initialize SDL2 subsystems
    graphics_init(NULL); // Will be passed proper renderer later
    audio_init();
    
    // Initialize game state variables
    FADE_PARAMETERS.step = 0;
    FADE_PARAMETERS.delay = 0;
    FADE_DELAY_FRAMES_LEFT = 0;
    
    // Initialize random number generator
    srand((unsigned int)time(NULL));
    
    printf("System initialization complete\n");
}

// Additional utility functions commonly used in the game
void copy_memory(void* dest, const void* src, unsigned short size) {
    memcpy(dest, src, size);
}

void zero_memory(void* dest, unsigned short size) {
    memset(dest, 0, size);
}

// Modern sleep/delay function
void wait_frames(unsigned short frame_count) {
    // SDL2 frame timing - approximately 60fps
    unsigned int delay_ms = (frame_count * 1000) / 60;
    SDL_Delay(delay_ms);
}