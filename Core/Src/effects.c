#include "effects.h"

Reverb reverb;

void Reverb_Init(void)
{
    memset(reverb.buffer, 0, sizeof(reverb.buffer));
    reverb.write_index = 0;
    reverb.read_index = REVERB_BUFFER_SIZE - REVERB_DELAY;
    reverb.wet_level = 1.0f;  
    reverb.enabled = 0;     
}

float Reverb_Process(float input_sample)
{
    if (!reverb.enabled) 
    {
        return input_sample;
    }
    
    float delayed = reverb.buffer[reverb.read_index];
    
    float wet_signal = input_sample + delayed * REVERB_FEEDBACK;
    
    reverb.buffer[reverb.write_index] = wet_signal;
    
    reverb.write_index = (reverb.write_index + 1) % REVERB_BUFFER_SIZE;
    reverb.read_index = (reverb.read_index + 1) % REVERB_BUFFER_SIZE;
    
    return input_sample * (1.0f - reverb.wet_level) + wet_signal * reverb.wet_level;
}

void Reverb_Toggle(void)
{
    reverb.enabled = !reverb.enabled;
}

void Reverb_Enable(void)
{
    reverb.enabled = 1;
}

void Reverb_Disable(void)
{
    reverb.enabled = 0;
}