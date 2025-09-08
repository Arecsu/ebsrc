#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Game headers - minimal demo
// #include "system.h"      // Excluded for demo
// #include "battle.h"      // Excluded for demo
// #include "overworld.h"   // Excluded for demo
#include "platform/graphics.h"
#include "platform/audio.h"
#include "platform/input.h"

// Screen dimensions (original SNES resolution)
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 224
#define SCALE_FACTOR 3

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
} SDLState;

static SDLState sdl_state = {0};

bool initialize_sdl() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return false;
    }

    // Initialize SDL_image
    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)) {
        printf("SDL_image initialization failed: %s\n", IMG_GetError());
        return false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer initialization failed: %s\n", Mix_GetError());
        return false;
    }

    // Create window
    sdl_state.window = SDL_CreateWindow("EarthBound",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH * SCALE_FACTOR, SCREEN_HEIGHT * SCALE_FACTOR,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    if (!sdl_state.window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return false;
    }

    // Create renderer
    sdl_state.renderer = SDL_CreateRenderer(sdl_state.window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (!sdl_state.renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        return false;
    }

    SDL_RenderSetLogicalSize(sdl_state.renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(sdl_state.renderer, 0, 0, 0, 255);

    return true;
}

void cleanup() {
    if (sdl_state.renderer) {
        SDL_DestroyRenderer(sdl_state.renderer);
    }
    if (sdl_state.window) {
        SDL_DestroyWindow(sdl_state.window);
    }
    
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

void handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                sdl_state.running = false;
                break;
                
            case SDL_KEYDOWN:
                // Handle input through our platform layer
                handle_key_down(event.key.keysym.sym);
                break;
                
            case SDL_KEYUP:
                handle_key_up(event.key.keysym.sym);
                break;
        }
    }
}

// Simple demo variables
static int demo_frame = 0;
static float demo_time = 0.0f;

void render_intro_demo() {
    // Clear screen with a dark blue background
    SDL_SetRenderDrawColor(sdl_state.renderer, 16, 32, 64, 255);
    SDL_RenderClear(sdl_state.renderer);
    
    // Draw animated background tiles
    for (int y = 0; y < 28; y++) {
        for (int x = 0; x < 32; x++) {
            int tile_id = ((x + y + demo_frame / 10) % 8);
            render_background_tile(0, x, y, tile_id);
        }
    }
    
    // Draw title text
    draw_text_simple(60, 50, "EARTHBOUND PC DEMO", 255, 255, 255);
    draw_text_simple(80, 70, "SDL2 VERSION", 200, 200, 255);
    
    // Draw some animated sprites
    for (int i = 0; i < 4; i++) {
        int sprite_x = 64 + (int)(40 * sin(demo_time + i * 1.5f));
        int sprite_y = 120 + (int)(20 * cos(demo_time * 0.7f + i));
        render_sprite(sprite_x, sprite_y, i, (demo_frame / 20 + i) % 4);
    }
    
    // Draw controls info
    draw_text_simple(40, 180, "CONTROLS:", 255, 255, 128);
    draw_text_simple(40, 195, "Z=A  X=B  ARROWS=MOVE", 192, 192, 128);
    draw_text_simple(40, 210, "PRESS ESC TO QUIT", 255, 128, 128);
    
    // Play some demo sounds occasionally
    if (demo_frame % 120 == 0) {
        play_sound_effect(1); // Demo sound effect
    }
    
    if (demo_frame % 180 == 0 && demo_frame > 0) {
        play_music(2); // Demo music track
    }
}

void game_loop() {
    // Update demo state
    demo_frame++;
    demo_time = demo_frame * 0.016f; // Approximate 60fps timing
    
    // Update input state
    update_input_state();
    
    // Handle game input
    if (is_button_just_pressed(1, EB_BUTTON_START)) {
        printf("START button pressed!\n");
        play_sound_effect(10);
    }
    
    if (is_button_just_pressed(1, EB_BUTTON_A)) {
        printf("A button pressed!\n");
        play_sound_effect(5);
    }
    
    // Check for ESC key to quit
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_ESCAPE]) {
        sdl_state.running = false;
    }
    
    // Render the intro demo
    render_intro_demo();
    
    // Present the frame
    SDL_RenderPresent(sdl_state.renderer);
}

int main(int argc, char* argv[]) {
    (void)argc; // Suppress unused parameter warning
    (void)argv;
    
    if (!initialize_sdl()) {
        cleanup();
        return 1;
    }

    printf("EarthBound PC - Starting...\n");
    
    // Initialize platform systems
    graphics_init(sdl_state.renderer);
    audio_init();
    input_init();
    
    printf("Platform systems initialized\n");
    
    sdl_state.running = true;
    
    // Main game loop
    while (sdl_state.running) {
        handle_events();
        game_loop();
        
        // Simple frame rate limiting
        SDL_Delay(16); // ~60 FPS
    }
    
    cleanup();
    printf("EarthBound PC - Exiting...\n");
    return 0;
}