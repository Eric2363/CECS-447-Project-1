//// ModuleTest.c
//// Course number: CECS 447
//// Term: Spring 2026
//// Project number: 1
//// Project description: Step 3 Module Tests
//// Team #: 4


//#include "../tm4c123gh6pm.h"

//// ================= Select ONE =================
////#define PIANOKEYS_INT_TEST
////#define PORTF_INT_TEST
////#define SYSTICK_TEST
////#define DAC_TEST
//// =============================================

//// Includes
//#if defined(PIANOKEYS_INT_TEST)
//  #include "PianoKeys.h"
//  #include "PortF.h"
//#endif

//#if defined(PORTF_INT_TEST)
//  #include "PortF.h"
//#endif

//#if defined(SYSTICK_TEST)
//  #include "SysTick.h"
//  #include "PortF.h"
//#endif

//#if defined(DAC_TEST)
//  #include "Dac.h"
//  #include "PortF.h"
//#endif

//// Prototypes
//void System_Init(void);

//#if defined(PIANOKEYS_INT_TEST)
//  void PianoKeys_Test(void);
//#endif

//#if defined(PORTF_INT_TEST)
//  void PortF_Test(void);
//#endif

//#if defined(DAC_TEST)
//  void DAC_Test(void);
//#endif

//#if defined(SYSTICK_TEST)
//  void SysTick_Test(void);
//#endif

//// ===================== TEST VARIABLES (ISR -> Main) =====================
//volatile unsigned long PortE_Hits[4] = {0,0,0,0};
//volatile unsigned long PortF_SW1_Hits = 0;
//volatile unsigned long PortF_SW2_Hits = 0;
//// =======================================================================

////int main(void){
////  System_Init();

////  while(1){

////#if defined(PIANOKEYS_INT_TEST)
////    PianoKeys_Test();
////#endif

////#if defined(PORTF_INT_TEST)
////    PortF_Test();
////#endif

////#if defined(DAC_TEST)
////    DAC_Test();
////#endif

////#if defined(SYSTICK_TEST)
////    SysTick_Test();
////#endif

////  }
////}

//void System_Init(void){

//#if defined(PIANOKEYS_INT_TEST)
//  PortF_Init();       // LEDs for evidence
//  PianoKeys_Init();   // Port E interrupt init
//#endif

//#if defined(PORTF_INT_TEST)
//  PortF_Init();       // Port F interrupt init + LEDs
//#endif

//#if defined(DAC_TEST)
//  PortF_Init();       // optional LEDs
//  DAC_Init();
//#endif

//#if defined(SYSTICK_TEST)
//  PortF_Init();
//  SysTick_InitBusyWait();
//#endif

//}

//// ===================== INTERRUPT HANDLERS =====================

//#if defined(PIANOKEYS_INT_TEST)
//void GPIOPortE_Handler(void){
//  unsigned long status = GPIO_PORTE_MIS_R & (PE0 | PE1 | PE2 | PE3);
//  GPIO_PORTE_ICR_R = status; // clear all causes FIRST

//  if(status & PE0) PortE_Hits[0]++;
//  if(status & PE1) PortE_Hits[1]++;
//  if(status & PE2) PortE_Hits[2]++;
//  if(status & PE3) PortE_Hits[3]++;
//}
//#endif

//#if defined(PORTF_INT_TEST)
//void GPIOPortF_Handler(void){
//  unsigned long status = GPIO_PORTF_MIS_R & (SW1 | SW2);
//  GPIO_PORTF_ICR_R = status;

//  if(status & SW1) PortF_SW1_Hits++;
//  if(status & SW2) PortF_SW2_Hits++;
//}
//#endif

//// ===================== TEST LOOPS =====================

//#if defined(PIANOKEYS_INT_TEST)
//void PianoKeys_Test(void){
//  static unsigned long last[4] = {0,0,0,0};

//  if(PortE_Hits[0] != last[0]) { last[0] = PortE_Hits[0]; GPIO_PORTF_DATA_R = RED; }
//  if(PortE_Hits[1] != last[1]) { last[1] = PortE_Hits[1]; GPIO_PORTF_DATA_R = BLUE; }
//  if(PortE_Hits[2] != last[2]) { last[2] = PortE_Hits[2]; GPIO_PORTF_DATA_R = GREEN; }
//  if(PortE_Hits[3] != last[3]) { last[3] = PortE_Hits[3]; GPIO_PORTF_DATA_R = (RED|BLUE); }
//}
//#endif

//#if defined(PORTF_INT_TEST)
//void PortF_Test(void){
//  static unsigned long lastSW1 = 0;
//  static unsigned long lastSW2 = 0;

//  if(PortF_SW1_Hits != lastSW1) { lastSW1 = PortF_SW1_Hits; GPIO_PORTF_DATA_R = BLUE; }
//  if(PortF_SW2_Hits != lastSW2) { lastSW2 = PortF_SW2_Hits; GPIO_PORTF_DATA_R = RED;  }
//}
//#endif

//#if defined(DAC_TEST)
//static void Delay(volatile unsigned long t){
//  while(t--) {}
//}

//void DAC_Test(void){
//  DAC_Out(0x00);            // 0%
//  GPIO_PORTF_DATA_R = RED;
//  Delay(800000);

//  DAC_Out(0x20);            // mid (32)
//  GPIO_PORTF_DATA_R = BLUE;
//  Delay(800000);

//  DAC_Out(0x3F);            // max (63)
//  GPIO_PORTF_DATA_R = GREEN;
//  Delay(800000);
//}
//#endif

//#if defined(SYSTICK_TEST)
//void SysTick_Test(void){
//  GPIO_PORTF_DATA_R ^= RED;
//  for(int i=0; i<500; i++){
//    SysTick_Wait1ms();
//  }
//}
//#endif
