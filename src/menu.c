#include "menu.h"
#include <ncurses.h>
#include <stdlib.h>
typedef struct Menu {
  unsigned int pos;
  char selected;
} Menu;

void m_print(Menu *menu, unsigned int terminal_width,
             unsigned int terminal_height) {
  int lml = (terminal_width - 22) / 2 + 1;
  char *lm = malloc(lml * sizeof(char));
  for (int i = 0; i < lml; i++) {
    lm[i] = ' ';
  }
  lm[lml] = '\0';
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
    printw("%s│     > Help <     │\n", lm);
  } else {
    printw("%s│       Help       │\n", lm);
  }
  printw("%s└──────────────────┘\n", lm);
}

void print_help() {
  erase();
  printw("- Movement:   <hjkl> <←↓↑→>\n");
  printw("- Place flag: <f>\n");
  printw("- Open tile:  <s>\n");
  printw("- Quit:       <q>\n");
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
    if (menu->pos + 1 < 5)
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
Game *select_mode(unsigned int terminal_width, unsigned int terminal_height) {
  Menu menu;
  menu.pos = 0;
  menu.selected = 0;
  for (;;) {
    erase();

    m_print(&menu, terminal_width, terminal_height);
    char ret = control(&menu);
    if (ret == -1)
      return NULL;
    if (menu.selected == 1) {
      switch (menu.pos) {
      case 0:
        return g_new(20, 5, 7);
      case 1:
        return g_new(18, 7, 15);
      case 2:
        return g_new(40, 10, 40);
      case 3:
        return g_new(33, 15, 99);
      case 4:
        print_help();
        return select_mode(terminal_width, terminal_height);
      default:
        break;
      }
      return g_new(2, 2, 2);
    }
  }
  return g_new(2, 2, 2);
}
