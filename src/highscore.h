#ifndef HIGHSCORE_H
#define HIGHSCORE_H
#include <time.h>

typedef struct Highscore {
  char *user;
  unsigned width;
  unsigned height;
  unsigned mines;
  time_t time;
} Highscore;

// last element in array will be NULL
Highscore *load_highscores();
int save_highscore(Highscore);
void init_state_files();
#endif
