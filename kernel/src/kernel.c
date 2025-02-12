#include "kernel.h"
#include "display.h"
#include "timer.h"

void start_kernel() {
    isr_install();
    asm volatile("sti");
    init_timer(100000);
    init_keyboard();
    init_dynamic_mem();
    init_display();

    int x_0 = 0;
    int x_dir = 5;
    int y_1 = 0;
    int y_dir = 5;
    while(1) {
        if (x_0 + 100 >= get_screen_width()) {
            x_dir = -5;
        } else if (x_0 <= 0) {
            x_dir = 5;
        }

        if (y_1 + 100 >= get_screen_height()) {
            y_dir = -5;
        } else if (y_1 <= 0) {
            y_dir = 5;
        }

        x_0 += x_dir;
        y_1 += y_dir;
        
        fill_screen(0xffa3a3a3);
        draw_rectangle(x_0, 100, 100, 100, 0xffffffff);
        draw_rectangle(100, y_1, 100, 100, 0x00000000);
        draw_text(210, 100, LEFT, "BridOS", 0xff000000, 1.5, put_pixel);
        sleep(100);
    }
}
