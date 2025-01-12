#include "kernel.h"

void start_kernel() {
    isr_install();
    asm volatile("sti");
    init_keyboard();
    init_dynamic_mem();
    init_display();

    fill_screen(0xffa3a3a3);
    draw_rectangle(100, 100, 100, 100, 0xffffffff);
    draw_rectangle(100, 250, 100, 100, 0x00000000);
    draw_text(210, 100, LEFT, "Hello, World", 0xff000000, 2.0, put_pixel);
}
