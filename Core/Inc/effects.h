
#ifndef EFFECTS_H
#define EFFECTS_H

#include "main.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define REVERB_BUFFER_SIZE 2400  
#define REVERB_DELAY 1000       
#define REVERB_FEEDBACK 0.7f     

typedef struct {
    float buffer[REVERB_BUFFER_SIZE];
    uint32_t write_index;
    uint32_t read_index;
    float wet_level;
    uint8_t enabled;
} Reverb;

extern Reverb reverb;

void Reverb_Init(void);
float Reverb_Process(float input_sample);
void Reverb_SetWetLevel(float level);
void Reverb_Toggle(void);

#endif

