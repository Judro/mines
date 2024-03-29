#include "display.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

void print_top_margin(unsigned int terminal_y, unsigned int game_y) {
  char *tm = get_top_margin(terminal_y / 2 - ((game_y + 5) / 2));
  printw("%s", tm);
  free(tm);
}
void print_left_margin(unsigned int terminal_x, unsigned int game_x) {
  char *lm = get_left_margin(terminal_x / 2 - ((game_x + 2) / 2));
  printw("%s", lm);
  free(lm);
}

char *get_top_margin(unsigned int l) {
  if (l > 1000)
    l = 1;
  char *ret = calloc(l + 1, sizeof(char));
  for (int i = 0; i < l; i++) {
    ret[i] = '\n';
  }
  return ret;
}
char *get_left_margin(unsigned int l) {
  if (l > 1000)
    l = 1;
  char *ret = calloc(l + 1, sizeof(char));
  for (int i = 0; i < l; i++) {
    ret[i] = ' ';
  }
  return ret;
}

void print_header(PrintableHeaderInstance gph, unsigned int terminal_x,
                  unsigned int game_x) {
  print_left_margin(terminal_x, game_x);
  printw("│");
  int mtw = 15;
  if (gph->width >= 25) {
    printw("  %03d  │", gph->mines);
    mtw = 17;
  } else {
    printw(" %03d │", gph->mines);
  }
  for (int i = 0; i < gph->width - mtw; i++) {
    if (i == (gph->width - mtw) / 2) {
      switch (gph->state) {
      case Playing:
        printw(" ");
        break;
      case Won:
        printw("☆");
        break;
      case Lost:
        printw("☠");
      }
      continue;
    }
    printw(" ");
  }
  printw(" │ %02ld:%02ld │\n", gph->time / 60, gph->time % 60);
  print_left_margin(terminal_x, game_x);
  printw("└");
  int mrb = 5;
  if (gph->width >= 25)
    mrb = 7;
  for (int i = 0; i < gph->width; i++) {
    if (i == mrb || i == gph->width - 8) {
      printw("┴");
      continue;
    }
    printw("─");
  }
  printw("┘\n");
}

void print(PrintableInstance gp, unsigned int terminal_x, unsigned int game_x) {
  char is_in_radius = 0;
  extern char g_helper_mode;
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  print_left_margin(terminal_x, game_x);
  printw("┌");
  for (int i = 0; i < gp->width; i++) {
    printw("─");
  }
  printw("┐\n");
  for (int i = 0; i < gp->width * gp->height; i++) {
    if (i % gp->width == 0 && i > 0) {
      printw("│\n");
    }
    if (i % gp->width == 0) {
      print_left_margin(terminal_x, game_x);
      printw("│");
    }
    if (i == (gp->player.y * gp->width + gp->player.x))
      attron(A_STANDOUT);
    if ((i == gp->player.y * gp->width + gp->player.x + 1 ||
         i == gp->player.y * gp->width + gp->player.x - 1 ||
         i == gp->player.y * gp->width + gp->player.x + 1 + gp->width ||
         i == gp->player.y * gp->width + gp->player.x - 1 + gp->width ||
         i == gp->player.y * gp->width + gp->player.x + 1 - gp->width ||
         i == gp->player.y * gp->width + gp->player.x - 1 - gp->width ||
         i == gp->player.y * gp->width + gp->player.x - gp->width ||
         i == gp->player.y * gp->width + gp->player.x + gp->width) &&
        i % gp->width >= 0 && abs(i % gp->width - gp->player.x) < 2 &&
        g_helper_mode) {
      is_in_radius = 1;
      attron(A_BOLD);
    } else {
      attroff(A_BOLD);
      is_in_radius = 0;
    }

    switch (gp->fields[i]) {
    case UNTOUCHED:
      if (is_in_radius) {
        printw("▒");
      } else {
        printw("░");
      }
      break;
    case UNVEILED:
      printw(" ");
      break;
    case FLAG:
      attron(COLOR_PAIR(6));
      printw("+");
      attroff(COLOR_PAIR(6));
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
      attron(COLOR_PAIR(4));
      printw("6");
      attroff(COLOR_PAIR(4));
      break;
    case SEVEN:
      attron(COLOR_PAIR(5));
      printw("7");
      attroff(COLOR_PAIR(5));
      break;
    case EIGHT:
      attron(COLOR_PAIR(5));
      printw("8");
      attroff(COLOR_PAIR(5));
      break;
    }
    if (i == (gp->player.y * gp->width + gp->player.x))
      attroff(A_STANDOUT);
  }
  printw("│\n");
  print_left_margin(terminal_x, game_x);
  printw("├");
  int mrb = 5;
  if (gp->width >= 25)
    mrb = 7;
  for (int i = 0; i < gp->width; i++) {
    if (i == mrb || i == gp->width - 8) {
      printw("┬");
      continue;
    }
    printw("─");
  }
  printw("┤\n");
}
