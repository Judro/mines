#include "display.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>



char *generate_top_margin(unsigned int l) {
  if (l > 1000)
    l = 1;
  char *ret = calloc(l + 1, sizeof(char));
  for (int i = 0; i < l; i++) {
    ret[i] = '\n';
  }
  return ret;
}
char *generate_left_margin(unsigned int l) {
  if (l > 1000)
    l = 1;
  char *ret = calloc(l + 1, sizeof(char));
  for (int i = 0; i < l; i++) {
    ret[i] = ' ';
  }
  return ret;
}

void print_top_margin(unsigned int terminal_y, unsigned int game_y) {
  char *tm = generate_top_margin(terminal_y / 2 - ((game_y + 5) / 2));
  printw("%s", tm);
  free(tm);
}
void print_left_margin(unsigned int terminal_x, unsigned int game_x) {
  char *lm = generate_left_margin(terminal_x / 2 - ((game_x + 2) / 2));
  printw("%s", lm);
  free(lm);
}

void print_header(GameView view, unsigned int terminal_x,
                  unsigned int game_x) {
  print_left_margin(terminal_x, game_x);
  printw("│");
  int mtw = 15;
  if (view->width >= 25) {
    printw("  %03d  │", view->mines);
    mtw = 17;
  } else {
    printw(" %03d │", view->mines);
  }
  for (int i = 0; i < view->width - mtw; i++) {
    if (i == (view->width - mtw) / 2) {
      switch (view->state) {
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
  printw(" │ %02ld:%02ld │\n", view->time / 60, view->time % 60);
  print_left_margin(terminal_x, game_x);
  printw("└");
  int mrb = 5;
  if (view->width >= 25)
    mrb = 7;
  for (int i = 0; i < view->width; i++) {
    if (i == mrb || i == view->width - 8) {
      printw("┴");
      continue;
    }
    printw("─");
  }
  printw("┘\n");
}

void print(GameView view, unsigned int terminal_x, unsigned int game_x) {
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
  for (int i = 0; i < view->width; i++) {
    printw("─");
  }
  printw("┐\n");
  for (int i = 0; i < view->width * view->height; i++) {
    if (i % view->width == 0 && i > 0) {
      printw("│\n");
    }
    if (i % view->width == 0) {
      print_left_margin(terminal_x, game_x);
      printw("│");
    }
    if (i == (view->player.y * view->width + view->player.x))
      attron(A_STANDOUT);
    if ((i == view->player.y * view->width + view->player.x + 1 ||
         i == view->player.y * view->width + view->player.x - 1 ||
         i == view->player.y * view->width + view->player.x + 1 + view->width ||
         i == view->player.y * view->width + view->player.x - 1 + view->width ||
         i == view->player.y * view->width + view->player.x + 1 - view->width ||
         i == view->player.y * view->width + view->player.x - 1 - view->width ||
         i == view->player.y * view->width + view->player.x - view->width ||
         i == view->player.y * view->width + view->player.x + view->width) &&
        i % view->width >= 0 && abs(i % view->width - view->player.x) < 2 &&
        g_helper_mode) {
      is_in_radius = 1;
      attron(A_BOLD);
    } else {
      attroff(A_BOLD);
      is_in_radius = 0;
    }

    switch (view->cells[i]) {
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
    if (i == (view->player.y * view->width + view->player.x))
      attroff(A_STANDOUT);
  }
  printw("│\n");
  print_left_margin(terminal_x, game_x);
  printw("├");
  int mrb = 5;
  if (view->width >= 25)
    mrb = 7;
  for (int i = 0; i < view->width; i++) {
    if (i == mrb || i == view->width - 8) {
      printw("┬");
      continue;
    }
    printw("─");
  }
  printw("┤\n");
  print_header(view, terminal_x, game_x);
}
