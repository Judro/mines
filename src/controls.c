#include "display.h"
#include "game.h"
#include <ncurses.h>
#include <stdlib.h>
int cmove(GameInstance g, WINDOW **window) {
  extern char g_helper_mode;
  extern char print_highscore_flag;
  nodelay(*window, 1);
  char ch = getch();
  Cord old = player_position(g);
  switch (ch) {
  case 'j':
  // vt100 sequence 0x1B 0x5B 0x42
  case 0x42:
    set_player_position_y(g, old.y + 1);
    if (player_position(g).y == field_height(g))
      set_player_position_y(g, 0);
    break;
  // vt100 sequence 0x1B 0x5B 0x41
  case 0x41:
  case 'k':
    set_player_position_y(g, old.y - 1);
    if (player_position(g).y == -1)
      set_player_position_y(g, field_height(g) - 1);
    break;
  // vt100 sequence 0x1B 0x5B 0x44
  case 0x44:
  case 'h':
    set_player_position_x(g, old.x - 1);
    if (player_position(g).x == -1)
      set_player_position_x(g, field_width(g) - 1);
    break;
  // vt100 sequence 0x1B 0x5B 0x43
  case 0x43:
  case 'l':
    set_player_position_x(g, old.x + 1);
    if (player_position(g).x == field_width(g))
      set_player_position_x(g, 0);
    break;
  case 'q':
    return -1;
  case 'f':
    flag_cell(g);
    break;
  case 's':
    unveil_cell(g);
    break;
  case 'e':
    g_helper_mode ^= 1;
    break;
  case 'b':
    print_highscore_flag ^= 1;
    break;
  case 'm':
    endwin();
    system("man mines-tui");
    *window = create_window();
    break;
  }
  return 0;
}
