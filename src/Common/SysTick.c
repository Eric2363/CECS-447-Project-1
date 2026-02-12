/*

Name: Eric Santana

Config
=================================================================
Interupt Type: Periodic
Priority Level: 0
Description: This file is in charge of configuring SysTick for
periodic interupts of a given frequency
=================================================================
*/

#include "SysTick.h"
#include "stdint.h"

void SysTick_Init(){
	
	//Start SysTick off  for config
	NVIC_ST_CTRL_R = DISABLE;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x00000000; // priority 0
	
}

void Load_SysTick(uint32_t Reload_Value){

	NVIC_ST_RELOAD_R = Reload_Value -1;

}

void Start_SysTick(){
	
	//Start Timer
	NVIC_ST_CTRL_R = ENABLE;

}

void Stop_SysTick(){

	NVIC_ST_CTRL_R = DISABLE;
	
}
