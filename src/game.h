#ifndef GAME_H
#define GAME_H
#include "highscore.h"
#include <time.h>
typedef struct {
  int x;
  int y;
} Cord;

typedef struct Game *GameInstance;

typedef enum GameState {
  Playing,
  Won,
  Lost,
} GameState;

typedef enum CellType {
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

typedef struct GameView {
  CellType *cells;
  Cord player;
  unsigned width;
  unsigned height;
  GameState state;
  unsigned mines;
  time_t time;
} *GameView;

GameState game_state(GameInstance);
GameView createView(GameInstance);
GameView createViewGameover(GameInstance);
void deleteView(GameView);
GameInstance createGameInstance(int, int, int);
Cord player_position(GameInstance);
void set_player_position_x(GameInstance, int);
void set_player_position_y(GameInstance, int);
int field_width(GameInstance);
int field_height(GameInstance);
void flag_cell(GameInstance);
void unveil_cell(GameInstance);
void validate_flags(GameInstance);
void deleteGameInstance(GameInstance);
Highscore generate_highscore(GameInstance);
#endif
