#ifndef PATTERN_H
#define PATTERN_H

#include "main.h"
#include "audio.h"
//#include "samples.h"
#include "stm32f4xx_hal.h"
//#include "timers.h"
#include <string.h>

#define NUM_SLOTS 16
#define NUM_SAMPLES 16

typedef struct 
{
    uint8_t activated_samples[NUM_SLOTS][NUM_SAMPLES]; 
} Pattern;

extern Pattern current_pattern;
extern uint8_t current_slot;
extern uint8_t is_recording_pattern;
extern uint8_t is_playing_pattern;

//Initialisation func
void init_pattern(void);

//Managing pattern's recording
void start_pattern_recording(void);
void stop_pattern_recording(void);

//Managing pattern's playing
void start_pattern_playback(void);
void stop_pattern_playback(void);

//Slot managing
void clear_pattern(void);
void record_sample_to_slot(uint8_t slot, uint8_t sample_id);
void remove_sample_from_slot(uint8_t slot, uint8_t sample_id);
void play_samples_for_slot(uint8_t slot);
uint8_t is_sample_in_slot(uint8_t slot, uint8_t sample_id);

#endif // PATTERN_H