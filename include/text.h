#ifndef TEXT_H
#define TEXT_H

// Text and dialogue system functions

// Text mode control
void clear_instant_printing(void);
void unlock_input(void);

// Blinking prompt control
void clear_blinking_prompt(void);
unsigned short get_blinking_prompt(void);
void enable_blinking_triangle(void);

// Window focus
unsigned short get_window_focus(void);
void set_window_focus(unsigned short window_id);

// Text sound control
void set_text_sound_mode(unsigned short mode);

// Window font control
void change_current_window_font(unsigned short font_id);

#endif