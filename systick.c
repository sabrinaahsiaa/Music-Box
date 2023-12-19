#include "tm4c123gh6pm.h"
#include "music.h"

#define SPEAKER (*((volatile unsigned long *)0x40004020)) // PA3
#define Speaker_Toggle_Mask     0x00000008  // mask used to toggle the speaker output

// Initialize SysTick with interrupt priority 2. Do not start it.
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   // disable SysTick during setup
  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; // priority 2
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_CLK_SRC+NVIC_ST_CTRL_INTEN;
}

// start running systick timer
void SysTick_start(void)
{
	NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE;
}
// stop running systick timer
void SysTick_stop(void)
{
	NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE;
}

// update the current note with n_value
void SysTick_Set_Current_Note(unsigned long n_value)
{
  NVIC_ST_RELOAD_R = n_value-1;// update reload value
  NVIC_ST_CURRENT_R = 0;                // any write to current clears it
}

// Interrupt service routine, 
// frequency is determined by current tone being played:
// stop systick timer, toggle speaker output, update reload value with current note reload value, 
// clear current register and restart the systick
void SysTick_Handler(void){
	SysTick_stop(); //clr bit 0
	SPEAKER ^= Speaker_Toggle_Mask; // inverse bit 3
  NVIC_ST_CURRENT_R = 0;      // any write to current clears it
	SysTick_start();
}
