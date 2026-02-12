#include "../tm4c123gh6pm.h"

/*
Port E  bits: E3 - E2 - E1 - E0
Notes:        F    E    D    C  
*/
#define PORTE 0x10

//Port C Bits 3-0
#define KEYS_MASK 0x0F
#define PE0 0x01
#define PE1 0x02
#define PE2 0x04
#define PE3 0x08

//Interupt Priority Level
#define LEVEL1 0x00000020
void PianoKeys_Init(void);


