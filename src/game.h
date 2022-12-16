#ifndef GAME_H
#define GAME_H
#include <time.h>
typedef struct {
  int x;
  int y;
} Cord;

typedef struct Game *GameInstance;
typedef enum GPrint {
  UNTOUCHED,
  UNVEILED,
  FALSE_FLAG,
  FLAG_NOT_FOUND,
  FLAG,
  ONE,
  TWO,
  THREE,
  FOUR,
  FIVE,
  SIX,
  SEVEN,
  EIGHT,
} GPrint;

typedef struct GPrintable {
  Cord player;
  GPrint *fields;
  unsigned int width;
  unsigned int height;
} *PrintableInstance;

typedef struct GPrintableH {
  unsigned int mines;
  time_t time;
  unsigned int width;
} *PrintableHeaderInstance;

PrintableInstance createPrintable(GameInstance);
PrintableInstance createPrintableGameover(GameInstance);
PrintableHeaderInstance createPrintableHeader(GameInstance);
void deletePrintable(PrintableInstance);
GameInstance createGameInstance(int, int, int);
Cord g_player_position(GameInstance);
void g_set_player_position_x(GameInstance, int);
void g_set_player_position_y(GameInstance, int);
int g_flags_total(GameInstance);
int g_flags_found(GameInstance);
int g_width(GameInstance);
int g_height(GameInstance);
time_t g_start(GameInstance);
void g_flag(GameInstance);
int g_unveil(GameInstance);
int checkflags(GameInstance);
void deleteGameInstance(GameInstance);

#endif
