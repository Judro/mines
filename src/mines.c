#include "controls.h"
#include "display.h"
#include "game.h"
#include "menu.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]) {
  initscr();
  start_color();
  curs_set(0);
  Game *game = select_mode();
  if (game == NULL) {
    goto end;
  }
start:
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
  g_kill(game);
  game = select_mode();
  if (game != NULL)
    goto start;
  getch();
end:
  endwin();
  return 0;
}
