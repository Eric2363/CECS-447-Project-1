
// 6-bit DAC (PB5-PB0)
#include <stdint.h>
#include "../tm4c123gh6pm.h"


// Port B masked data register (PB5-PB0)
#define DAC_PORT (*((volatile uint32_t *)0x400050FC))
	
#define TABLE_LEN 64
#define PORTB_CLK 0x02
#define DAC_MASK 0x3F

#define SYSTICK_DISABLE 0x00
#define SYSTICK_ENABLE 0x07

//Formula for Systick reload values: Rv = Clk/ (table_length * Note_frequency)

#define LOW_C 2985
#define LOW_D 2658
#define LOW_E 2367
#define LOW_F 2238
#define LOW_G 1992
#define LOW_A 1775
#define LOW_B 1581

#define MID_C 1493
#define MID_D 1331
#define MID_E 1185
#define MID_F 1119
#define MID_G 996
#define MID_A 887
#define MID_B 791

#define HIGH_C 746
#define HIGH_D 665
#define HIGH_E 593
#define HIGH_F 559
#define HIGH_G 498
#define HIGH_A 444
#define HIGH_B 395


void DAC_Init(void);
void Sound_Init(void);
void Sound_Stop(void);
void Sound_Start(unsigned long period);
