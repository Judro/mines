#include "game.h"
#include <ncurses.h>
#include <time.h>
#ifndef DISPLAY_H
#define DISPLAY_H
extern const unsigned highscore_window_height;

void print(GameView, unsigned int, unsigned int);
void print_top_margin(unsigned int, unsigned int);

void print_scrollable(char **text, unsigned terminal_width,
                      unsigned text_width);

WINDOW *create_window();
#endif
