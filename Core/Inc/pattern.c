#include "pattern.h"

uint8_t current_slot = 0;
uint32_t sample_positions[NUM_SAMPLES] = {0};
uint8_t is_recording_pattern = 0;
uint8_t is_playing_pattern = 0;

static uint8_t blink_editing_sample = 1;

Pattern current_pattern;

void init_pattern(void) 
{
    memset(&current_pattern, 0, sizeof(Pattern));
}

void start_pattern_recording(void) 
{
    is_recording_pattern = 1;
    is_playing_pattern = 1;
    current_slot = 0;
}

void stop_pattern_recording(void) 
{
    is_recording_pattern = 0;
}

void start_pattern_playback(void) 
{
    is_playing_pattern = 1;
    //current_slot = 0;
}

void stop_pattern_playback(void) 
{
    is_playing_pattern = 0;
}

void clear_pattern(void) 
{
    memset(&current_pattern, 0, sizeof(Pattern));
}

void record_sample_to_slot(uint8_t slot, uint8_t sample_id) 
{
    if (slot < NUM_SLOTS && sample_id < NUM_SAMPLES) 
    {
        // ?????????, ??? ?? ??? ????? ?????? ? ?????
        for(uint8_t i = 0; i < current_pattern.sample_count[slot]; i++) {
            if(current_pattern.samples[slot][i] == sample_id) {
                return; // ????? ??? ???? ? ?????
            }
        }
        
        // ????????? ?????, ???? ???? ?????
        if(current_pattern.sample_count[slot] < MAX_SAMPLES_PER_SLOT) {
            current_pattern.samples[slot][current_pattern.sample_count[slot]] = sample_id;
            current_pattern.sample_count[slot]++;
        }
    }
}

void remove_sample_from_slot(uint8_t slot, uint8_t sample_id) 
{
    if (slot < NUM_SLOTS && sample_id < NUM_SAMPLES) 
    {
        for(uint8_t i = 0; i < current_pattern.sample_count[slot]; i++) {
            if(current_pattern.samples[slot][i] == sample_id) {
                // ???????? ?????????? ??????
                for(uint8_t j = i; j < current_pattern.sample_count[slot] - 1; j++) {
                    current_pattern.samples[slot][j] = current_pattern.samples[slot][j + 1];
                }
                current_pattern.sample_count[slot]--;
                break;
            }
        }
    }
}

void remove_all_samples_from_slot(uint8_t slot)
{
    if (slot < NUM_SLOTS) {
        current_pattern.sample_count[slot] = 0;
    }
}

void play_samples_for_slot(uint8_t slot) 
{
    if (slot >= NUM_SLOTS || !is_playing_pattern) return;
    
    // ????????????? ??? ?????? ? ?????
    for (uint8_t i = 0; i < current_pattern.sample_count[slot]; i++) {
        play_sample(current_pattern.samples[slot][i]);
    }
}

uint8_t is_sample_in_slot(uint8_t slot, uint8_t sample_id) 
{
    if (slot < NUM_SLOTS && sample_id < NUM_SAMPLES) 
    {
        for(uint8_t i = 0; i < current_pattern.sample_count[slot]; i++) {
            if(current_pattern.samples[slot][i] == sample_id) {
                return 1;
            }
        }
    }
    return 0;
}

uint8_t get_sample_count_in_slot(uint8_t slot)
{
    if (slot < NUM_SLOTS) {
        return current_pattern.sample_count[slot];
    }
    return 0;
}

void enter_sample_edit_mode(uint8_t sample_id)
{
    is_editing_mode = 1;
    editing_sample_id = sample_id;
    blink_editing_sample = 1;
}

void exit_sample_edit_mode(void)
{
    is_editing_mode = 0;
    editing_sample_id = 0;
    blink_editing_sample = 0;
    LEDS_AllOff();
}

uint8_t is_editing_sample(void)
{
    return is_editing_mode;
}

uint8_t get_editing_sample_id(void)
{
    return editing_sample_id;
}

void stop_sample_blinking(void)
{
    blink_editing_sample = 0;
}

void start_sample_blinking(void)
{
    blink_editing_sample = 1;
}

uint8_t is_sample_blinking(void)
{
    return blink_editing_sample;
}