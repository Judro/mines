#include "game.h"
#include <fcntl.h>
#include <limits.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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

// controls
int cmove(Game *g) {
  char ch = getch();
  Cord old = g_player_position(g);
  switch (ch) {
  case 'j':
    g_set_player_position_y(g, old.y + 1);
    if (g_player_position(g).y == g_height(g))
      g_set_player_position_y(g, 0);
    break;
  case 'k':
    g_set_player_position_y(g, old.y - 1);
    if (g_player_position(g).y == -1)
      g_set_player_position_y(g, g_height(g) - 1);
    break;
  case 'h':
    g_set_player_position_x(g, old.x - 1);
    if (g_player_position(g).x == -1)
      g_set_player_position_x(g, g_width(g) - 1);
    break;
  case 'l':
    g_set_player_position_x(g, old.x + 1);
    if (g_player_position(g).x == g_width(g))
      g_set_player_position_x(g, 0);
    break;
  case 'q':
    return -1;
    // case '$': g->cord.x = g_width(g) -1;
    //   break;
    // case '0': g->cord.x=0;
    //   break;
    // case 'g': g->cord.y=0;
    //   break;
    // case 'G': g->cord.y= g_height(g) -1;
    //   break;
  case 'f':
    return -2;
  case 's':
    return -3;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  initscr();
  int wt, hi, ch, mi;
  wt = hi = mi = 0;
  const char *errstr;

  while ((ch = getopt(argc, argv, "h:w:m:")) != -1) {
    switch (ch) {
    case 'h':
      hi = strtonum(optarg, 0, 100, &errstr);
      break;
    case 'w':
      wt = strtonum(optarg, 0, 500, &errstr);
      break;
    case 'm':
      mi = strtonum(optarg, 0, 50000, &errstr);
      break;
      // default: usaage();
      argc -= optind;
      argv += optind;
    }
  }
  if (wt == 0)
    wt = 40;
  if (hi == 0)
    hi = 10;
  if (mi == 0)
    mi = 40;
  start_color();
  Game *game = g_new(wt, hi, mi);
  while (1) {
    clear();
    print(game, 0);
    int ret = cmove(game);
    if (ret == -1) {
      break;
    } else if (ret == -2) {
      g_flag(game);
    } else if (ret == -3) {
      if (g_unveil(game) == -1) {
        clear();
        printw(" Game over     ");
        print(game, 1);
        break;
      }
    }
    if (checkflags(game)) {
      clear();
      printw("You found all %d mines!", g_flags_total(game));
      break;
    }
  }

  getch();
  endwin();
  g_kill(game);
  return 0;
}
