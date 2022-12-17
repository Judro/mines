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

void print_gameover(GameInstance game, WINDOW *window) {
  char ret = 0;
  erase();
  print_top_margin(getmaxy(window), g_height(game));
  print_left_margin(getmaxx(window), g_width(game));
  printw(" Game over     \n");
  PrintableInstance gp = createPrintableGameover(game);
  print(gp, getmaxx(window), g_width(game));
  printw("\n");
  print_left_margin(getmaxx(window), g_width(game));
  printw(" Press <q>\n");
  deletePrintable(gp);
  while (1) {
    ret = cmove(game, window);
    if (ret == -1)
      break;
  }
}

void print_victory(GameInstance game, WINDOW *window) {
  time_t current;
  time(&current);
  char ret = 0;
  erase();
  print_top_margin(getmaxy(window), g_height(game));
  print_left_margin(getmaxx(window), g_width(game));
  printw(" Congratulations!\n");
  PrintableInstance gp = createPrintableGameover(game);
  print(gp, getmaxx(window), g_width(game));
  print_left_margin(getmaxx(window), g_width(game));
  char *hm = get_left_margin(g_width(game) - 16);
  printw(" Press <q>%s[%02ld:%02ld]\n", hm, (current - g_start(game)) / 60,
         (current - g_start(game)) % 60);
  free(hm);
  deletePrintable(gp);
  while (1) {
    ret = cmove(game, window);
    if (ret == -1)
      break;
  }
}

void print_game(GameInstance game, WINDOW *window) {
  erase();
  print_top_margin(getmaxy(window), g_height(game));
  PrintableInstance gp = createPrintable(game);
  PrintableHeaderInstance gph = createPrintableHeader(game);
  print(gp, getmaxx(window), g_width(game));
  print_header(gph, getmaxx(window), g_width(game));
  free(gph);
  deletePrintable(gp);
}

int main(int argc, char *argv[]) {
  setlocale(LC_CTYPE, "en_US.UTF-8");
  WINDOW *window = initscr();
  start_color();
  curs_set(0);
  GameInstance game = select_mode(getmaxx(window), getmaxy(window));
  if (game == NULL) {
    goto end;
  }

start:
  while (1) {
    print_game(game, window);
    int ret = cmove(game, window);
    if (ret == -1) {
      break;
    } else if (ret == -2) {
      g_flag(game);
    } else if (ret == -3) {
      if (g_unveil(game) == -1) {
        print_gameover(game, window);
        break;
      }
    }
    if (checkflags(game)) {
      print_victory(game, window);
      break;
    }
    // sleeps 0.01 seconds
    usleep(10000);
  }
  deleteGameInstance(game);
  game = select_mode(getmaxx(window), getmaxy(window));
  if (game != NULL)
    goto start;
end:
  endwin();
  return 0;
}
