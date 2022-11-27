#include "game.h"
#include <time.h>
#ifndef DISPLAY_H
#define DISPLAY_H
char *get_top_margin(unsigned int);
char *get_left_margin(unsigned int);
void print(GPrintable *, char *);
void print_header(GPrintableH *, char *);
#endif
