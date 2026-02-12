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
#include "../Common/DelayTimer.h"



// function prototypes
void System_Init(void);
void PianoMode(void);
void AutoPlayMode(void);
void Beep(void);
void Song_Test(void);

//void Delay(uint32_t ms);
//globals
volatile uint8_t keyPressed = 0;
volatile uint8_t currentSong = 0; 
volatile uint16_t songIndex = 0;

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

//Songs
//========================================================
//Tone tab
const uint32_t Tone_Tab[] =
{LOW_C,LOW_D,LOW_E,LOW_F,LOW_G,LOW_A,LOW_B,
 MID_C,MID_D,MID_E,MID_F,MID_G,MID_A,MID_B,
 HIGH_C,HIGH_D,HIGH_E,HIGH_F,HIGH_G,HIGH_A,HIGH_B};

 //Notes
typedef struct Note {
  uint8_t tone_index;
  uint32_t delay;
}Note;


typedef const struct Note NTyp;
void play_a_song(const NTyp scoretab[]);


//TEMPO
#define UNIT_MS 80 // 1 duration is 100ms

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

#define PAUSE_IDX 255


#define SONG_COUNT 4
#define SONG_MAX_NOTES 255



NTyp mysong[SONG_COUNT][SONG_MAX_NOTES] = {
  // Song 0: Mary Had a Little Lamb
  {
    // | E D C D | E E E - |
    {LE,4}, {LD,4}, {LC,4}, {LD,4}, {LE,4}, {PAUSE_IDX,1}, {LE,4}, {PAUSE_IDX,1}, {LE,8},
    // | D D D - | E G G - |
    {LD,4}, {PAUSE_IDX,1}, {LD,4}, {PAUSE_IDX,1}, {LD,8}, {LE,4}, {LG,4}, {PAUSE_IDX,1}, {LG,8},
    // | E D C D | E E E - |
    {LE,4}, {LD,4}, {LC,4}, {LD,4}, {LE,4}, {PAUSE_IDX,1}, {LE,4}, {PAUSE_IDX,1}, {LE,8},
    // | D D E D C - |
    {LD,4}, {PAUSE_IDX,1}, {LD,4}, {LE,4}, {LD,4}, {LC,8},
    {0,0}
  },

  // Song 1: Twinkle Twinkle Little Star
  {
    // | C C G G | A A G - | F F E E | D D C - |
    {LC,3}, {PAUSE_IDX,1}, {LC,3}, {PAUSE_IDX,1}, {LG,3}, {PAUSE_IDX,1}, {LG,3}, {PAUSE_IDX,1}, 
    {LA,3}, {PAUSE_IDX,1}, {LA,3}, {PAUSE_IDX,1}, {LG,8},
    {LF,3}, {PAUSE_IDX,1}, {LF,3}, {PAUSE_IDX,1}, {LE,3}, {PAUSE_IDX,1}, {LE,3}, {PAUSE_IDX,1}, 
    {LD,3}, {PAUSE_IDX,1}, {LD,3}, {PAUSE_IDX,1}, {LC,8},
    // | G G F F | E E D - | G G F F | E E D - |
    {LG,3}, {PAUSE_IDX,1}, {LG,3}, {PAUSE_IDX,1}, {LF,3}, {PAUSE_IDX,1}, {LF,3}, {PAUSE_IDX,1}, 
    {LE,3}, {PAUSE_IDX,1}, {LE,3}, {PAUSE_IDX,1}, {LD,8},
    {LG,3}, {PAUSE_IDX,1}, {LG,3}, {PAUSE_IDX,1}, {LF,3}, {PAUSE_IDX,1}, {LF,3}, {PAUSE_IDX,1}, 
    {LE,3}, {PAUSE_IDX,1}, {LE,3}, {PAUSE_IDX,1}, {LD,8},
    // | C C G G | A A G - | F F E E | D D C - |
    {LC,3}, {PAUSE_IDX,1}, {LC,3}, {PAUSE_IDX,1}, {LG,3}, {PAUSE_IDX,1}, {LG,3}, {PAUSE_IDX,1}, 
    {LA,3}, {PAUSE_IDX,1}, {LA,3}, {PAUSE_IDX,1}, {LG,8},
    {LF,3}, {PAUSE_IDX,1}, {LF,3}, {PAUSE_IDX,1}, {LE,3}, {PAUSE_IDX,1}, {LE,3}, {PAUSE_IDX,1}, 
    {LD,3}, {PAUSE_IDX,1}, {LD,3}, {PAUSE_IDX,1}, {LC,8},
    {0,0}
  },

  // Song 2: Happy Birthday
  {
    // | C C | D C F | E C C | D C G | F C C |
    {MC,2}, {PAUSE_IDX,1}, {MC,1}, {PAUSE_IDX,1}, {MD,4}, {MC,4}, {MF,4}, {ME,8}, 
    {MC,2}, {PAUSE_IDX,1}, {MC,1}, {PAUSE_IDX,1}, {MD,4}, {MC,4}, {MG,4}, {MF,8},
    {MC,2}, {PAUSE_IDX,1}, {MC,1}, {PAUSE_IDX,1},
    // | C' A F | E D B B | A F G | F |
    {HC,4}, {MA,4}, {MF,4}, {ME,4}, {MD,4}, 
    {MB,2}, {PAUSE_IDX,1}, {MB,1}, {PAUSE_IDX,1}, {MA,4}, {MF,4}, {MG,4}, {MF,12},
    {0,0}
  },

// Song 5: Pac-Man Intro (High Precision)
{
  {HB,2}, {PAUSE_IDX,1}, {HB,2}, {PAUSE_IDX,1}, {HF,2}, {PAUSE_IDX,1}, {HD,2}, {PAUSE_IDX,1},
  {HB,2}, {PAUSE_IDX,1}, {HF,4}, {PAUSE_IDX,1}, {HD,8}, {PAUSE_IDX,2},
  
  {HC,2}, {PAUSE_IDX,1}, {HC,2}, {PAUSE_IDX,1}, {HG,2}, {PAUSE_IDX,1}, {HE,2}, {PAUSE_IDX,1},
  {HC,2}, {PAUSE_IDX,1}, {HG,4}, {PAUSE_IDX,1}, {HE,8}, {PAUSE_IDX,2},
  
  {HB,2}, {PAUSE_IDX,1}, {HB,2}, {PAUSE_IDX,1}, {HF,2}, {PAUSE_IDX,1}, {HD,2}, {PAUSE_IDX,1},
  {HB,2}, {PAUSE_IDX,1}, {HF,4}, {PAUSE_IDX,1}, {HD,8}, {PAUSE_IDX,2},
  
  {HD,2}, {HE,2}, {HF,2}, {PAUSE_IDX,1}, {HF,2}, {HG,2}, {HA,2}, {PAUSE_IDX,1},
  {HA,2}, {HB,2}, {HC,2}, {PAUSE_IDX,1},{PAUSE_IDX,1},{PAUSE_IDX,1},
  
  {0,0}
}
	
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
  // 1. Check if we are at the end
  if(mysong[currentSong][songIndex].delay == 0){
    songIndex = 0;
    Sound_Stop();
    DelayMs(300);
    return;
  }

  // 2. Index Guard: Capture index before the delay
  uint16_t indexAtStart = songIndex;

  // 3. Capture current note info
  uint8_t t = mysong[currentSong][songIndex].tone_index;
  uint32_t d = mysong[currentSong][songIndex].delay;

  // 4. Play the sound
  if(t == PAUSE_IDX){
    Sound_Stop();
  } else {
    uint8_t degree = t % 7;
    uint8_t base = (currentState == Low_Octave) ? 0 : (currentState == Middle_Octave ? 7 : 14);
    Sound_Start(Tone_Tab[base + degree]);
  }

  // 5. Delay
  DelayMs(d * UNIT_MS);
  Sound_Stop();
  DelayMs(20);

  // 6. Only increment if the interrupt didn't reset songIndex to 0
  if(songIndex == indexAtStart){
    songIndex++;
  }
}




void System_Init(void) {
	gpio_btn_init();
	PLL_Init();
	PortF_Init();
  PianoKeys_Init();
	DAC_Init();
	Sound_Init();
	DelayTimer_Init();
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
  // SW1 piano toggle/ autoplay
  if(GPIO_PORTF_MIS_R & SW1){
    GPIO_PORTF_ICR_R = SW1; 
		//debounce
		DelayMs(50);
    GPIO_PORTF_DATA_R &= ~LEDS; 
    GPIO_PORTF_DATA_R |= BLUE; 
    
    if(currentMode == Mode1) currentMode = Mode2;
    else currentMode = Mode1;
  }

  // SW2 Octave change / song change
  if(GPIO_PORTF_MIS_R & SW2){
    GPIO_PORTF_ICR_R = SW2; 
    DelayMs(50); // Initial debounce

    if(currentMode == Mode1){
      if(currentState == Low_Octave) currentState = Middle_Octave;
      else if(currentState == Middle_Octave) currentState = High_Octave;
      else currentState = Low_Octave;
    }
    else {
      Sound_Stop();
      currentSong++;
      songIndex = 0; // Reset index
      if(currentSong >= SONG_COUNT) currentSong = 0;
    }

    // WAIT FOR RELEASE: Prevents multiple increments per press
    while((GPIO_PORTF_DATA_R & SW2) == 0); 
    DelayMs(50); // Release debounce
    
    GPIO_PORTF_DATA_R &= ~LEDS; // Clear LED after release
  }
}



//play a song array
void play_a_song(const NTyp scoretab[]){
    uint16_t i = 0;

    while(scoretab[i].delay){

        if(scoretab[i].tone_index == PAUSE){
            Sound_Stop();
        } else {
            Sound_Start(Tone_Tab[scoretab[i].tone_index]);
        }

        DelayMs((uint32_t)scoretab[i].delay * UNIT_MS);

        Sound_Stop();
        DelayMs(10);

        i++;
    }

    DelayMs(15 * UNIT_MS);   // pause between songs
}






