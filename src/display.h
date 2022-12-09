#include "game.h"
#include <time.h>
#ifndef DISPLAY_H
#define DISPLAY_H
char *get_top_margin(unsigned int);
char *get_left_margin(unsigned int);
void print(GPrintable *, unsigned int, unsigned int);
void print_header(GPrintableH *, unsigned int, unsigned int);
void print_top_margin(unsigned int, unsigned int);
void print_left_margin(unsigned int, unsigned int);
#endif
