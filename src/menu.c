#include "menu.h"
#include "custom_game.h"
#include <ncurses.h>
#include <stdlib.h>
typedef struct Menu {
  unsigned int pos;
  char selected;
} Menu;

unsigned int last_selected = 0;

void m_print(Menu *menu, unsigned int terminal_width,
             unsigned int terminal_height) {
  int lml = (terminal_width - 20) / 2 + 1;
  char *lm = malloc(lml * sizeof(char));
  for (int i = 0; i < lml; i++) {
    lm[i] = ' ';
  }
  lm[lml - 1] = '\0';
  for (int i = 0; i < (terminal_height - 10) / 2; i++) {
    printw("\n");
  }
  printw("%s┌──────────────────┐\n", lm);
  if (menu->pos == 0) {
    printw("%s│   > Beginner <   │\n", lm);
  } else {
    printw("%s│     Beginner     │\n", lm);
  }
  if (menu->pos == 1) {
    printw("%s│     > Easy <     │\n", lm);
  } else {
    printw("%s│       Easy       │\n", lm);
  }
  if (menu->pos == 2) {
    printw("%s│ > Intermediate < │\n", lm);
  } else {
    printw("%s│   Intermediate   │\n", lm);
  }
  if (menu->pos == 3) {
    printw("%s│    > Expert <    │\n", lm);
  } else {
    printw("%s│      Expert      │\n", lm);
  }
  if (menu->pos == 4) {
    printw("%s│    > Custom <    │\n", lm);
  } else {
    printw("%s│      Custom      │\n", lm);
  }
  if (menu->pos == 5) {
    printw("%s│     > Help <     │\n", lm);
  } else {
    printw("%s│       Help       │\n", lm);
  }
  printw("%s└──────────────────┘\n", lm);
  free(lm);
}

void print_help() {
  erase();
  printw("- Movement:                 <hjkl>\n");
  printw("                            <←↓↑→>>\n");
  printw("- Place flag:               <f>\n");
  printw("- Open tile:                <s>\n");
  printw("- Quit:                     <q>\n");
  printw("- Emphazise important cells: <e>\n");
  for (;;) {
    char ch = getch();
    if (ch == 'q')
      return;
  }
}

char control(Menu *menu) {
  char ch = getch();
  switch (ch) {
  case 'j':
  case 0x42:
    if (menu->pos + 1 < 6)
      menu->pos += 1;
    break;
  case 'k':
  case 0x41:
    if (menu->pos > 0)
      menu->pos -= 1;
    break;
  case '\n':
  case ' ':
  case '\t':
  case 'f':
  case 's':
    menu->selected = 1;
    break;
  case 'q':
    return -1;
  default:
    break;
  }
  return 0;
}
GameInstance select_mode(unsigned int terminal_width,
                         unsigned int terminal_height) {
  Menu menu;
  menu.pos = last_selected;
  menu.selected = 0;
  for (;;) {
    erase();

    m_print(&menu, terminal_width, terminal_height);
    char ret = control(&menu);
    if (ret == -1)
      return NULL;
    if (menu.selected == 1) {
			last_selected = menu.pos;
      switch (menu.pos) {
      case 0:
        return createGameInstance(20, 5, 7);
      case 1:
        return createGameInstance(18, 7, 15);
      case 2:
        return createGameInstance(40, 10, 46);
      case 3:
        return createGameInstance(33, 15, 105);
      case 4:
        return get_custom_game(terminal_width, terminal_height);
      case 5:
        print_help();
        return select_mode(terminal_width, terminal_height);
      default:
        break;
      }
      return createGameInstance(2, 2, 2);
    }
  }
  return createGameInstance(2, 2, 2);
}
