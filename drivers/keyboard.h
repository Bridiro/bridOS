#pragma once

#include <stdbool.h>
#include "ports.h"
#include "../cpu/isr.h"
#include "display.h"
#include "../kernel/util.h"
#include "../kernel/kernel.h"

#define KEY_ESCAPE 0x1
#define KEY_SPACE 0x39
#define KEY_ENTER 0x1C
#define KEY_BACKSPACE 0x0E
#define KEY_TAB 0xF
#define KEY_UPARROW 0x48
#define KEY_DOWNARROW 0x50
#define KEY_LEFTARROW 0x4B
#define KEY_RIGHTARROW 0x4D
#define KEY_PGUP 0x49
#define KEY_PGDOWN 0x51
#define EXTENDED 0xE0
#define EXTENDED_2 0x2A

#define KEY_LSHIFT 0x2A
#define KEY_RSHIFT 0x36
#define KEY_CTRL 0x1D
#define KEY_ALT 0x38
#define KEY_ALTGR 0x39

#define KEY_LSHIFT_RELEASE 0xAA
#define KEY_RSHIFT_RELEASE 0xB6

#define SC_MAX 57

const static char *sc_name[] = {"ERROR", "Esc", "1", "2", "3", "4", "5", "6",
                                "7", "8", "9", "0", "-", "=", "Backspace", "Tab", "Q", "W", "E",
                                "R", "T", "Y", "U", "I", "O", "P", "[", "]", "Enter", "Lctrl",
                                "A", "S", "D", "F", "G", "H", "J", "K", "L", ";", "'", "`",
                                "LShift", "\\", "Z", "X", "C", "V", "B", "N", "M", ",", ".",
                                "/", "RShift", "Keypad *", "LAlt", "Spacebar"};

const static char sc_ascii[] = {'?', '?', '1', '2', '3', '4', '5', '6',
                                '7', '8', '9', '0', '-', '=', '?', '?', 'q', 'w', 'e', 'r', 't', 'y',
                                'u', 'i', 'o', 'p', '[', ']', '?', '?', 'a', 's', 'd', 'f', 'g',
                                'h', 'j', 'k', 'l', ';', '\'', '`', '?', '\\', 'z', 'x', 'c', 'v',
                                'b', 'n', 'm', ',', '.', '/', '?', '?', '?', ' '};

const static char sc_ascii_shifted[] = {'?', '?', '!', '"', '?', '$', '%', '&',
                                        '/', '(', ')', '=', '?', '^', '?', '?', 'Q', 'W', 'E', 'R', 'T', 'Y',
                                        'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G',
                                        'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V',
                                        'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};

void init_keyboard();
