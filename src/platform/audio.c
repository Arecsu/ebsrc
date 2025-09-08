#include "platform/audio.h"
#include <stdio.h>

// Audio compatibility variables
unsigned short CURRENT_MUSIC_TRACK = 0;
unsigned char SOUND_EFFECT_QUEUE[8] = {0};
unsigned char SOUND_EFFECT_QUEUE_END_INDEX = 0;

bool audio_init(void) {
    printf("Audio system initialized\n");
    return true;
}

void audio_cleanup(void) {
    printf("Audio system cleaned up\n");
}

// Music functions
void play_music(int track_id) {
    printf("Playing music track: %d\n", track_id);
    CURRENT_MUSIC_TRACK = track_id;
}

void stop_music(void) {
    printf("Stopping music\n");
    CURRENT_MUSIC_TRACK = 0;
}

void pause_music(void) {
    printf("Pausing music\n");
}

void resume_music(void) {
    printf("Resuming music\n");
}

void set_music_volume(float volume) {
    printf("Setting music volume: %.2f\n", volume);
}

// Sound effects
void play_sound_effect(int sound_id) {
    printf("Playing sound effect: %d\n", sound_id);
}

void queue_sound_effect(int sound_id) {
    if (SOUND_EFFECT_QUEUE_END_INDEX < 8) {
        SOUND_EFFECT_QUEUE[SOUND_EFFECT_QUEUE_END_INDEX] = sound_id;
        SOUND_EFFECT_QUEUE_END_INDEX++;
        printf("Queued sound effect: %d\n", sound_id);
    }
}

void process_sound_queue(void) {
    // Process and play queued sound effects
    for (int i = 0; i < SOUND_EFFECT_QUEUE_END_INDEX; i++) {
        play_sound_effect(SOUND_EFFECT_QUEUE[i]);
    }
    SOUND_EFFECT_QUEUE_END_INDEX = 0; // Clear queue
}