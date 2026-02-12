/*

Name: Eric Santana

Config
=================================================================
Port: E
Bits: 3-0
DIR: Input
Interupt Type: Level Sensitive
Priority Level: 1
Switch logic: Pull Down
Description: This file is in charge of configuring portC for 
the piano keys input.
=================================================================
*/

#include "PianoKeys.h"

void PianoKeys_Init(){
	
	//turn on portc clk
	SYSCTL_RCGCGPIO_R |= PORTE;
	
	//wait for clk to stablize
	while((SYSCTL_RCGCGPIO_R & PORTE) != PORTE);
	

	
	
	//Congigure bits 3-0 as input
	GPIO_PORTE_DIR_R &=~ KEYS_MASK;
	GPIO_PORTE_DEN_R |= KEYS_MASK;
	GPIO_PORTE_AMSEL_R &=~ KEYS_MASK;
	GPIO_PORTE_AFSEL_R &=~ KEYS_MASK;
	
	//Configure Level sensitve interupts
	
	GPIO_PORTE_IS_R &=~ KEYS_MASK; // Edge interupt
	GPIO_PORTE_IBE_R |= KEYS_MASK; //both edge trigger
	GPIO_PORTE_ICR_R = KEYS_MASK; // Clear any previouse interupt flags
	GPIO_PORTE_IM_R |= KEYS_MASK; //Arm interupt
	
	//Interupt Priority
	NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFFFF0F) | LEVEL1; // Set priority level 0
	NVIC_EN0_R |= 0x00000010; // Enable portE interupt: IRQ4
	
}