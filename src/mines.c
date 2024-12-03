#include "../config.h"
#include "controls.h"
#include "display.h"
#include "game.h"
#include "highscore.h"
#include "menu.h"
#include <locale.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

time_t fps_timestamp;
int fps_frame_counter = 0;
int fps = 60;
int sleep_time = 10000;

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

void print_game(GameInstance game, WINDOW *window) {
  erase();
  print_top_margin(getmaxy(window), field_height(game) + 2);
  GameView view;
  if (game_state(game) == Lost) {
    view = createViewGameover(game);
  } else {
    view = createView(game);
  }
  print(view, getmaxx(window), field_width(game));
  deleteView(view);
}

int main(int argc, char *argv[]) {
  FILE *local_highscores = init_state_files();
  if (local_highscores == NULL)
    exit(EXIT_FAILURE);
  extern char g_helper_mode;
  g_helper_mode = 1;
  time(&fps_timestamp);
  setlocale(LC_CTYPE, "en_US.UTF-8");
  WINDOW *window = initscr();
  start_color();
  curs_set(0);
  noecho();
  GameInstance game = select_mode(getmaxx(window), getmaxy(window));
  if (game == NULL) {
    goto end;
  }

start:
  while (1) {
    print_game(game, window);
    validate_flags(game);
    if (cmove(game, window) == -1)
      break;
    switch (game_state(game)) {
    case Playing:
      break;
    case Won:
      print_game(game, window);
      Highscore highscore = generate_highscore(game);
      save_highscore(highscore, local_highscores);
    case Lost:
      print_game(game, window);
      while (1) {
        if (cmove(game, window) == -1)
          goto new_game;
        limit_fps();
      }
    }
    limit_fps();
  }
new_game:
  deleteGameInstance(game);
  nodelay(window, 0);
  game = select_mode(getmaxx(window), getmaxy(window));
  if (game != NULL)
    goto start;
end:
  endwin();
  fclose(local_highscores);
  return 0;
}
