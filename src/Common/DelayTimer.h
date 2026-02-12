#ifndef DELAYTIMER_H
#define DELAYTIMER_H

#include <stdint.h>

void DelayTimer_Init(void);
void DelayMs(uint32_t ms);
void Delay(void);  // 0.1 sec like professor

#endif
