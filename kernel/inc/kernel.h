#ifndef KERNEL_H
#define KERNEL_H

#include "idt.h"
#include "isr.h"
#include "timer.h"
#include "display.h"
#include "keyboard.h"
#include "util.h"
#include "mem.h"
#include "text.h"

void start_kernel();

#endif // KERNEL_H
