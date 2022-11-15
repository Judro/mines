#include "display.h"
#include <ncurses.h>

void print(Game *g, char game_over) {
  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);
  init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(5, COLOR_YELLOW, COLOR_BLACK);
  init_pair(6, COLOR_CYAN, COLOR_BLACK);
  printw(" Flags: %d\n", g_flags_total(g) - g_flags_found(g));
  printw(" ");
  for (int i = 0; i < g_width(g); i++) {
    printw("_");
  }
  printw("\n");
  for (int i = 0; i < g_height(g); i++) {
    printw("|");
    for (int j = 0; j < g_width(g); j++) {
      if (i == g_player_position(g).y && j == g_player_position(g).x)
        attron(A_REVERSE);
      if (g_flaged(g, j, i)) {
        if (game_over && !g_mine(g, j, i)) {
          printw("X");
        } else {
          printw("P");
        }
      } else if (g_unveiled(g, j, i) && g_plain(g, j, i)) {
        printw(" ");
      } else if (g_plain(g, j, i)) {
        printw("O");
      } else if (g_unveiled(g, j, i)) {
        switch (g_amount_near_mines(g, j, i)) {
        case 1:
          attron(COLOR_PAIR(1));
          break;
        case 2:
          attron(COLOR_PAIR(2));
          break;
        case 3:
          attron(COLOR_PAIR(3));
          break;
        case 4:
          attron(COLOR_PAIR(4));
          break;
        case 5:
          attron(COLOR_PAIR(5));
          break;
        case 6:
          attron(COLOR_PAIR(6));
          break;
        }
        printw("%d", g_amount_near_mines(g, j, i));
        attroff(COLOR_PAIRS);
      } else {
        if (game_over && g_player_position(g).y == i &&
            g_player_position(g).x == j) {
          printw("X");
        } else {
          printw("O");
        }
      }
      if (i == g_player_position(g).y && j == g_player_position(g).x)
        attroff(A_REVERSE);
    }
    printw("|");
    printw("\n");
  }
  printw("|");
  for (int i = 0; i < g_width(g); i++) {
    printw("=");
  }
  printw("|\n");
}
