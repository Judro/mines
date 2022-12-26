#include "custom_game.h"
#include "menu.h"
#include <stdlib.h>

typedef struct CGMenu {
  unsigned width;
  unsigned height;
  unsigned mines;
  char cursor_position;
} CustomGameMenu;

void cm_print(CustomGameMenu *menu, unsigned terminal_width,
              unsigned terminal_height) {
  int lml = (terminal_width - 22) / 2 + 1;
  char *lm = malloc(lml * sizeof(char));
  for (int i = 0; i < lml; i++) {
    lm[i] = ' ';
  }
  lm[lml - 1] = '\0';
  for (int i = 0; i < (terminal_height - 10) / 2; i++) {
    printw("\n");
  }
  printw("%s┌──────────────────┐\n", lm);
  if (menu->cursor_position == 0) {
    printw("%s│  > width  =  %2d  │\n", lm, menu->width);
  } else {
    printw("%s│    width  =  %2d  │\n", lm, menu->width);
  }
  if (menu->cursor_position == 1) {
    printw("%s│  > height =  %2d  │\n", lm, menu->height);
  } else {
    printw("%s│    height =  %2d  │\n", lm, menu->height);
  }
  if (menu->cursor_position == 2) {
    printw("%s│  > mines  = %3d  │\n", lm, menu->mines);
  } else {
    printw("%s│    mines  = %3d  │\n", lm, menu->mines);
  }
  printw("%s└──────────────────┘\n", lm);
  free(lm);
}
char custom_game_control(CustomGameMenu *menu) {
  char ch = getch();
  switch (ch) {
  case 'j':
  case 0x42:
    if (menu->cursor_position + 1 < 3)
      menu->cursor_position += 1;
    break;
  case 'k':
  case 0x41:
    if (menu->cursor_position > 0)
      menu->cursor_position -= 1;
    break;
  case 0x43:
  case 'l':
    switch (menu->cursor_position) {
    case 0:
      menu->width++;
      break;
    case 1:
      menu->height++;
      break;
    case 2:
      menu->mines++;
      break;
    default:
      break;
    }
    break;
  case 0x44:
  case 'h':
    switch (menu->cursor_position) {
    case 0:
      menu->width--;
      if (menu->width > 10000)
        menu->width = 0;
      break;
    case 1:
      menu->height--;
      if (menu->height > 10000)
        menu->height = 0;
      break;
    case 2:
      menu->mines--;
      if (menu->mines > 100000)
        menu->mines = 0;
      break;
    default:
      break;
    }
    break;
  case '\n':
  case ' ':
  case 'f':
  case 's':
    return 1;
    break;
  case 'q':
    return -1;
  default:
    break;
  }
  return 0;
}
GameInstance get_custom_game(unsigned terminal_width,
                             unsigned terminal_height) {
  CustomGameMenu menu;
  menu.width = 40;
  menu.height = 10;
  menu.mines = 40;
  menu.cursor_position = 0;
  int ret = 0;
  while (!ret) {
    erase();
    cm_print(&menu, terminal_width, terminal_height);
    ret = custom_game_control(&menu);
    if (ret == -1)
      return select_mode(terminal_width, terminal_height);
  }
  return createGameInstance(menu.width, menu.height, menu.mines);
}
