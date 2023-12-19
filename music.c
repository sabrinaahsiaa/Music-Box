#include "tm4c123gh6pm.h"
#include "music.h"
#include "SysTick.h"

void Delay(void);

// global variables to be used
unsigned char i=0, j=0, m=0;
unsigned char playing=0;

// initial values for piano major notes: assume SysTick clock is 16MHz.
const unsigned long Tone_Tab[] =
// initial values for three major notes for 16MHz system clock
// Note name: C, D, E, F, G, A, B
// Offset:0, 1, 2, 3, 4, 5, 6
{30534,27211,24242,22923,20408,18182,16194, // C4 major notes
 15289,13621,12135,11454,10204,9091,8099, // C5 major notes
 7645,6810,6067,5727,5102,4545,4050};// C6 major notes

// Index for notes used in music scores
#define C5 0+7
#define D5 1+7
#define E5 2+7
#define F5 3+7
#define G5 4+7
#define A5 5+7
#define B5 6+7
#define C6 0+2*7
#define D6 1+2*7
#define E6 2+2*7
#define F6 3+2*7
#define G6 4+2*7
#define A6 5+2*7
#define B6 6+2*7

#define PAUSE 255				// assume there are less than 255 tones used in any song
#define MAX_NOTES 50  // assume maximum number of notes in any song is 100. You can change this value if you add a long song.
 
// doe ray mi fa so la ti 
// C   D   E  F  G  A  B
NTyp Score_Tab[][MAX_NOTES] = {  
// score table for Happy Birthday
{//so   so   la   so   doe' ti
   PAUSE, 4, G5,2,G5,2,A5,4,G5,4,C6,4,B5,4,
// pause so   so   la   so   ray' doe'
   PAUSE,4,  G5,2,G5,2,A5,4,G5,4,D6,4,C6,4,
// pause so   so   so'  mi'  doe' ti   la
   PAUSE,4,  G5,2,G5,2,G6,4,E6,4,C6,4,B5,4,A5,8, 
// pause fa'  fa'   mi'  doe' ray' doe' stop
	 PAUSE,4,  F6,2,F6,2, E6,4,C6,4,D6,4,C6,8,0,0},
	
// score table for Mary Had A Little Lamb
{PAUSE, 4, E6, 4, D6, 4, C6, 4, D6, 4, E6, 4, E6, 4, E6, 8, 
 D6, 4, D6, 4, D6, 8, E6, 4, G6, 4, G6, 8,
 E6, 4, D6, 4, C6, 4, D6, 4, E6, 4, E6, 4, E6, 8, 
 D6, 4, D6, 4, E6, 4, D6, 4, C6, 8, 0, 0 },

// score table for Twinkle Twinkle Little Stars
{PAUSE, 4, C6,4,C6,4,G6,4,G6,4,A6,4,A6,4,G6,8,F6,4,F6,4,E6,4,E6,4,D6,4,D6,4,C6,8, 
 G6,4,G6,4,F6,4,F6,4,E6,4,E6,4,D6,8,G6,4,G6,4,F6,4,F6,4,E6,4,E6,4,D6,8, 
 C6,4,C6,4,G6,4,G6,4,A6,4,A6,4,G6,8,F6,4,F6,4,E6,4,E6,4,D6,4,D6,4,C6,8,0,0}
};

void play_a_song(void)
{
	j=0;
	playing = 1;
	if (playing == 1) {
		while (Score_Tab[i][j].delay && playing == 1) {
			if (Score_Tab[i][j].tone_index == PAUSE) // index = 255 indicate a pause: stop systick
				SysTick_stop(); // silence tone, turn off SysTick timer
			else {
				SysTick_Set_Current_Note(Tone_Tab[Score_Tab[i][j].tone_index]);
				SysTick_start();
			}
			
			// tempo control: 
			// play current note for duration 
			// specified in the music score table
			for (m=0;m<Score_Tab[i][j].delay;m++) 
				Delay();
			
			SysTick_stop();
			
			j++;  // move to the next note
		}
		
		// pause after each play
		for (m=0;m<15;m++) 
			Delay();
	}
}

void next_song(void)
{
	SysTick_stop();
	for (m=0;m<2;m++) {
		Delay();
	}
	j=0;
	i++;
	i=i%3;
}

unsigned char is_music_on(void)
{
	if (playing == 1) {
		return 1;
	}
	else {
		return 0;
	}
} 

void turn_off_music(void)
{
	if (playing == 1) {
		playing = 0;
		SysTick_stop();
	}
}

void turn_on_music(void)
{
	if (playing == 0) {
		playing = 1;
		j=0;
		i=0;
	}
	else {
	turn_off_music();
	}
}

// Make PA3 an output to the speaker, enable digital I/O, ensure alt. functions off
void Music_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x01;           // 1) activate clock for Port A
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
                                    // 2) no need to unlock PA2
  GPIO_PORTA_PCTL_R &= ~0x0000F000; // 3) regular GPIO
  GPIO_PORTA_AMSEL_R &= ~0x08;      // 4) disable analog function on PA3
  GPIO_PORTA_DIR_R |= 0x08;         // 5) set direction to output
  GPIO_PORTA_AFSEL_R &= ~0x08;      // 6) regular port function
  GPIO_PORTA_DEN_R |= 0x08;         // 7) enable digital port
  GPIO_PORTA_DR8R_R |= 0x08;        // 8) optional: enable 8 mA drive on PA3 to increase the voice volume
}

// Subroutine to wait 0.1 sec
// Inputs: None
// Outputs: None
// Notes: ...
void Delay(void){
	unsigned long volatile time;
  time = 727240*20/91;  // 0.1sec for 16MHz
//  time = 727240*100/91;  // 0.1sec for 80MHz
  while(time){
		time--;
  }
}
