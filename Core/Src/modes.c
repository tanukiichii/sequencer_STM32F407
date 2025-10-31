#include "modes.h"

static SEQ_MODE current_mode = MUTE;
static uint8_t edit_mode_sample = 0;

static uint32_t mode_button_press_time = 0;
static uint8_t mode_button_was_pressed = 0;

static uint32_t last_mode_change_time = 0;

static uint16_t fx_held_buttons = 0;
static uint32_t fx_button_press_time[16] = {0};

void mode_handler_init(void)
{
    current_mode = MUTE;
    mode_button_press_time = 0;
    mode_button_was_pressed = 0;
    last_mode_change_time = 0;
}


SEQ_MODE get_current_mode(void)
{
    return current_mode;
}


void set_current_mode(SEQ_MODE new_mode) 
{
    if(new_mode != current_mode && (HAL_GetTick() - last_mode_change_time) > MODE_CHANGE_DEBOUNCE_MS) 
    {
        SEQ_MODE old_mode = current_mode;
        current_mode = new_mode;
        last_mode_change_time = HAL_GetTick();
        
        handle_mode_transition(old_mode, new_mode);
    }
}

void handle_mode_transition(SEQ_MODE old_mode, SEQ_MODE new_mode)
{
    if(old_mode == EDIT) 
    {
        exit_sample_edit_mode();
    }    
    
    switch (new_mode) 
    {
        case MUTE:
            stop_pattern_playback();
            LEDS_AllOff();
            break;

        case EDIT:
            start_pattern_playback();
            LEDS_AllOff();
            break;
            
        case PLAY:
            start_pattern_playback();
            break;
            
        case REC:
            start_pattern_playback();
            break;
            
        case FX:
            start_pattern_playback();
            break;
    }
}


void handle_mute_mode(uint8_t button_id)
{
    // Do nothing in mute mode
    // No sound output, no LED indication
}

void handle_edit_mode(uint8_t button_id)
{
  if (!is_editing_sample()) 
    {
        enter_sample_edit_mode(button_id);
        
        uint8_t editing_sample = get_editing_sample_id();
        play_sample(editing_sample);
        
        for(int i = 0; i < NUM_SLOTS; i++)
        {
            if(is_sample_in_slot(i, editing_sample))
            {
                LEDS_On(i + 1);
            }
        }
        
        HAL_Delay(300);
    }
    else 
    {
        stop_sample_blinking();
        uint8_t editing_sample = get_editing_sample_id();
        
        if (is_sample_in_slot(button_id, editing_sample)) 
        {
            remove_sample_from_slot(button_id, editing_sample);
            LEDS_Off(button_id + 1);
        }
        else 
        {
            record_sample_to_slot(button_id, editing_sample);
            LEDS_On(button_id + 1);
        }
    }
}

void handle_play_mode(uint8_t button_id)
{
    play_sample(button_id);    
}

void handle_rec_mode(uint8_t button_id)
{
    record_sample_to_slot(current_slot, button_id);
    
    play_sample(button_id);    
}
  
void handle_fx_mode(uint8_t button_id)
{
    if (!(fx_held_buttons & (1 << button_id))) 
    {
        fx_held_buttons |= (1 << button_id);
        fx_button_press_time[button_id] = HAL_GetTick();
        
        if (button_id == 0) 
        {
            reverb.enabled = 1;
        }        
    }
}

void handle_fx_button_release(uint8_t button_id)
{
    if (fx_held_buttons & (1 << button_id)) 
    {
        fx_held_buttons &= ~(1 << button_id);
        
        if (button_id == 0) 
        {
            reverb.enabled = 0;
        }        
    }
}

void handle_button_press(uint8_t button_id, SEQ_MODE mode)
{
    switch (mode) 
    {
        case MUTE:
            handle_mute_mode(button_id);
            break;
            
        case EDIT:
            handle_edit_mode(button_id);
            break;
            
        case PLAY:
            handle_play_mode(button_id);
            break;
            
        case REC:
            handle_rec_mode(button_id);
            break;
        case FX:
            handle_fx_mode(button_id);
    }
}