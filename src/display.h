#include "game.h"
#include <time.h>
#ifndef DISPLAY_H
#define DISPLAY_H
void print(GPrintable *);
void print_header(unsigned int, time_t start, time_t current);
#endif
