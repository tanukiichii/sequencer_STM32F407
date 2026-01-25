#ifndef AUDIO_H
#define AUDIO_H

#include "main.h"
#include <math.h>
#include <stdlib.h>

#define F_SAMPLE 50000.0f
#define PI 3.14159265358979323846f

#define NUM_SAMPLES 16

typedef struct 
{
    uint32_t total_samples;
    uint32_t sample_index;
    uint8_t is_playing;
} SampleState;

typedef struct 
{
    uint32_t total_samples;
    float (*play_func)(uint32_t, uint32_t);
} SampleInfo;

extern SampleState audio_state;
extern const SampleInfo samples[];

void play_sample(uint8_t sample_number);
void stop_sample(void);
void audio_callback(void);

float play_kick(uint32_t sample_index, uint32_t total_samples);
float play_techno_kick(uint32_t sample_index, uint32_t total_samples);
float play_snare(uint32_t sample_index, uint32_t total_samples);
float play_electro_snare(uint32_t sample_index, uint32_t total_samples);
float play_high_tom(uint32_t sample_index, uint32_t total_samples);
float play_mid_tom(uint32_t sample_index, uint32_t total_samples);
float play_low_tom(uint32_t sample_index, uint32_t total_samples);
float play_closed_hihat(uint32_t sample_index, uint32_t total_samples);
float play_open_hihat(uint32_t sample_index, uint32_t total_samples);
float play_crash(uint32_t sample_index, uint32_t total_samples);
float play_ride(uint32_t sample_index, uint32_t total_samples);
float play_claves(uint32_t sample_index, uint32_t total_samples);
float play_side_stick(uint32_t sample_index, uint32_t total_samples);
float play_tambourine(uint32_t sample_index, uint32_t total_samples);
float play_cowbell(uint32_t sample_index, uint32_t total_samples);
float play_conga(uint32_t sample_index, uint32_t total_samples);

#endif // AUDIO_H

