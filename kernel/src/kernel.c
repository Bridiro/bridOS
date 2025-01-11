#include "kernel.h"

void start_kernel() {
    isr_install();
    asm volatile("sti");
    init_keyboard();
    init_dynamic_mem();
    init_display();

    fill_screen(0xffff00ff);
    draw_rectangle(100, 100, 100, 100, 0xffffffff);
    draw_rectangle(250, 100, 100, 100, 0x00000000);
    draw_text(210, 100, LEFT, "Hello, World", 0xff000000, 1.0, put_pixel);
}
