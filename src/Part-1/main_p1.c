// main_p1.c
// Course number: CECS 447
// Term: Spring 2026
// Project number: 1
// Project description:
// Team #: 4
// Team members: Eric Santana; Dante Jimenez; Alexander Zepeda; Kour, Thaisinge
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
void Beep(void);
void Song_Test(void);
void Delay(uint32_t ms);
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

//Tone tab
const uint32_t Tone_Tab[] =
{LOW_C,LOW_D,LOW_E,LOW_F,LOW_G,LOW_A,LOW_B,
 MID_C,MID_D,MID_E,MID_F,MID_G,MID_A,MID_B,
 HIGH_C,HIGH_D,HIGH_E,HIGH_F,HIGH_G,HIGH_A,HIGH_B};


#define PAUSE 255
 
typedef struct Note {
  uint8_t tone_index;
  uint32_t delay;
}Note;

typedef const struct Note NTyp;

// indices into Tone_Tab
#define LC 0
#define LD 1
#define LE 2
#define LF 3
#define LG 4
#define LA 5
#define LB 6
#define MC 7
#define MD 8
#define ME 9
#define MF 10
#define MG 11
#define MA 12
#define MB 13
#define HC 14
#define HD 15
#define HE 16
#define HF 17
#define HG 18
#define HA 19
#define HB 20

NTyp Song_HappyBirthday_ms[] = {
  {LC,50},{LC,50},{LD,100},{LC,100},{LF,100},{LE,200},
  {LC,50},{LC,50},{LD,100},{LC,100},{LG,100},{LF,200},
  {LC,50},{LC,50},{MC,100},{LA,100},{LF,100},{LE,100},{LD,200},
  {LB,50},{LB,50},{LA,100},{LF,100},{LG,100},{LF,300},
  {0,0}
};



int main(void){	
  System_Init();


  while(1){
	
		switch(currentMode){
			case Mode1:PianoMode();break;
			case Mode2:AutoPlayMode();break;
		}
		

			
  }
}

//Mode 1
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

void AutoPlayMode(void){
  static uint32_t i = 0;   // note index persists across calls

  // If you ever want it to restart when you enter Mode2:
  // (optional) you can detect mode changes with another static var.
  // For now: it just keeps looping forever.

  // End of song -> restart
  if(Song_HappyBirthday_ms[i].delay == 0){
    i = 0;
    Sound_Stop();
    Delay(300);
    return;
  }

  // Play current note
  if(Song_HappyBirthday_ms[i].tone_index == PAUSE){
    Sound_Stop();
  } else {
    Sound_Start(Tone_Tab[Song_HappyBirthday_ms[i].tone_index]);
  }

  // Hold note for its duration (ms)
  Delay(Song_HappyBirthday_ms[i].delay);

  // Small gap between notes
  Sound_Stop();
  Delay(20);

  // Next note
  i++;
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

// Busy-wait delay for 50 MHz system clock
void Delay(uint32_t ms){
    volatile unsigned long time;
    
    while(ms){
        time = 16667;   // ˜ 1ms at 50 MHz
        while(time){
            time--;
        }
        ms--;
    }
}

void Beep(){


	Sound_Start(LOW_A);
	Delay(300);
	Sound_Stop();
	Delay(300);
	Sound_Start(LOW_B);
	Delay(300);
		Sound_Stop();
	Sound_Start(LOW_G);
	Delay(300);
		Sound_Stop();
	Sound_Stop();
	Delay(300);

}





