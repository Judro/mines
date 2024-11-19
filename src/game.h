#ifndef GAME_H
#define GAME_H
#include <time.h>
typedef struct {
  int x;
  int y;
} Cord;

typedef struct Game *GameInstance;

typedef enum GState {
  Playing,
  Won,
  Lost,
} GState;

//GPrint
typedef enum CellType  {
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
} CellType;

typedef struct GPrintable {
  Cord player;
  CellType *cells;
  unsigned int width;
  unsigned int height;
} *PrintableInstance;

typedef struct GPrintableH {
  GState state;
  unsigned int mines;
  time_t time;
  unsigned int width;
} *PrintableHeaderInstance;

GState g_state(GameInstance);
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
void g_unveil(GameInstance);
void g_checkflags(GameInstance);
void deleteGameInstance(GameInstance);

#endif
