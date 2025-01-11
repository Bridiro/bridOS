#ifndef DISPLAY_H
#define DISPLAY_H

#include "ports.h"
#include <stdint.h>
#include "mem.h"
#include "util.h"

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

#define MODE_INFO_BLOCK_ADDRESS ((struct ModeInfoBlock*)0x8000)

struct ModeInfoBlock{
    uint16_t attributes;
    uint8_t window_a, window_b;
    uint16_t granularity;
    uint16_t window_size;
    uint16_t segment_a, segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;
    uint16_t width, height;
    uint8_t w_char, y_char, planes, bpp, banks;
    uint8_t memory_model, bank_size, image_pages;
    uint8_t reserved0;
    uint8_t red_mask, red_position;
    uint8_t green_mask, green_position;
    uint8_t blue_mask, blue_position;
    uint8_t reserved_mask, reserved_position;
    uint8_t direct_color_attributes;
    uint32_t framebuffer;
    uint32_t offscreen_mem_off;
    uint16_t offscreen_mem_size;
    uint8_t reserved[206];
};


/* Public kernel API */
void print_string(char *string);
void print_nl();
void clear_screen();
int scroll_ln(int offset);
void print_backspace();

/* NEW FUNCTIONS FOR GRAPHIC MODE */
void init_display();
void put_pixel(int x, int y, uint32_t color);
void draw_rectangle(int x, int y, int w, int h, uint32_t color);
void fill_screen(uint32_t color);


#endif // DISPLAY_H
