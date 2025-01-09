#include "kernel.h"

#define MODE_INFO_BLOCK_ADDRESS ((ModeInfoBlock*)0x8000)

typedef struct {
    uint16_t attributes;
    uint8_t window_a, window_b;
    uint16_t granularity;
    uint16_t window_size;
    uint16_t segment_a, segment_b;
    uint32_t win_func_ptr;
    uint16_t pitch;          // Bytes per scanline
    uint16_t width, height;  // Risoluzione
    uint8_t w_char, y_char, planes, bpp, banks;
    uint8_t memory_model, bank_size, image_pages;
    uint8_t reserved0;
    uint8_t red_mask, red_position;
    uint8_t green_mask, green_position;
    uint8_t blue_mask, blue_position;
    uint8_t reserved_mask, reserved_position;
    uint8_t direct_color_attributes;
    uint32_t framebuffer;    // Indirizzo LFB
    uint32_t offscreen_mem_off;
    uint16_t offscreen_mem_size;
    uint8_t reserved[206];
} ModeInfoBlock;

void put_pixel(ModeInfoBlock* mode_info, int x, int y, uint8_t color) {
    uint8_t* framebuffer = (uint8_t*)mode_info->framebuffer;
    framebuffer[y * mode_info->pitch + x] = color;
}

void draw_rectangle(ModeInfoBlock* mode_info, int x, int y, int w, int h, uint8_t color) {
    for (int dy = y; dy < y + h; dy++) {
        for (int dx = x; dx < w + w; dx++) {
            put_pixel(mode_info, dx, dy, color);
        }
    }
}

void fill_screen(ModeInfoBlock* mode_info, uint8_t color) {
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

    fill_screen(mode_info, 0x4);
    draw_rectangle(mode_info, 100, 100, 100, 100, 0xf);
}
