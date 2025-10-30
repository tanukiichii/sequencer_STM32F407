#include "audio.h"
extern DAC_HandleTypeDef hdac;
// ?????????? ????????? ?????
SampleState audio_state = {0, 0, 0};

// ?????? ???? ???????
const SampleInfo samples[NUM_SAMPLES] = {
    {12000, play_kick},           // 1. ???-?????
    {10000, play_techno_kick},    // 2. ??????????? ???-?????
    {8000,  play_snare},          // 3. ???????????? ???????
    {6000,  play_electro_snare},  // 4. ??????????? ?????
    {10000, play_high_tom},       // 5. ???-??? ???????
    {12000, play_mid_tom},        // 6. ???-??? ???????
    {15000, play_low_tom},        // 7. ???-??? ??????
    {3000,  play_closed_hihat},   // 8. ???????? ???-???
    {8000,  play_open_hihat},     // 9. ???????? ???-???
    {20000, play_crash},          // 10. ????-???????
    {15000, play_ride},           // 11. ????-???????
    {2000,  play_claves},         // 12. ?????
    {4000,  play_side_stick},     // 13. ????? ???????
    {6000,  play_tambourine},     // 14. ????????
    {8000,  play_cowbell},        // 15. ???????
    {12000, play_conga}           // 16. ?????
};

void play_sample(uint8_t sample_number) 
{
    if(sample_number >= 0 && sample_number <= NUM_SAMPLES) 
    {
        uint8_t sample_idx = sample_number;
        audio_state.total_samples = samples[sample_idx].total_samples;
        audio_state.sample_index = 0;
        audio_state.is_playing = sample_number + 1;
    }
}

void stop_sample(void)
{
    audio_state.is_playing = 0;
    audio_state.sample_index = 0;
    audio_state.total_samples = 0;
}

void audio_callback(void)
{
    if(audio_state.is_playing == 0 || 
       audio_state.sample_index >= audio_state.total_samples)
    {
        stop_sample();
        return;
    }
    
    uint8_t sample_idx = audio_state.is_playing - 1;
    float sample = samples[sample_idx].play_func(audio_state.sample_index, 
                                                audio_state.total_samples);
    uint32_t dac_val = (uint32_t)((sample + 1.0f) * 127.0f);
    
    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_8B_R, dac_val);
    audio_state.sample_index++;
}

float play_kick(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-10.0f * t);
    float freq = 80.0f + 40.0f * expf(-15.0f * t);
    float sample = envelope * sinf(2.0f * PI * freq * t);
    
    if (sample_index < 50) {
        sample += 0.7f * sinf(2.0f * PI * 300.0f * t);
    }
    return sample;
}

float play_techno_kick(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-12.0f * t);
    float freq = 60.0f + 80.0f * expf(-20.0f * t);
    float sample = envelope * sinf(2.0f * PI * freq * t);
    
    if (sample_index < 30) {
        sample += 0.5f * sinf(2.0f * PI * 800.0f * t);
    }
    return sample;
}

float play_snare(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-6.0f * t);
    float body = 0.6f * envelope * sinf(2.0f * PI * 180.0f * t);
    float noise_env = expf(-8.0f * t);
    float noise = noise_env * ((float)(rand() % 200 - 100) / 100.0f);
    
    return body + noise;
}

float play_electro_snare(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-7.0f * t);
    float osc1 = sinf(2.0f * PI * 200.0f * t);
    float osc2 = sinf(2.0f * PI * 350.0f * t);
    float tone = envelope * (0.7f * osc1 + 0.3f * osc2);
    float noise = envelope * 0.4f * ((float)(rand() % 200 - 100) / 100.0f);
    
    return tone + noise;
}

float play_high_tom(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-6.0f * t);
    float freq = 200.0f - 60.0f * t;
    float sample = envelope * sinf(2.0f * PI * freq * t);
    
    if (sample_index < 150) {
        sample += envelope * 0.3f * ((float)(rand() % 200 - 100) / 100.0f);
    }
    return sample;
}

float play_mid_tom(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-5.5f * t);
    float freq = 150.0f - 40.0f * t;
    float sample = envelope * sinf(2.0f * PI * freq * t);
    
    if (sample_index < 120) {
        sample += envelope * 0.25f * ((float)(rand() % 200 - 100) / 100.0f);
    }
    return sample;
}

float play_low_tom(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-5.0f * t);
    float freq = 100.0f - 30.0f * t;
    float sample = envelope * sinf(2.0f * PI * freq * t);
    
    if (sample_index < 100) {
        sample += envelope * 0.2f * ((float)(rand() % 200 - 100) / 100.0f);
    }
    return sample;
}

float play_closed_hihat(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-15.0f * t);
    float freq1 = 6000.0f * (1.0f - 0.3f * t);
    float freq2 = 8000.0f * (1.0f - 0.4f * t);
    float sample = envelope * 0.5f * (sinf(2.0f * PI * freq1 * t) + 
                                     sinf(2.0f * PI * freq2 * t));
    sample += envelope * 0.3f * ((float)(rand() % 200 - 100) / 100.0f);
    
    return sample;
}

float play_open_hihat(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-4.0f * t);
    float freq1 = 5000.0f * (1.0f - 0.2f * t);
    float freq2 = 7000.0f * (1.0f - 0.25f * t);
    float freq3 = 9000.0f * (1.0f - 0.3f * t);
    float sample = envelope * 0.4f * (sinf(2.0f * PI * freq1 * t) + 
                                     sinf(2.0f * PI * freq2 * t) +
                                     sinf(2.0f * PI * freq3 * t));
    sample += envelope * 0.4f * ((float)(rand() % 200 - 100) / 100.0f);
    
    return sample;
}

float play_crash(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-2.5f * t);
    float bands[5] = {800.0f, 1500.0f, 3000.0f, 6000.0f, 8000.0f};
    float sample = 0;
    
    for (int i = 0; i < 5; i++) {
        float freq = bands[i] * (1.0f - 0.1f * t * (i + 1));
        sample += envelope * 0.15f * sinf(2.0f * PI * freq * t);
    }
    sample += envelope * 0.3f * ((float)(rand() % 200 - 100) / 100.0f);
    
    return sample;
}

float play_ride(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-3.0f * t);
    float freq1 = 1200.0f * (1.0f - 0.05f * t);
    float freq2 = 2500.0f * (1.0f - 0.1f * t);
    float freq3 = 4000.0f * (1.0f - 0.15f * t);
    float sample = envelope * 0.6f * (0.5f * sinf(2.0f * PI * freq1 * t) + 
                                     0.3f * sinf(2.0f * PI * freq2 * t) +
                                     0.2f * sinf(2.0f * PI * freq3 * t));
    sample += envelope * 0.1f * ((float)(rand() % 200 - 100) / 100.0f);
    
    return sample;
}

float play_claves(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-25.0f * t);
    float freq = 1200.0f;
    float sample = envelope * sinf(2.0f * PI * freq * t);
    sample += 0.3f * envelope * sinf(2.0f * PI * freq * 2.5f * t);
    
    return sample;
}

float play_side_stick(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-12.0f * t);
    float body = 0.7f * envelope * sinf(2.0f * PI * 400.0f * t);
    float noise = 0.3f * envelope * ((float)(rand() % 200 - 100) / 100.0f);
    
    return body + noise;
}

float play_tambourine(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-8.0f * t);
    float metal = envelope * 0.4f * (sinf(2.0f * PI * 2000.0f * t) + 
                                    sinf(2.0f * PI * 3000.0f * t));
    float shake = envelope * 0.6f * ((float)(rand() % 200 - 100) / 100.0f);
    
    return metal + shake;
}

float play_cowbell(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-6.0f * t);
    float freqs[4] = {500.0f, 750.0f, 1000.0f, 1500.0f};
    float amps[4] = {0.4f, 0.3f, 0.2f, 0.1f};
    float sample = 0;
    
    for (int i = 0; i < 4; i++) {
        sample += envelope * amps[i] * sinf(2.0f * PI * freqs[i] * t);
    }
    return sample;
}

float play_conga(uint32_t sample_index, uint32_t total_samples) {
    float t = (float)sample_index / F_SAMPLE;
    float envelope = expf(-5.0f * t);
    float freq1 = 150.0f * (1.0f - 0.1f * t);
    float freq2 = 300.0f * (1.0f - 0.15f * t);
    float freq3 = 500.0f * (1.0f - 0.2f * t);
    float sample = envelope * (0.5f * sinf(2.0f * PI * freq1 * t) + 
                             0.3f * sinf(2.0f * PI * freq2 * t) +
                             0.2f * sinf(2.0f * PI * freq3 * t));
    
    if (sample_index < 200) {
        sample += envelope * 0.2f * ((float)(rand() % 200 - 100) / 100.0f);
    }
    return sample;
}