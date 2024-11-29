#include "custom_game.h"
#include "menu.h"
#include <stdlib.h>

typedef struct CGMenu {
  unsigned width;
  unsigned height;
  unsigned mines;
  char cursor_position;
} CustomGameMenu;

CustomGameMenu custom_game_menu = {40, 10, 40, 0};

void cm_print(unsigned terminal_width, unsigned terminal_height) {
  int lml = (terminal_width - 21) / 2 + 1;
  char *lm = malloc(lml * sizeof(char));
  for (int i = 0; i < lml; i++) {
    lm[i] = ' ';
  }
  lm[lml - 1] = '\0';
  for (int i = 0; i < (terminal_height - 10) / 2; i++) {
    printw("\n");
  }
  printw("%s╭───────────────────╮\n", lm);
  if (custom_game_menu.cursor_position == 0) {
    printw("%s│  width  = <%3d >  │\n", lm, custom_game_menu.width);
  } else {
    printw("%s│  width  =  %3d    │\n", lm, custom_game_menu.width);
  }
  if (custom_game_menu.cursor_position == 1) {
    printw("%s│  height = <%3d >  │\n", lm, custom_game_menu.height);
  } else {
    printw("%s│  height =  %3d    │\n", lm, custom_game_menu.height);
  }
  if (custom_game_menu.cursor_position == 2) {
    printw("%s│  mines  = <%3d >  │\n", lm, custom_game_menu.mines);
  } else {
    printw("%s│  mines  =  %3d    │\n", lm, custom_game_menu.mines);
  }
  printw("%s╰───────────────────╯\n", lm);
  free(lm);
}
char custom_game_control() {
  char ch = getch();
  switch (ch) {
  case 'j':
  case 0x42:
    if (custom_game_menu.cursor_position + 1 < 3)
      custom_game_menu.cursor_position += 1;
    break;
  case 'k':
  case 0x41:
    if (custom_game_menu.cursor_position > 0)
      custom_game_menu.cursor_position -= 1;
    break;
  case 0x43:
  case 'l':
    switch (custom_game_menu.cursor_position) {
    case 0:
      custom_game_menu.width++;
      break;
    case 1:
      custom_game_menu.height++;
      break;
    case 2:
      custom_game_menu.mines++;
      break;
    default:
      break;
    }
    break;
  case 0x44:
  case 'h':
    switch (custom_game_menu.cursor_position) {
    case 0:
      custom_game_menu.width--;
      if (custom_game_menu.width > 10000 || custom_game_menu.width < 12)
        custom_game_menu.width = 12;
      break;
    case 1:
      custom_game_menu.height--;
      if (custom_game_menu.height > 10000 || custom_game_menu.height < 4)
        custom_game_menu.height = 4;
      break;
    case 2:
      custom_game_menu.mines--;
      if (custom_game_menu.mines > 100000)
        custom_game_menu.mines = 0;
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
  int ret = 0;
  while (!ret) {
    erase();
    cm_print(terminal_width, terminal_height);
    ret = custom_game_control();
    if (ret == -1)
      return select_mode(terminal_width, terminal_height);
  }
  return createGameInstance(custom_game_menu.width, custom_game_menu.height,
                            custom_game_menu.mines);
}
