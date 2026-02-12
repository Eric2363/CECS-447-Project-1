// main_p1.c
// Course number: CECS 447
// Term: Spring 2026
// Project number: 1
// Project description:
// Team #: 4
// Team members: Eric Santana
//=================================================================
#include "../tm4c123gh6pm.h"
#include "../Common/gpio_btn.h"
#include "../Common/PortF.h"
#include "../Common/PianoKeys.h"
#include "../Common/Dac.h"
#include "../Common/PLL.h"


// function prototypes
void System_Init(void);
void PianoMode(void);
void AutoPlayMode(void);

//globals
volatile uint8_t keyPressed = 0;

typedef enum{

	Low_Octave,
	Middle_Octave,
	High_Octave
	
}State;

typedef enum{
	Mode1,
	Mode2,
}Mode;

//Start in Low octave
State currentState = Low_Octave;

//Starting mode piano mode
Mode currentMode = Mode1;


int main(void){	
  System_Init();

	
  while(1){
	
		switch(currentMode){
			case Mode1:PianoMode();break;
			case Mode2:AutoPlayMode();break;
		}
		

			
  }
}

//Mode1
void PianoMode(void){
  static uint8_t last = 0;   // static so it persists across calls
  uint8_t k = keyPressed;

  // Low octave
  if(currentState == Low_Octave){
    if(k != last){
      switch(k){
        case 1: Sound_Start(LOW_C); break;
        case 2: Sound_Start(LOW_D); break;
        case 3: Sound_Start(LOW_E); break;
        case 4: Sound_Start(LOW_F); break;
        default: Sound_Stop(); break;
      }
      last = k;
    }
  }

  // Middle octave
  if(currentState == Middle_Octave){
    if(k != last){
      switch(k){
        case 1: Sound_Start(MID_C); break;
        case 2: Sound_Start(MID_D); break;
        case 3: Sound_Start(MID_E); break;
        case 4: Sound_Start(MID_F); break;
        default: Sound_Stop(); break;
      }
      last = k;
    }
  }

  // High octave
  if(currentState == High_Octave){
    if(k != last){
      switch(k){
        case 1: Sound_Start(HIGH_C); break;
        case 2: Sound_Start(HIGH_D); break;
        case 3: Sound_Start(HIGH_E); break;
        case 4: Sound_Start(HIGH_F); break;
        default: Sound_Stop(); break;
      }
      last = k;
    }
  }
}

void AutoPlayMode(){
	
}

void System_Init(void) {
	gpio_btn_init();
	PLL_Init();
	PortF_Init();
  PianoKeys_Init();
	DAC_Init();
	Sound_Init();
	
}

//Description: Responsible for handling piano key input
void GPIOPortE_Handler(){
 
	//Check which bit caused interupt to fire
	uint8_t status = GPIO_PORTE_MIS_R & KEYS_MASK;
	//Clear interupt flag
	GPIO_PORTE_ICR_R = status;
	//Clear current LEDS
	GPIO_PORTF_DATA_R &=~LEDS;
	
	//See current state of PORTE data
	uint8_t data = GPIO_PORTE_DATA_R;
	
	if(data & PE0){
	
		GPIO_PORTF_DATA_R |= RED;
		keyPressed = 1;
	}
	else if(data & PE1){
		
		GPIO_PORTF_DATA_R |= BLUE;
		keyPressed =2;
	}
	else if(data & PE2){
		GPIO_PORTF_DATA_R |= LEDS;
		keyPressed = 3;
	}
	else if (data & PE3){
		GPIO_PORTF_DATA_R |= GREEN;
		keyPressed = 4;
	}
	else{
		keyPressed = 0;
	}
	
}

//Description: Responsible for switches to change modes
void GPIOPortF_Handler(){
	
	//SW1 piano toggle/ autoplay
	if(GPIO_PORTF_MIS_R & SW1){
	
		GPIO_PORTF_ICR_R = SW1; // Clear interupt flag for SW1
		
		GPIO_PORTF_DATA_R &=~LEDS; // Clear previous LEDS
		
		GPIO_PORTF_DATA_R |= BLUE; // set new LED
		
		if(currentMode == Mode1){
			currentMode = Mode2;
		}
		else{
			currentMode = Mode1;
		}
	
	}
	//SW2 Octave change / song change
	if(GPIO_PORTF_MIS_R & SW2){
		
		GPIO_PORTF_ICR_R = SW2; // Clear interupt flag for SW2
		
		GPIO_PORTF_DATA_R &=~LEDS; // Clear previous LEDS
		GPIO_PORTF_DATA_R |= RED; // set new LED
		

		
		if(currentState == Low_Octave){
			currentState = Middle_Octave;
		}
		else if(currentState == Middle_Octave){
			currentState = High_Octave;
		}
		else{
			currentState = Low_Octave;
		}
		
	}

}


