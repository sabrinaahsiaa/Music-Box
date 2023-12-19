#ifndef SYSTICK_H
#define SYSTICK_H
// Initialize SysTick with busy wait running at bus clock.
void SysTick_Init(void);
void SysTick_start(void);
void SysTick_stop(void);
void SysTick_Set_Current_Note(unsigned long n_value);
#endif
