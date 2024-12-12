#include "../config.h"
#include "controls.h"
#include "display.h"
#include "game.h"
#include "helper.h"
#include "highscore.h"
#include "menu.h"
#include <limits.h>
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

void print_highscore(unsigned terminal_width, unsigned terminal_height,
                     GameInstance game) {
  unsigned scroll_index = 0;
  struct highscore cmp;
  cmp.width = field_width(game);
  cmp.height = field_height(game);
  cmp.mines = total_mines(game);
  UserHighscore *highscores = load_highscores(cmp);
  char **printable_highscores = userHighscores2string(highscores);
  struct dimension text_max =
      buff_max_dimensions(printable_highscores, highscore_capacity);
  while (1) {
    erase();
    print_top_margin(terminal_height, highscore_window_height + 2);
    print_scrollable(printable_highscores + scroll_index, terminal_width,
                     text_max.width);
    char ch = getch();
    switch (ch) {
    case 'j':
    case 0x42:
      if (scroll_index + highscore_window_height < text_max.len)
        scroll_index++;
      break;
    case 0x41:
    case 'k':
      scroll_index--;
      if (scroll_index == UINT_MAX)
        scroll_index = 0;
      break;
    case 0x44:
      break;
    case 'q':
    case 'b':
      clear_char_buff(printable_highscores, highscore_capacity);
      print_highscore_flag ^= 1;
      return;
    }
    limit_fps();
  }
}

void print_game(GameInstance game, WINDOW *window) {
  erase();
  print_top_margin(getmaxy(window), field_height(game) + 2);
  GameView view;
  if (print_highscore_flag) {
    print_highscore(getmaxx(window), getmaxy(window), game);
  } else {
    if (game_state(game) == LOST) {
      view = createViewGameover(game);
    } else {
      view = createView(game);
    }
    print(view, getmaxx(window), field_width(game));
    deleteView(view);
  }
}

int main(int argc, char *argv[]) {
  FILE *local_highscores = init_state_files();
  if (local_highscores == NULL)
    exit(EXIT_FAILURE);
  time(&fps_timestamp);
  setlocale(LC_CTYPE, "");
  WINDOW *window = create_window();
  GameInstance game = select_mode(&window);
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
    case PLAYING:
      break;
    case WON:
      print_game(game, window);
      Highscore highscore = generate_highscore(game);
      save_highscore(highscore, local_highscores);
    case LOST:
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
  game = select_mode(&window);
  if (game != NULL)
    goto start;
end:
  endwin();
  fclose(local_highscores);
  return 0;
}
