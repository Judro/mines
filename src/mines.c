#include "../config.h"
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

time_t fps_timestamp;
int fps_frame_counter = 0;
int fps = 0;
int sleep_time = 0;

void limit_fps() {
  time_t current;
  time(&current);
  if (current > fps_timestamp) {
    fps = fps_frame_counter;
    fps_frame_counter = 0;
    fps_timestamp = current;
    if (fps < fps_limit)
      sleep_time /= 2;
    if (fps > fps_limit + 2)
      sleep_time += 50 * (fps - fps_limit);
  }
  fps_frame_counter++;
  usleep(sleep_time);
}

void print_gameover(GameInstance game, WINDOW *window) {
  char ret = 0;
  while (1) {
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
    ret = cmove(game, window);
    if (ret == -1)
      break;
    limit_fps();
  }
}

void print_victory(GameInstance game, WINDOW *window) {
  time_t current;
  time(&current);
  char ret = 0;
  while (1) {
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
    ret = cmove(game, window);
    if (ret == -1)
      break;
    limit_fps();
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
  time(&fps_timestamp);
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
    limit_fps();
  }
  deleteGameInstance(game);
  game = select_mode(getmaxx(window), getmaxy(window));
  if (game != NULL)
    goto start;
end:
  endwin();
  return 0;
}
