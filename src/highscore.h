#ifndef HIGHSCORE_H
#define HIGHSCORE_H
#include <stdio.h>
#include <time.h>

extern const unsigned highscore_capacity;

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
UserHighscore *load_highscores(struct highscore cmp);
int save_highscore(Highscore, FILE *);
char **userHighscores2string(UserHighscore *highscores);
FILE *init_state_files();

#endif
