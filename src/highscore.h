#ifndef HIGHSCORE_H
#define HIGHSCORE_H
#include <stdio.h>
#include <time.h>

typedef struct Highscore {
  unsigned width;
  unsigned height;
  unsigned mines;
  unsigned time;
  time_t date;
} Highscore;

// last element in array will be NULL
Highscore *load_highscores();
int save_highscore(Highscore, FILE *);
FILE *init_state_files();
#endif
