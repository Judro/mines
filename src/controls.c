#include "game.h"
#include <ncurses.h>
int cmove(Game *g, WINDOW *window) {
  nodelay(window, 1);
  char ch = getch();
  Cord old = g_player_position(g);
  switch (ch) {
  case 'j':
  // vt100 sequence 0x1B 0x5B 0x42
  case 0x42:
    g_set_player_position_y(g, old.y + 1);
    if (g_player_position(g).y == g_height(g))
      g_set_player_position_y(g, 0);
    break;
  // vt100 sequence 0x1B 0x5B 0x41
  case 0x41:
  case 'k':
    g_set_player_position_y(g, old.y - 1);
    if (g_player_position(g).y == -1)
      g_set_player_position_y(g, g_height(g) - 1);
    break;
  // vt100 sequence 0x1B 0x5B 0x44
  case 0x44:
  case 'h':
    g_set_player_position_x(g, old.x - 1);
    if (g_player_position(g).x == -1)
      g_set_player_position_x(g, g_width(g) - 1);
    break;
  // vt100 sequence 0x1B 0x5B 0x43
  case 0x43:
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
