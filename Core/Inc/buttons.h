#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include <stdint.h>
#include "stm32f4xx_hal.h"

#define MODE_GPIO GPIOB
#define MODE_Pin GPIO_PIN_11

#define BTN_GPIO GPIOB
#define BTN_Pin GPIO_PIN_10

#define FX_GPIO GPIOB
#define FX_Pin GPIO_PIN_12

//Info functions
uint8_t is_mode_button_pressed(void);
uint8_t is_mode_button_held(void);
uint8_t is_fx_button_pressed(void);

//Handle buttons functions
void handle_mode_button(void);
void handle_tm1638_buttons(void);
void handle_clear_button (void);
void handle_fx_button(void);

//combo info
uint8_t is_combo_press_detected(void);
uint8_t is_mode_button_held(void);
uint8_t get_combo_sample_id(void);

#endif //BUTTONS_H