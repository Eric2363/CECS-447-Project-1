// main_p2.c
// Course number:
// Term: 
// Project number: 
// Project description:
// Team #:
// Team members: 

#include "../tm4c123gh6pm.h"
#include "../Common/gpio_btn.h"

// function prototypes
void System_Init(void);

int main(void){	
  System_Init();
	
  while(1){
  }
}

void System_Init(void) {
	gpio_btn_init();
}
