#ifndef __LEDS_H__
#define __LEDS_H__

#include <stdint.h>
#include "stm32f4xx_hal.h"
#include "modes.h"

#define LED_GPIO_Port GPIOD
#define LED1_Pin GPIO_PIN_0
#define LED2_Pin GPIO_PIN_1
#define LED3_Pin GPIO_PIN_2
#define LED4_Pin GPIO_PIN_3
#define LED5_Pin GPIO_PIN_4
#define LED6_Pin GPIO_PIN_5
#define LED7_Pin GPIO_PIN_6
#define LED8_Pin GPIO_PIN_7
#define LED9_Pin GPIO_PIN_8
#define LED10_Pin GPIO_PIN_9
#define LED11_Pin GPIO_PIN_10
#define LED12_Pin GPIO_PIN_11
#define LED13_Pin GPIO_PIN_12
#define LED14_Pin GPIO_PIN_13
#define LED15_Pin GPIO_PIN_14
#define LED16_Pin GPIO_PIN_15

//Managing LEDs
void LEDS_Init(void);
void LEDS_Set(uint8_t led_num, uint8_t state);
void LEDS_On(uint8_t led_num);
void LEDS_Off(uint8_t led_num);
void LEDS_SetAll(uint16_t pattern);
void LEDS_AllOn(void);
void LEDS_AllOff(void);

//Sampler indication
void LEDS_IndicateSlot(uint8_t slot);
void LEDS_IndicateSampleTrigger(uint8_t sample_id); 
void update_edit_mode_leds(void);

#endif //LEDS