#include "kernel.h"

#define VIDEO_MEM 0xA0000
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void put_pixel(int x, int y, unsigned char color) {
    unsigned char* video_buffer = (unsigned char*)VIDEO_MEM;
    video_buffer[y * SCREEN_WIDTH + x] = color;
}

void fill_screen(unsigned char color) {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            put_pixel(x, y, color);
        }
    }
}

void start_kernel() {
    isr_install();
    asm volatile("sti");
    init_keyboard();
    init_dynamic_mem();

    fill_screen(15);
}
