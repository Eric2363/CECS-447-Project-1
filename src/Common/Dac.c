/*
Group: 4
Config
=================================================================
Port: B
Bits: B5-B0
DIR: Output
Systick Interupt Priority Level: 0
Description: This file is in charge of configuring portB as output into the DAC.
Systick interupt updates input to DAC 
=================================================================
*/
#include "Dac.h"
unsigned char Index;

// 6-bit sine table: 64 samples, values 0..63
static const uint8_t SineWave[TABLE_LEN] = {
  32,35,38,41,44,46,49,51,54,56,58,59,61,62,62,63,
  63,63,62,62,61,59,58,56,54,51,49,46,44,41,38,35,
  32,28,25,22,19,17,14,12, 9, 7, 5, 4, 2, 1, 1, 0,
   0, 0, 1, 1, 2, 4, 5, 7, 9,12,14,17,19,22,25,28
};

void DAC_Init(void) {
	//Enable PortB clk
  SYSCTL_RCGCGPIO_R |= PORTB_CLK;
  while((SYSCTL_RCGCGPIO_R & PORTB_CLK) == 0);// wait for clk ready

  GPIO_PORTB_AMSEL_R &= ~DAC_MASK;      // disable analog
  GPIO_PORTB_AFSEL_R &= ~DAC_MASK;      // disable alternate
  GPIO_PORTB_DIR_R |= DAC_MASK;         // outputs
  GPIO_PORTB_DEN_R |= DAC_MASK;         // digital enable

  GPIO_PORTB_DR8R_R |= DAC_MASK;        // Enable 8mA drive
}

void Sound_Init(){

	Index = 0; //Init table
	NVIC_ST_CTRL_R = SYSTICK_DISABLE; // Disable Systick
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | (0u << 29);//Priority lvl 0
}

void Sound_Stop(){
	
	NVIC_ST_CTRL_R = SYSTICK_DISABLE; // Disable SYSTICK
}

void Sound_Start(unsigned long period){
	
	NVIC_ST_RELOAD_R = period -1; // Reload value
	NVIC_ST_CURRENT_R = 0; // Clear current value
	NVIC_ST_CTRL_R |= SYSTICK_ENABLE; // start timer with system clk, interupts
}

void SysTick_Handler(void){
	Index = (Index + 1) & 0x3F;
	DAC_PORT = SineWave[Index];
}

