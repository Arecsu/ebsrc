#include "include/hardware.h"
#include "include/system.h"

// Forward declarations for external functions  
extern void stop_music(void);

// Simple redirect function for stop music
void redirect_stop_music(void) {
    stop_music();
}

// Memory utility functions
void memset24(unsigned char* dest, unsigned char value, unsigned short size) {
    unsigned short i;
    for (i = 0; i < size; i++) {
        dest[i] = value;
    }
}

// String length function
unsigned short eb_strlen(const unsigned char* str) {
    unsigned short length = 0;
    while (str[length] != 0) {
        length++;
    }
    return length;
}

// 16-bit memory copy function
void memcpy16(unsigned short* dest, const unsigned short* src, unsigned short word_count) {
    unsigned short i;
    for (i = 0; i < word_count; i++) {
        dest[i] = src[i];
    }
}

// 16-bit memory set function 
void memset16(unsigned short* dest, unsigned char value, unsigned short word_count) {
    unsigned short i;
    unsigned short word_value = (value << 8) | value;  // Duplicate byte to both halves
    
    for (i = 0; i < word_count; i++) {
        dest[i] = word_value;
    }
}

// Wait for DMA operations to complete
void wait_dma_finished(void) {
    // Wait until DMA queue index matches last completed DMA index
    while (DMA_QUEUE_INDEX != LAST_COMPLETED_DMA_INDEX) {
        // Busy wait
    }
}

// Set BG2 VRAM location and tilemap settings
void set_bg2_vram_location(unsigned char tilemap_config, unsigned short tilemap_addr, unsigned short tile_addr) {
    // Configure BG2 screen/tilemap settings
    BG2SC_MIRROR = (tilemap_config & 0x03) | ((tilemap_addr >> 8) & 0xFC);
    *(volatile unsigned char*)BG2SC = BG2SC_MIRROR;
    
    // Configure BG2 tile data location  
    BG12NBA_MIRROR = (BG12NBA_MIRROR & 0x0F) | ((tile_addr >> 8) & 0xF0);
    *(volatile unsigned char*)BG12NBA = BG12NBA_MIRROR;
    
    // Reset BG2 scroll positions
    BG2_X_POS = 0;
    BG2_Y_POS = 0;
}

// IRQ callback management
void reset_irq_callback(void) {
    IRQ_CALLBACK = DEFAULT_IRQ_CALLBACK;
}

void set_irq_callback(unsigned short callback_addr) {
    IRQ_CALLBACK = callback_addr;
}

// Graphics color mode setting
void set_colour_addsub_mode(unsigned char cgwsel, unsigned char cgadsub) {
    *(volatile unsigned char*)CGWSEL = cgwsel;
    *(volatile unsigned char*)CGADSUB = cgadsub;
}

// Display control redirect
void set_inidisp_far(unsigned char value) {
    set_inidisp(value);
}

// Random number functions
unsigned short rand_0_3(void) {
    return rand() & 0x0003;
}

unsigned short rand_0_7(void) {
    return rand() & 0x0007;
}

// Math utilities

// 8-bit multiplication using SNES hardware
unsigned short mult8(unsigned char a, unsigned char b) {
    // Use SNES hardware multiplier
    *(volatile unsigned char*)0x4202 = a;  // WRMPYA
    *(volatile unsigned char*)0x4203 = b;  // WRMPYB
    // Wait for multiplication to complete (2 cycles)
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    return *(volatile unsigned short*)0x4216;  // RDMPYL
}

// 16-bit multiplication
unsigned short mult16(unsigned short a, unsigned short b) {
    return a * b;  // Simple C multiplication for now
}

// 16-bit unsigned division
unsigned short division16(unsigned short dividend, unsigned short divisor) {
    if (divisor == 0) return 0;
    return dividend / divisor;
}

// 16-bit signed division
unsigned short division16s(signed short dividend, signed short divisor) {
    if (divisor == 0) return 0;
    return (unsigned short)(dividend / divisor);
}

// 8-bit modulus
unsigned char modulus8(unsigned char dividend, unsigned char divisor) {
    if (divisor == 0) return 0;
    return dividend % divisor;
}

// Random number with upper limit
unsigned short rand_limit(unsigned short limit) {
    if (limit == 0) return 0;
    return rand() % limit;
}

// Truncate 16-bit value to 8-bit with scaling
unsigned short truncate_16_to_8(unsigned short value, unsigned short multiplier) {
    unsigned long result = (unsigned long)value * multiplier;
    return (unsigned short)(result >> 8);  // Shift right 8 bits
}

// Arithmetic shift right for 16-bit signed values
short asr16(short value) {
    if (value >= 0) {
        return value;  // Positive values unchanged
    } else {
        // For negative values, this is the same as regular right shift in C
        return value >> 1;  // C handles sign extension automatically
    }
}

// 16-bit signed modulus operation
short modulus16s(short dividend, short divisor) {
    if (divisor == 0) return 0;
    
    short result = dividend % divisor;
    
    // Handle sign correction for negative dividend
    if (dividend < 0 && result != 0) {
        result = -result;
    }
    
    return result;
}

// Random number generator state  
static unsigned short rand_a = 0x1234;  // Initial seed values
static unsigned short rand_b = 0x5678;

// Custom random number generator implementation
unsigned char rand_custom(void) {
    // Implementation based on the ASM code
    unsigned short temp_a = rand_a;
    unsigned short temp_b = rand_b;
    
    // Multiply and add operations
    unsigned long mult_result = (unsigned long)(temp_a >> 8) * (temp_b & 0xFF);
    temp_b = (temp_b + 0x6D) & 0xFFFF;
    rand_b = temp_b;
    
    // Bit manipulation for rand_a
    mult_result = (mult_result >> 2) & 0x3FFF;
    temp_a = ((mult_result & 0x03) + temp_a) >> 1;
    
    if ((mult_result & 0x01) == 0) {
        temp_a &= 0x7FFF;  // Clear high bit
    } else {
        temp_a |= 0x8000;  // Set high bit
    }
    
    rand_a = temp_a;
    
    return (unsigned char)(mult_result & 0xFF);
}

// 8-bit signed modulus operation
char modulus8s(char dividend, char divisor) {
    if (divisor == 0) return 0;
    
    char result = dividend % divisor;
    
    // Handle sign correction for negative dividend
    if (dividend < 0 && result != 0) {
        result = -result;
    }
    
    return result;
}

// Arithmetic shift right for 8-bit values with shift count
unsigned char asr8(unsigned char value, unsigned char shift_count) {
    if (value < 0x80) {
        // Positive value - logical shift right
        return value >> shift_count;
    } else {
        // Negative value - arithmetic shift right (sign extension)
        unsigned char result = value;
        for (unsigned char i = 0; i < shift_count; i++) {
            result = (result >> 1) | 0x80;  // Shift right and preserve sign bit
        }
        return result;
    }
}