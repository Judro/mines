#include "controls.h"
#include "display.h"
#include "game.h"
#include "menu.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  WINDOW *window = initscr();
  start_color();
  curs_set(0);
  Game *game = select_mode();
  if (game == NULL) {
    goto end;
  }
start:
  while (1) {
    erase();
    time_t current;
    time(&current);
    print_header(g_flags_total(game) - g_flags_found(game), g_start(game),
                 current);
    GPrintable *gp = g_printable(game);
    print(gp);
    g_gprintable_kill(gp);
    int ret = cmove(game, window);
    if (ret == -1) {
      break;
    } else if (ret == -2) {
      g_flag(game);
    } else if (ret == -3) {
      if (g_unveil(game) == -1) {
        erase();
        printw(" Game over     \n");
        GPrintable *gp = g_printable_gameover(game);
        print(gp);
        g_gprintable_kill(gp);
        break;
      }
    }
    if (checkflags(game)) {
      erase();
      printw("You found all %d mines!", g_flags_total(game));
      break;
    }
  }
  g_kill(game);
  getch();
  game = select_mode();
  if (game != NULL)
    goto start;
end:
  endwin();
  return 0;
}
