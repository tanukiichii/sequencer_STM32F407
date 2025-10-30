#include "bpm.h"

static uint16_t current_bpm = DEFAULT_BPM;
static uint16_t adc_raw_value = 0;
static uint16_t adc_value;

uint16_t BPM_Get(void)
{
    return current_bpm;
}

void BPM_UpdateFromADC(uint16_t adc_value)
{
    adc_raw_value = adc_value;
    
    float normalized = (float)adc_value / 4095.0f;
    current_bpm = MIN_BPM + (uint16_t)(normalized * (MAX_BPM - MIN_BPM));    
}

uint16_t UpdateBPMFromPotentiometer(void)
{
    static uint32_t last_adc_read = 0;
    
    if (HAL_GetTick() - last_adc_read > 50) 
    {
        last_adc_read = HAL_GetTick();
        
        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, 10);
        adc_value = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);
        
        BPM_UpdateFromADC(adc_value);
        current_bpm = BPM_Get();
    }
    
    return current_bpm;
}