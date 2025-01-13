#include "keyboard.h"
#include "ports.h"
#include "isr.h"
#include "display.h"
#include "util.h"
#include "commands.h"
#include <stdint.h>

static char key_buffer[1024];

static bool is_shifted = false;
static bool is_alt = false;
static bool is_ctrl = false;
static bool is_altgr = false;

static char *curmap = (char*)(uintptr_t)sc_ascii;

static void keyboard_callback(registers_t *regs) {
    uint8_t scancode = port_byte_in(0x60);

    if(is_shifted==1)
        curmap = sc_ascii_shifted;
    else if(is_shifted==0)
        curmap = sc_ascii;

    switch(scancode) {
    case KEY_LSHIFT:
    case KEY_RSHIFT:
        is_shifted = 1;
        return;
    case KEY_LSHIFT_RELEASE:
    case KEY_RSHIFT_RELEASE:
        is_shifted = 0;
        return;
    case KEY_ALT:
        is_alt = 1;
        return;
    case KEY_ALT_RELEASE:
        is_alt = 0;
        return;
    case KEY_CTRL:
        is_ctrl = 1;
        return;
    case KEY_CTRL_RELEASE:
        is_ctrl = 0;
        return;
    case KEY_ALTGR:
        is_altgr = 1;
        return;
    case KEY_ALTGR_RELEASE:
        is_altgr = 0;
        return;
    }

    // for < and >
    if (scancode == 86) {
        if (is_shifted) {
            append(key_buffer, '>');
            char str[2] = {'>', '\0'};
            print_string(str);
        } else {
            append(key_buffer, '<');
            char str[2] = {'<', '\0'};
            print_string(str);
        }
    }

    if (scancode > SC_MAX) return;

    switch(scancode){
        case KEY_BACKSPACE:
            if (backspace(key_buffer)) {
                print_backspace();
            }
            break;

        case KEY_ENTER:
            print_nl();
            // execute_command(key_buffer);
            key_buffer[0] = '\0';
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

