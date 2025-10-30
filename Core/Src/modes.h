#ifndef __MODES_H__
#define __MODES_H__

#include <stdint.h>
#include "audio.h"
#include "stm32f4xx_hal.h"
#include "pattern.h"
#include "tm1638.h"

#define HOLD_TIME_MS 200    
#define DOUBLE_CLICK_TIME_MS 300 
#define MODE_CHANGE_DEBOUNCE_MS 50 

typedef enum 
{
  MUTE,
  EDIT,
  PLAY,
  REC,
} SEQ_MODE;

//Mode initialization 
void mode_handler_init(void);

//Mode info func
SEQ_MODE get_current_mode(void);
void set_current_mode(SEQ_MODE new_mode);

//Handle modes functions
void handle_mode_transition(SEQ_MODE old_mode, SEQ_MODE new_mode);
void handle_mute_mode(uint8_t button_id);
void handle_edit_mode(uint8_t button_id);
void handle_play_mode(uint8_t button_id);
void handle_rec_mode(uint8_t button_id);
void handle_button_press(uint8_t button_id, SEQ_MODE mode);

#endif //MODES_H