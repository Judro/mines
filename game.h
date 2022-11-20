#ifndef GAME_H
#define GAME_H
typedef struct {
  int x;
  int y;
} Cord;

typedef struct myGame Game;
 typedef enum GPrint{
	UNTOUCHED,
	UNVEILED,
	FALSE_FLAG,
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

typedef struct GPrintable{
	Cord player;
	GPrint* fields;
	unsigned int width;
	unsigned int height;
} GPrintable;

GPrintable* g_printable(Game*);
GPrintable* g_printable_gameover(Game*);
void g_gprintable_kill(GPrintable *);
Game *g_new(int width, int height, int minesamount);
Cord g_player_position(Game *g);
void g_set_player_position_x(Game *g, int x);
void g_set_player_position_y(Game *g, int y);
int g_flags_total(Game *g);
int g_flags_found(Game *g);
int g_width(Game *g);
int g_height(Game *g);
void g_flag(Game *g);
int g_unveil(Game *g);
int checkflags(Game *g);
void g_kill(Game *g);

#endif
