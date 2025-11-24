#include "leds.h"

static uint8_t current_slot = 0;
static uint8_t active_samples[16] = {0};

void LEDS_Init(void) 
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    GPIO_InitStruct.Pin = LED1_Pin | LED2_Pin | LED3_Pin | LED4_Pin
                          | LED6_Pin | LED7_Pin | LED8_Pin |
                         LED9_Pin | LED10_Pin | LED11_Pin | LED12_Pin |
                         LED13_Pin | LED14_Pin | LED15_Pin | LED16_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
    
    LEDS_AllOff();
}

void LEDS_Set(uint8_t led_num, uint8_t state) 
{
    if (led_num < 1 || led_num > 16) return;
    
    GPIO_PinState pin_state = state ? GPIO_PIN_SET : GPIO_PIN_RESET;
    
    if (led_num == 5)
    {
      HAL_GPIO_WritePin(LED1_GPIO_Port, LED5_Pin, pin_state);
    }
    else
    {
      uint16_t pin_mask = 1 << (led_num - 1);
      HAL_GPIO_WritePin(LED_GPIO_Port, pin_mask, pin_state);
    }
}

void LEDS_On(uint8_t led_num) 
{
    LEDS_Set(led_num, 1);
}

void LEDS_Off(uint8_t led_num) 
{
    LEDS_Set(led_num, 0);
}

void LEDS_SetAll(uint16_t pattern) 
{
    for (uint8_t i = 0; i < 16; i++) 
    {
        LEDS_Set(i + 1, (pattern >> i) & 0x01);
    }
}

void LEDS_AllOn(void) 
{
    LEDS_SetAll(0xFFFF);
}

void LEDS_AllOff(void) 
{
    LEDS_SetAll(0x0000);
}

void LEDS_IndicateSlot(uint8_t slot) 
{
    if (slot >= 16) return;
    
    current_slot = slot;
    
    SEQ_MODE current_mode = get_current_mode();
    
    if(current_mode == EDIT && is_editing_sample()) 
    {
        LEDS_AllOff();
        LEDS_On(get_editing_sample_id() + 1);
        for (uint8_t i = 0; i < 16; i++) 
        {
            if (is_sample_in_slot(i, get_editing_sample_id())) 
            {
                LEDS_On(i + 1);
            }
        }
        LEDS_On(slot + 1);
    }
    else 
    {
        LEDS_AllOff();
        LEDS_On(slot + 1);
        
        for (uint8_t i = 0; i < 16; i++) 
        {
            if (active_samples[i]) 
            {
                LEDS_On(i + 1);
            }
        }
    }
}

void LEDS_IndicateSampleTrigger(uint8_t sample_id) 
{
    if (sample_id >= 16) return;
    
    LEDS_On(sample_id + 1);
    HAL_Delay(150);
    LEDS_Off(sample_id + 1);    
}

void update_edit_mode_leds(void)
{
    static uint32_t last_blink_time = 0;
    static uint8_t blink_state = 0;
    uint32_t current_time = HAL_GetTick();
    
    if(is_editing_sample())
    {
        uint8_t editing_sample = get_editing_sample_id();
        
        if(is_sample_blinking()) 
        {
            if(current_time - last_blink_time > 500) 
            {
                last_blink_time = current_time;
                blink_state = !blink_state;
                
                LEDS_Off(editing_sample + 1);
                if(blink_state) 
                {
                    LEDS_On(editing_sample + 1);
                }
            }
        }
        else 
        {
            //LEDS_Off(editing_sample + 1);
            last_blink_time = current_time; 
            blink_state = 0;
        }
    }
    else 
    {
        last_blink_time = current_time;
        blink_state = 0;
    }
}


