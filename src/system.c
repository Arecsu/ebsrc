#include "include/hardware.h"
#include "include/system.h"
#include <stdint.h>
#include <stddef.h>

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

// 32-bit modulus operation
long modulus32s(long dividend, long divisor) {
    if (divisor == 0) return 0;
    
    // Store sign of dividend for result
    int dividend_negative = (dividend < 0);
    
    // Get absolute values
    long abs_dividend = (dividend < 0) ? -dividend : dividend;
    long abs_divisor = (divisor < 0) ? -divisor : divisor;
    
    // Perform division and get remainder
    long remainder = abs_dividend % abs_divisor;
    
    // Apply sign correction - remainder has same sign as dividend
    if (dividend_negative && remainder != 0) {
        remainder = -remainder;
    }
    
    return remainder;
}

// 32-bit unsigned modulus
unsigned long modulus32(unsigned long dividend, unsigned long divisor) {
    if (divisor == 0) return 0;
    return dividend % divisor;
}

// 32-bit multiplication
long mult32(long a, long b) {
    return a * b;
}

// 16-bit x 8-bit multiplication optimized
unsigned short mult168(unsigned short a, unsigned char b) {
    return a * b;
}

// Forward declaration
short cosine_sine(short value, unsigned char angle);

// Cosine function using lookup table
short cosine(short value, unsigned char angle) {
    // Cosine is sine shifted by 90 degrees (0x40)
    unsigned char cos_angle = (angle - 0x40) & 0xFF;
    return cosine_sine(value, cos_angle);
}

// Cosine/Sine calculation using hardware multiplier and lookup table
short cosine_sine(short value, unsigned char angle) {
    // Use SNES hardware multiplier (Mode 7 matrix)
    // This is a placeholder implementation - actual would use SINE_LOOKUP_TABLE
    // and hardware registers M7A, M7B, MPYM
    
    // For now, use basic trigonometry approximation
    // In actual implementation, would access SINE_LOOKUP_TABLE[angle]
    // and use hardware multiplication via M7A/M7B registers
    
    // Simplified implementation for compilation
    (void)angle; // Suppress unused parameter warning
    return value; // Placeholder return
}

// Clear OAM (Object Attribute Memory) for sprites
void oam_clear(void) {
    // Reset priority sprite offsets
    PRIORITY_0_SPRITE_OFFSET = 0;
    PRIORITY_1_SPRITE_OFFSET = 0; 
    PRIORITY_2_SPRITE_OFFSET = 0;
    PRIORITY_3_SPRITE_OFFSET = 0;
    
    // Set up OAM addresses based on frame buffer
    if (NEXT_FRAME_BUF_ID == 1) {
        OAM_ADDR = (unsigned short)((uintptr_t)&OAM1[0]);
        OAM_END_ADDR = (unsigned short)((uintptr_t)&OAM1[128]);
        OAM_HIGH_TABLE_ADDR = (unsigned short)((uintptr_t)&OAM1_HIGH_TABLE[0]);
        
        // Clear all sprite Y coordinates to 0xE0 (off-screen)
        for (int i = 0; i < 128; i++) {
            OAM1[i].y_coord = 0xE0;
        }
    } else {
        OAM_ADDR = (unsigned short)((uintptr_t)&OAM2[0]);
        OAM_END_ADDR = (unsigned short)((uintptr_t)&OAM2[128]);
        OAM_HIGH_TABLE_ADDR = (unsigned short)((uintptr_t)&OAM2_HIGH_TABLE[0]);
        
        // Clear all sprite Y coordinates to 0xE0 (off-screen)
        for (int i = 0; i < 128; i++) {
            OAM2[i].y_coord = 0xE0;
        }
    }
    
    OAM_HIGH_TABLE_BUFFER = 0x80;
}

// Read joypad input (with demo playback support)
void read_joypad(void) {
    // Check if demo playback is active
    if ((DEMO_RECORDING_FLAGS & DEMO_RECORDING_FLAG_PLAYBACK) != 0) {
        DEMO_FRAMES_LEFT--;
        
        if (DEMO_FRAMES_LEFT == 0) {
            // Advance to next demo input frame
            DEMO_READ_SOURCE += 3;
            
            unsigned char frame_count = *(unsigned char*)DEMO_READ_SOURCE;
            if (frame_count == 0) {
                // End of demo, stop playback
                DEMO_RECORDING_FLAGS &= ~DEMO_RECORDING_FLAG_PLAYBACK;
            } else {
                DEMO_FRAMES_LEFT = frame_count;
                // Read joypad data from demo
                unsigned short demo_input = *(unsigned short*)(DEMO_READ_SOURCE + 1);
                PAD_RAW = demo_input;
                PAD_RAW_2 = demo_input;
            }
        }
    } else {
        // Read actual joypad hardware
        PAD_RAW_2 = JOYPAD_2_DATA;
        PAD_RAW = JOYPAD_1_DATA;
    }
}

// 24-bit memory copy function
void memcpy24(void* dest, const void* src, unsigned char count) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    
    for (unsigned char i = 0; i <= count; i++) {
        d[i] = s[i];
    }
}

// Custom strlen function
unsigned char strlen_custom(const char* str) {
    unsigned char len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}


// IRQ vector jump
void irq_vector(void) {
    // Jump to IRQ handler
    irq();
}

// Execute IRQ callback
void execute_irq_callback(void) {
    // Call function pointer stored in IRQ_CALLBACK
    if (IRQ_CALLBACK != 0) {
        ((void(*)(void))(uintptr_t)IRQ_CALLBACK)();
    }
}

// Calculate save block XOR checksum
unsigned short calc_save_block_xor_checksum(unsigned short slot) {
    // Calculate base address of save block game_state section
    void* save_addr = (void*)(SAVE_BASE + (slot * sizeof(save_block)) + sizeof(save_header));
    
    unsigned short checksum = 0;
    unsigned short* data = (unsigned short*)save_addr;
    
    // XOR all words in the save data (excluding header)
    for (size_t i = 0; i < (sizeof(save_block) - sizeof(save_header)) / 2; i++) {
        checksum ^= data[i];
    }
    
    return checksum;
}

// Erase save block - clear save data and write signature
void erase_save_block(unsigned short slot) {
    // Calculate save block address
    void* save_addr = (void*)(SAVE_BASE + (slot * 0x500));
    
    // Clear entire save block
    memset24(save_addr, 0, 0x500);
    
    // Copy SRAM signature to beginning of save block
    const char* signature = SRAM_SIGNATURE;
    unsigned char sig_len = strlen_custom((const char*)signature);
    memcpy24(save_addr, signature, sig_len);
}

// Forward declaration
void copy_save_block(unsigned short to_block, unsigned short from_block);

// Copy individual save block
void copy_save_block(unsigned short to_block, unsigned short from_block) {
    void* dest = (void*)(SAVE_BASE + (to_block * 0x500));
    void* src = (void*)(SAVE_BASE + (from_block * 0x500));
    
    // Copy entire save block (0x500 bytes)
    for (unsigned short i = 0; i < 0x500; i++) {
        ((unsigned char*)dest)[i] = ((unsigned char*)src)[i];
    }
}

// Copy save slot data 
void copy_save(unsigned short to, unsigned short from) {
    // Save slots are stored in pairs, so multiply by 2 to get block indexes
    unsigned short to_block = to * 2;
    unsigned short from_block = from * 2;
    
    // Copy both save blocks for the slot (main and backup)
    copy_save_block(to_block, from_block);
    copy_save_block(to_block + 1, from_block + 1);
}

// Get average color from palette (96 colors)
void get_colour_average(unsigned short* palette) {
    unsigned short red_total = 0;
    unsigned short green_total = 0;  
    unsigned short blue_total = 0;
    unsigned short color_count = 0;
    
    // Sum RGB components from 96 palette entries
    for (int i = 0; i < 96; i++) {
        unsigned short color = palette[i];
        
        // Skip transparent colors (bit 15 clear)
        if ((color & 0x7FFF) == 0) continue;
        
        // Extract RGB components (BGR555 format)
        red_total += color & 0x1F;                    // Red: bits 0-4
        green_total += (color >> 5) & 0x1F;           // Green: bits 5-9
        blue_total += (color >> 10) & 0x1F;           // Blue: bits 10-14
        
        color_count++;
    }
    
    // Calculate averages (with scaling factor of 8)
    if (color_count > 0) {
        COLOUR_AVERAGE_RED = (red_total * 8) / color_count;
        COLOUR_AVERAGE_GREEN = (green_total * 8) / color_count;  
        COLOUR_AVERAGE_BLUE = (blue_total * 8) / color_count;
    } else {
        COLOUR_AVERAGE_RED = 0;
        COLOUR_AVERAGE_GREEN = 0;
        COLOUR_AVERAGE_BLUE = 0;
    }
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

// Arithmetic shift left for 16-bit values with shift count  
unsigned short asl16(unsigned short value, unsigned char shift_count) {
    return value << shift_count;
}

// 8-bit signed division using SNES hardware
unsigned char division8s(char dividend, char divisor) {
    if (divisor == 0) return 0;
    
    // Convert signed values to unsigned for hardware
    unsigned char abs_dividend = (dividend < 0) ? -dividend : dividend;
    unsigned char abs_divisor = (divisor < 0) ? -divisor : divisor;
    
    // Use SNES hardware divider
    *(volatile unsigned short*)0x4204 = abs_dividend;  // WRDIVL (low byte only)
    *(volatile unsigned short*)0x4206 = 0;             // WRDIVH (clear high byte)
    *(volatile unsigned char*)0x4206 = abs_divisor;    // WRDIVB
    
    // Wait for division to complete (16 cycles)
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    __asm__ volatile ("nop");
    
    // Read quotient and remainder  
    unsigned char quotient = *(volatile unsigned char*)0x4214;  // RDDIVL
    (void)*(volatile unsigned char*)0x4216; // RDMPYL - read to clear register
    
    return quotient;
}

// 8-bit unsigned division wrapper - handles sign logic
unsigned char division8(char dividend, char divisor) {
    if (divisor == 0) return 0;
    
    // Determine if result should be negative (different signs)
    char sign_flag = dividend ^ divisor;
    
    // Call signed division function
    unsigned char result = division8s(dividend, divisor);
    
    // Apply sign correction if needed
    if (sign_flag < 0 && result != 0) {
        result = (unsigned char)(-(char)result);
    }
    
    return result;
}

// Animate palette - handles overworld palette animation timing
void animate_palette(void) {
    // Decrement animation timer
    OVERWORLD_PALETTE_ANIM.timer--;
    
    if (OVERWORLD_PALETTE_ANIM.timer == 0) {
        // Timer expired, advance to next frame
        unsigned char current_index = OVERWORLD_PALETTE_ANIM.index;
        
        // Get delay for current frame (doubled because delays are stored as words)
        unsigned char delay = OVERWORLD_PALETTE_ANIM.delays[current_index * 2];
        
        if (delay == 0) {
            // End of animation sequence, reset to beginning
            OVERWORLD_PALETTE_ANIM.index = 0;
            current_index = 0;
            delay = OVERWORLD_PALETTE_ANIM.delays[0];
        }
        
        // Set timer for next frame
        OVERWORLD_PALETTE_ANIM.timer = delay;
        
        // Process the current palette frame (calls external function)
        unknown_c0a1f2(current_index);
        
        // Advance to next frame
        OVERWORLD_PALETTE_ANIM.index++;
    }
}

// Random number modulo operation
unsigned short rand_mod(unsigned short modulus) {
    if (modulus == 0) return 0;
    
    // Generate random number and take modulus
    unsigned long rand_val = rand_custom();
    return (unsigned short)(rand_val % (modulus + 1));
}

// 32-bit arithmetic left shift
unsigned long asl32(unsigned long value, unsigned char shift_count) {
    return value << shift_count;
}

// 32-bit arithmetic right shift (signed)
long asr32(long value, unsigned char shift_count) {
    return value >> shift_count;  // C handles sign extension automatically
}

// 16-bit modulus operation  
unsigned short modulus16(unsigned short dividend, unsigned short divisor) {
    if (divisor == 0) return 0;
    return dividend % divisor;
}

// Generate long random number
unsigned long rand_long(void) {
    // Generate two 16-bit random values and combine them
    unsigned long high = rand_custom() << 8;
    unsigned long low = rand_custom();
    return (high << 8) | low;
}

// Enable NMI and joypad reading
void enable_nmi_joypad(void) {
    NMITIMEN_MIRROR |= 0x81;  // Enable NMI and joypad auto-read
    *(volatile unsigned char*)0x4200 = NMITIMEN_MIRROR;  // NMITIMEN
}

// Default IRQ callback - does nothing
void default_irq_callback(void) {
    // Empty function
}

// Set BG1 VRAM location
void set_bg1_vram_location(unsigned char bg_size, unsigned short tilemap_base, unsigned short tile_base) {
    BG1SC_MIRROR = (bg_size & 0x03) | ((tilemap_base & 0x3F) << 2);
    *(volatile unsigned char*)0x2107 = BG1SC_MIRROR;
    
    BG12NBA_MIRROR = (BG12NBA_MIRROR & 0xF0) | ((tile_base >> 4) & 0x0F);
    *(volatile unsigned char*)0x210B = BG12NBA_MIRROR;
    
    BG1_X_POS = 0;
    BG1_Y_POS = 0;
}

// Set BG3 VRAM location
void set_bg3_vram_location(unsigned char tilemap_config, unsigned short tilemap_addr, unsigned short tile_addr) {
    BG3SC_MIRROR = (tilemap_config & 0x03) | ((tilemap_addr >> 8) & 0xFC);
    *(volatile unsigned char*)0x210A = BG3SC_MIRROR;  // BG3SC
    
    BG34NBA_MIRROR = (BG34NBA_MIRROR & 0xF0) | ((tile_addr >> 12) & 0x0F);
    *(volatile unsigned char*)0x210D = BG34NBA_MIRROR;  // BG34NBA
    
    BG3_X_POS = 0;
    BG3_Y_POS = 0;
}

// Set BG4 VRAM location  
void set_bg4_vram_location(unsigned char tilemap_config, unsigned short tilemap_addr, unsigned short tile_addr) {
    BG4SC_MIRROR = (tilemap_config & 0x03) | ((tilemap_addr >> 8) & 0xFC);
    *(volatile unsigned char*)0x210B = BG4SC_MIRROR;  // BG4SC
    
    BG34NBA_MIRROR = (BG34NBA_MIRROR & 0x0F) | ((tile_addr >> 8) & 0xF0);
    *(volatile unsigned char*)0x210D = BG34NBA_MIRROR;  // BG34NBA
    
    BG4_X_POS = 0;
    BG4_Y_POS = 0;
}

// Allocate sprite memory slots
void alloc_sprite_mem(unsigned char slot_id, unsigned char sprite_size) {
    unsigned char target_value = (slot_id & 0xFF) | 0x80;
    
    for (unsigned short i = 0; i < 88; i++) {
        unsigned char current = SPRITE_VRAM_TABLE[i] & 0xFF;
        if (current == target_value || slot_id == 0x80) {
            SPRITE_VRAM_TABLE[i] = sprite_size;
        }
    }
}

// Test SRAM size by writing test values
unsigned char test_sram_size(void) {
    *(volatile unsigned char*)SRAM_SIZE_1_SCRATCH = 0x30;
    *(volatile unsigned char*)SRAM_SIZE_2_SCRATCH = 0x31;
    
    if (*(volatile unsigned char*)SRAM_SIZE_2_SCRATCH == *(volatile unsigned char*)SRAM_SIZE_1_SCRATCH) {
        return LAST_SRAM_BANK;
    }
    
    *(volatile unsigned char*)SRAM_SIZE_3_SCRATCH = 0x32;
    if (*(volatile unsigned char*)SRAM_SIZE_3_SCRATCH == *(volatile unsigned char*)SRAM_SIZE_1_SCRATCH) {
        return LAST_SRAM_BANK;
    }
    
    LAST_SRAM_BANK = 0x32;
    return LAST_SRAM_BANK;
}

// Start fade in effect
void fade_in(unsigned char step, unsigned char delay) {
    FADE_PARAMETERS.step = step;
    FADE_PARAMETERS.delay = delay;
    FADE_DELAY_FRAMES_LEFT = delay;
}

// Start fade out effect
void fade_out(unsigned char step, unsigned char delay) {
    FADE_PARAMETERS.step = (~step) + 1;  // Negate step for fade out
    FADE_PARAMETERS.delay = delay;
    FADE_DELAY_FRAMES_LEFT = delay;
}