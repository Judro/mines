#ifndef GAME_H
#define GAME_H
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


typedef struct GameView {
	CellType *cells;
	Cord player;
	unsigned width;
	unsigned height;
	GameState state;
	unsigned mines;
	time_t time;
} *GameView;

GameState g_state(GameInstance);
GameView createView(GameInstance);
GameView createViewGameover(GameInstance);
void deleteView(GameView);
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
