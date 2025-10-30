#include "buttons.h"
#include "modes.h"
#include "tm1638.h"

static uint16_t last_key_state = 0;
static uint32_t mode_button_press_time = 0;
static uint8_t mode_button_was_pressed = 0;

uint8_t is_mode_button_pressed(void)
{
    static uint8_t last_state = 0;
    static uint32_t last_change_time = 0;
    
    GPIO_PinState pin_state = HAL_GPIO_ReadPin(MODE_GPIO, MODE_Pin);
    
    uint8_t current_state = (pin_state == GPIO_PIN_SET); 
    
    uint32_t current_time = HAL_GetTick();
    
    if(current_state != last_state) 
    {
        if(current_time - last_change_time > MODE_CHANGE_DEBOUNCE_MS) 
        {
            last_state = current_state;
            last_change_time = current_time;
        }
    }
    else 
    {
        last_change_time = current_time;
    }
    
    return last_state;
}

uint8_t is_mode_button_held(void)
{
    static uint8_t button_held = 0;
    uint8_t button_pressed = is_mode_button_pressed();
    
    if(button_pressed && !button_held) 
    {
        if(HAL_GetTick() - mode_button_press_time > HOLD_TIME_MS) 
        {
            button_held = 1;
            return 1;
        }
    } 
    else if (!button_pressed) 
    {
        button_held = 0;
    }
    
    return 0;
} 

void handle_mode_button(void)
{
    static uint8_t last_button_state = 0;
    uint8_t current_button_state = is_mode_button_pressed();
    uint32_t current_time = HAL_GetTick();
    
    if(current_button_state && !last_button_state) 
    {
        mode_button_press_time = current_time;
        mode_button_was_pressed = 1;
    }
    
    if(!current_button_state && last_button_state) 
    {
        mode_button_was_pressed = 0;
        uint32_t press_duration = current_time - mode_button_press_time;
        
        if(press_duration < HOLD_TIME_MS) 
        {
            SEQ_MODE current_mode = get_current_mode();
            
            if(current_mode == MUTE) 
            {
                set_current_mode(PLAY);
            }
            else if(current_mode == PLAY) 
            {
                set_current_mode(EDIT);
            }
            else if(current_mode == EDIT) 
            {
                set_current_mode(PLAY);
            }
        }
    }
    
    if(current_button_state && get_current_mode() != REC) 
    {
        uint32_t press_duration = current_time - mode_button_press_time;
        
        if(press_duration >= HOLD_TIME_MS) 
        {
            set_current_mode(REC);
        }
    }
    
    if(!current_button_state && get_current_mode() == REC) 
    {
        set_current_mode(PLAY);
    }
    
    last_button_state = current_button_state;
}

void handle_tm1638_buttons(void)
{
    static uint32_t last_read_time = 0;
    uint32_t current_time = HAL_GetTick();
    
    if(current_time - last_read_time < 50) {
        return;
    }
    last_read_time = current_time;
    
    uint16_t current_keys = TM1638_ReadKeys();
    uint16_t key_changes = current_keys ^ last_key_state;
    SEQ_MODE current_mode = get_current_mode();
    
    if(key_changes & current_keys) 
    {
        for(uint8_t i = 0; i < 16; i++) {
            if((key_changes & (1 << i)) && (current_keys & (1 << i))) 
            {
                handle_button_press(i, current_mode);
            }
        }
    }
    
    last_key_state = current_keys;
}

void handle_clear_button (void)
{
  
  if(HAL_GPIO_ReadPin(BTN_GPIO, BTN_Pin))
  {
    SEQ_MODE current_mode = get_current_mode();
    
    if(current_mode == EDIT)
    {
      exit_sample_edit_mode();
      LEDS_AllOff();
    }
    else
    {
      init_pattern();
    }
  }     
}