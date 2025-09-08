#ifndef PLATFORM_AUDIO_H
#define PLATFORM_AUDIO_H

#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

// Audio initialization
bool audio_init(void);
void audio_cleanup(void);

// Music functions (replaces SPC700 music system)
void play_music(int track_id);
void stop_music(void);
void pause_music(void);
void resume_music(void);
void set_music_volume(float volume);

// Sound effects
void play_sound_effect(int sound_id);
void queue_sound_effect(int sound_id);
void process_sound_queue(void);

// Audio compatibility for SNES variables
extern unsigned short CURRENT_MUSIC_TRACK;
extern unsigned char SOUND_EFFECT_QUEUE[8];
extern unsigned char SOUND_EFFECT_QUEUE_END_INDEX;

#endif