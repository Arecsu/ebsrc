#ifndef SYSTEM_H
#define SYSTEM_H

// System utility functions

// Audio redirects
void redirect_stop_music(void);

// Memory utilities
void memset24(unsigned char* dest, unsigned char value, unsigned short size);
void memcpy16(unsigned short* dest, const unsigned short* src, unsigned short word_count);
void memset16(unsigned short* dest, unsigned char value, unsigned short word_count);

// String utilities
unsigned short eb_strlen(const unsigned char* str);

// DMA and hardware utilities
void wait_dma_finished(void);
void set_bg2_vram_location(unsigned char tilemap_config, unsigned short tilemap_addr, unsigned short tile_addr);

// IRQ callback management
void reset_irq_callback(void);
void set_irq_callback(unsigned short callback_addr);

// Graphics utilities
void set_colour_addsub_mode(unsigned char cgwsel, unsigned char cgadsub);
void set_inidisp_far(unsigned char value);

// Random number utilities
unsigned short rand_0_3(void);
unsigned short rand_0_7(void);

// Math utilities
unsigned short mult8(unsigned char a, unsigned char b);
unsigned short mult16(unsigned short a, unsigned short b);
unsigned short division16(unsigned short dividend, unsigned short divisor);
unsigned short division16s(signed short dividend, signed short divisor);
unsigned char modulus8(unsigned char dividend, unsigned char divisor);
unsigned short rand_limit(unsigned short limit);
unsigned short truncate_16_to_8(unsigned short value, unsigned short multiplier);

#endif