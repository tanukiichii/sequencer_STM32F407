#ifndef BPM_H
#define BPM_H

#include "main.h"
#include "stm32f4xx_hal.h"

#define DEFAULT_BPM 120
#define MIN_BPM 60
#define MAX_BPM 240

extern ADC_HandleTypeDef hadc1;

uint16_t BPM_Get(void);
void BPM_UpdateFromADC(uint16_t adc_value);
uint16_t UpdateBPMFromPotentiometer(void);

#endif