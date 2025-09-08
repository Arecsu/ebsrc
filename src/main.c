#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Game headers
#include "system.h"
#include "battle.h"
#include "overworld.h"
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

void game_loop() {
    // Main game logic will go here
    // For now, just clear the screen
    SDL_SetRenderDrawColor(sdl_state.renderer, 32, 64, 128, 255);
    SDL_RenderClear(sdl_state.renderer);
    
    // TODO: Call game update functions
    // update_overworld();
    // update_battle();
    // render_game();
    
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
    
    // Initialize game systems
    // TODO: Add proper initialization
    
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