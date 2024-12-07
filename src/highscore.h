#ifndef HIGHSCORE_H
#define HIGHSCORE_H
#include <stdio.h>
#include <time.h>

typedef struct highscore {
  unsigned width;
  unsigned height;
  unsigned mines;
  unsigned time;
  time_t date;
} Highscore;

typedef struct user_highscore {
  char *user;
  struct highscore highscore;
} UserHighscore;

extern const char *save_directory;
// last element in array will be NULL
UserHighscore *load_highscores();
int save_highscore(Highscore, FILE *);
FILE *init_state_files();

#endif
