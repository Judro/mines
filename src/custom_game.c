#include "custom_game.h"

typedef struct CGMenu {
  unsigned width;
  unsigned height;
  unsigned mines;
  char cursor_position;
} CustomGameMenu;

void cm_primt(CustomGameMenu *menu, unsigned terminal_width,
              unsigned terminal_height) {}
