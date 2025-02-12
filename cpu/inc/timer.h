#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void init_timer(uint32_t freq);
void sleep(uint32_t ms);

#endif // TIMER_H
