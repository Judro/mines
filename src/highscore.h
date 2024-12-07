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
extern char print_highscore_flag;
// last element in array will have user set to NULL
UserHighscore *load_highscores();
void filter_highscores(UserHighscore *highscores, struct highscore cmp);
int save_highscore(Highscore, FILE *);
FILE *init_state_files();

#endif
