#include "controls.h"
#include "display.h"
#include "game.h"
#include "menu.h"
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  setlocale(LC_CTYPE, "en_US.UTF-8");
  WINDOW *window = initscr();
  start_color();
  curs_set(0);
  Game *game = select_mode(getmaxx(window), getmaxy(window));
  if (game == NULL) {
    goto end;
  }

start:
  while (1) {
    erase();
    time_t current;
    time(&current);
    print_top_margin(getmaxy(window), g_height(game));
    GPrintable *gp = g_printable(game);
    GPrintableH *gph = g_printableH(game);
    print(gp, getmaxx(window), g_width(game));
    print_header(gph, getmaxx(window), g_width(game));
    free(gph);
    g_gprintable_kill(gp);
    int ret = cmove(game, window);
    if (ret == -1) {
      break;
    } else if (ret == -2) {
      g_flag(game);
    } else if (ret == -3) {
      if (g_unveil(game) == -1) {
        erase();
        print_top_margin(getmaxy(window), g_height(game));
        print_left_margin(getmaxx(window), g_width(game));
        printw(" Game over     \n");
        GPrintable *gp = g_printable_gameover(game);
        print(gp, getmaxx(window), g_width(game));
        printw("\n");
        print_left_margin(getmaxx(window), g_width(game));
        printw(" Press <q>\n");
        g_gprintable_kill(gp);
        while (1) {
          ret = cmove(game, window);
          if (ret == -1)
            break;
        }
        break;
      }
    }
    if (checkflags(game)) {
      erase();
      print_top_margin(getmaxy(window), g_height(game));
      print_left_margin(getmaxx(window), g_width(game));
      printw(" Congratulations!\n");
      GPrintable *gp = g_printable_gameover(game);
      print(gp, getmaxx(window), g_width(game));
      print_left_margin(getmaxx(window), g_width(game));
      char *hm = get_left_margin(g_width(game) - 16);
      printw(" Press <q>%s[%02ld:%02ld]\n", hm, (current - g_start(game)) / 60,
             (current - g_start(game)) % 60);
      free(hm);
      g_gprintable_kill(gp);
      while (1) {
        ret = cmove(game, window);
        if (ret == -1)
          break;
      }
      break;
    }
    // sleeps 0.01 seconds
    usleep(10000);
  }
  g_kill(game);
  game = select_mode(getmaxx(window), getmaxy(window));
  if (game != NULL)
    goto start;
end:
  endwin();
  return 0;
}
