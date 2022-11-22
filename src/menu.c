#include "menu.h"
#include <ncurses.h>
typedef struct Menu {
  unsigned int pos;
  char selected;
} Menu;

void m_print(Menu *menu) {
  printw(" +------------------+      ^\n");
  if (menu->pos == 0) {
    printw(" |   -[Beginner]-   |      k\n");
  } else {
    printw(" |     Beginner     |      k\n");
  }
  if (menu->pos == 1) {
    printw(" |     -[Easy]-     | < h     l >\n");
  } else {
    printw(" |       Easy       | < h     l >\n");
  }
  if (menu->pos == 2) {
    printw(" | -[Intermediate]- |      j\n");
  } else {
    printw(" |   Intermediate   |      j\n");
  }
  if (menu->pos == 3) {
    printw(" |    -[Expert]-    |      v\n");
  } else {
    printw(" |      Expert      |      v\n");
  }
  printw(" +------------------+\n");
  printw(" - place flag with <f>\n");
  printw(" - open tile with <s>\n");
  printw(" - quit with <q>\n");
}

char control(Menu *menu) {
  char ch = getch();
  switch (ch) {
  case 'j':
  case 0x42:
    if (menu->pos + 1 < 4)
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
  }
  return 0;
}
Game *select_mode() {
  Menu menu;
  menu.pos = 0;
  menu.selected = 0;
  for (;;) {
    clear();

    m_print(&menu);
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
      default:
      }
      return g_new(2, 2, 2);
    }
  }
  return g_new(2, 2, 2);
}
