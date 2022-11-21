#include "controls.h"
#include "display.h"
#include "game.h"
#include <fcntl.h>
#include <limits.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
    print_header(g_flags_total(game) - g_flags_found(game));
    GPrintable *gp = g_printable(game);
    print(gp);
    g_gprintable_kill(gp);
    int ret = cmove(game);
    if (ret == -1) {
      break;
    } else if (ret == -2) {
      g_flag(game);
    } else if (ret == -3) {
      if (g_unveil(game) == -1) {
        clear();
        printw(" Game over     \n");
        GPrintable *gp = g_printable_gameover(game);
        print(gp);
        g_gprintable_kill(gp);
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
