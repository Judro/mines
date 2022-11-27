#include "display.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

char *get_top_margin(unsigned int l) {
  char *ret = calloc(l + 1, sizeof(char));
  for (int i = 0; i < l; i++) {
    ret[i] = '\n';
  }
  return ret;
}
char *get_left_margin(unsigned int l) {
  char *ret = calloc(l + 1, sizeof(char));
  for (int i = 0; i < l; i++) {
    ret[i] = ' ';
  }
  return ret;
}

void print_header(GPrintableH *gph, char *lm) {
  char *wm = get_left_margin(gph->width - 10);
  printw(lm);
  printw(" %s[%02d %02ld:%02ld]\n", wm, gph->mines, gph->time / 60,
         gph->time % 60);
  free(wm);
}

void print(GPrintable *gp, char *lm) {
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  printw(lm);
  printw(" ");
  for (int i = 0; i < gp->width; i++) {
    printw("_");
  }
  printw("\n");
  for (int i = 0; i < gp->width * gp->height; i++) {
    if (i % gp->width == 0 && i > 0)
      printw("|\n");
    if (i % gp->width == 0) {
      printw(lm);
      printw("|");
    }
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
  printw("|\n");
  printw(lm);
  printw("'");
  for (int i = 0; i < gp->width; i++) {
    printw("=");
  }
  printw("'\n");
}
