#include "display.h"
#include <ncurses.h>
#include <time.h>

void print_header(unsigned int mines_left, time_t start, time_t current) {
  printw(" [%ld:%ld] Mines left: %d\n", (current - start) / 60,
         (current - start) % 60, mines_left);
}

void print(GPrintable *gp) {
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  printw(" ");
  for (int i = 0; i < gp->width; i++) {
    printw("_");
  }
  printw("\n");
  for (int i = 0; i < gp->width * gp->height; i++) {
    if (i % gp->width == 0 && i > 0)
      printw("|\n");
    if (i % gp->width == 0)
      printw("|");
    if (i == (gp->player.y * gp->width + gp->player.x))
      attron(A_REVERSE);

    switch (gp->fields[i]) {
    case UNTOUCHED:
      printw("0");
      break;
    case UNVEILED:
      printw(" ");
      break;
    case FLAG:
      printw("P");
      break;
    case FALSE_FLAG:
      printw("X");
      break;
    case FLAG_NOT_FOUND:
      printw("+");
      break;
    case ONE:
      attron(COLOR_PAIR(1));
      printw("1");
      attroff(COLOR_PAIR(1));
      break;
    case TWO:
      attron(COLOR_PAIR(2));
      printw("2");
      attroff(COLOR_PAIR(2));
      break;
    case THREE:
      attron(COLOR_PAIR(3));
      printw("3");
      attroff(COLOR_PAIR(3));
      break;
    case FOUR:
      attron(COLOR_PAIR(4));
      printw("4");
      attroff(COLOR_PAIR(4));
      break;
    case FIVE:
      attron(COLOR_PAIR(5));
      printw("5");
      attroff(COLOR_PAIR(5));
      break;
    case SIX:
      attron(COLOR_PAIR(6));
      printw("6");
      attroff(COLOR_PAIR(6));
      break;
    case SEVEN:
      attron(COLOR_PAIR(7));
      printw("7");
      attroff(COLOR_PAIR(7));
      break;
    case EIGHT:
      attron(COLOR_PAIR(8));
      printw("8");
      attroff(COLOR_PAIR(8));
      break;
    }
    if (i == (gp->player.y * gp->width + gp->player.x))
      attroff(A_REVERSE);
  }
  printw("|\n'");
  for (int i = 0; i < gp->width; i++) {
    printw("=");
  }
  printw("'\n");
}
