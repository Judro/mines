typedef struct{
  int x;
  int y;
}Cord;

typedef struct myGame Game;
Game* g_new(int width, int height, int  minesamount );
char is_mine(Game* g,int x, int y);
char is_plain(Game* g, int x, int y);
int amount_near_mines(Game* g,int x, int y);
Cord get_player_position(Game* g);
void set_player_position_x(Game* g,int x);
void set_player_position_y(Game* g,int y);
char is_unveiled(Game* g, int x,int y);
char is_flaged(Game* g, int x, int y);
int g_flags_total(Game *g);
int g_flags_found(Game *g);
int g_width(Game *g);
int g_height(Game *g);
void flag(Game *g);
int  test(Game *g);
int checkflags(Game *g);
void g_kill(Game *g);
