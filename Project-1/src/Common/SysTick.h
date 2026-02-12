#include "../tm4c123gh6pm.h"

void SysTick_Init(void);

//Enable SysTick with interupt
#define ENABLE 0x07
#define DISABLE 0

#define MIDDLE_C 1500000

#define TABLE_LENGTH 64
