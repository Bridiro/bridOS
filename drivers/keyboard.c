#include "keyboard.h"

static char key_buffer[256];

static int is_shifted = 0;

static char *curmap = sc_ascii;

static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = port_byte_in(0x60);

    if (scancode > SC_MAX) return;

    if(is_shifted==1)
        curmap = sc_ascii_shifted;
    else if(is_shifted==0)
        curmap = sc_ascii;

    switch(scancode){
        case KEY_BACKSPACE:
            if (backspace(key_buffer)) {
                print_backspace();
            }
            break;

        case KEY_ENTER:
            print_nl();
            execute_command(key_buffer);
            key_buffer[0] = '\0';
            break;

        case KEY_LSHIFT:
        case KEY_RSHIFT:
            is_shifted = 1;
            break;


        /*
            ----------------------------------- NOT WORKING -------------------------------------------
        */
        case KEY_LSHIFT_RELEASE:
        case KEY_RSHIFT_RELEASE:
            is_shifted = 0;
            break;
        /*
            ----------------------------------- NOT WORKING -------------------------------------------
        */


        case KEY_ESCAPE:
            is_shifted = 0;
            break;

        case KEY_TAB:
            for(int i=0; i<4; i++) {
                append(key_buffer, ' ');
            }
            print_string("    ");
            break;

        default:
            char *number;
            char letter = curmap[(int) scancode];
            append(key_buffer, letter);
            char str[2] = {letter, '\0'};
            print_string(str);
            break;
    }
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}

