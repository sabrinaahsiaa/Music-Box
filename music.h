#ifndef MUSIC_H
#define MUSIC_H

// define music note data structure 
struct Note {
  unsigned char tone_index;
  unsigned char delay;
};
typedef const struct Note NTyp;

// Constant definitions
#define NUM_SONGS 3

// Function prototypes
void play_a_song(void);
void Music_Init(void);
void next_song(void);
unsigned char is_music_on(void);

void turn_off_music(void);

void turn_on_music(void);

#endif
