#include "kernel.h"
#include <stdint.h>

#define MODE_INFO_BLOCK_ADDRESS ((ModeInfoBlock*)0x8000)

typedef struct {
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
} ModeInfoBlock;

void put_pixel(ModeInfoBlock* mode_info, int x, int y, uint32_t color) {
    uint8_t* framebuffer = (uint8_t*)(uintptr_t)mode_info->framebuffer;

    uint32_t offset = y * mode_info->pitch + x * 3;

    framebuffer[offset + 0] = (color & 0xFF);
    framebuffer[offset + 1] = (color >> 8) & 0xFF;
    framebuffer[offset + 2] = (color >> 16) & 0xFF;
}

void draw_rectangle(ModeInfoBlock* mode_info, int x, int y, int w, int h, uint32_t color) {
    for (int dy = y; dy < y + h; dy++) {
        for (int dx = x; dx < x + w; dx++) {
            put_pixel(mode_info, dx, dy, color);
        }
    }
}

void fill_screen(ModeInfoBlock* mode_info, uint32_t color) {
    for (int y = 0; y < mode_info->height; y++) {
        for (int x = 0; x < mode_info->width; x++) {
            put_pixel(mode_info, x, y, color);
        }
    }
}

void start_kernel() {
    isr_install();
    asm volatile("sti");
    init_keyboard();
    init_dynamic_mem();

    ModeInfoBlock* mode_info = MODE_INFO_BLOCK_ADDRESS;

    fill_screen(mode_info, 0xff00ff);
    draw_rectangle(mode_info, 100, 100, 100, 100, 0xffffff);
}
