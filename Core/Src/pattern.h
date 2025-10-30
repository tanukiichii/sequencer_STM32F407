#ifndef PATTERN_H
#define PATTERN_H

#include "audio.h"
#include "main.h"
#include "modes.h"
#include "stm32f4xx_hal.h"
#include <string.h>

#define NUM_SLOTS 16
#define NUM_SAMPLES 16
#define MAX_SAMPLES_PER_SLOT 4  // ???????? ??????? ? ????? ?????

typedef struct 
{
    uint8_t sample_count[NUM_SLOTS];  // ?????????? ??????? ? ?????? ?????
    uint8_t samples[NUM_SLOTS][MAX_SAMPLES_PER_SLOT];  // ?????? ??? ??????? ?????
} Pattern;

extern Pattern current_pattern;
extern uint8_t current_slot;
extern uint8_t is_recording_pattern;
extern uint8_t is_playing_pattern;
static uint8_t editing_sample_id = 0;
static uint8_t is_editing_mode = 0;

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
void remove_all_samples_from_slot(uint8_t slot);
void play_samples_for_slot(uint8_t slot);
uint8_t is_sample_in_slot(uint8_t slot, uint8_t sample_id);
uint8_t get_sample_count_in_slot(uint8_t slot);

//Pattern editing functions
void enter_sample_edit_mode(uint8_t sample_id);
void exit_sample_edit_mode(void);
uint8_t is_editing_sample(void);
uint8_t get_editing_sample_id(void);

//Sample blinking
void stop_sample_blinking(void);
void start_sample_blinking(void);
uint8_t is_sample_blinking(void);

#endif // PATTERN_H