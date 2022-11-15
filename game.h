typedef struct {
  int x;
  int y;
} Cord;

typedef struct myGame Game;
Game *g_new(int width, int height, int minesamount);
char g_mine(Game *g, int x, int y);
char g_plain(Game *g, int x, int y);
int g_amount_near_mines(Game *g, int x, int y);
Cord g_player_position(Game *g);
void g_set_player_position_x(Game *g, int x);
void g_set_player_position_y(Game *g, int y);
char g_unveiled(Game *g, int x, int y);
char g_flaged(Game *g, int x, int y);
int g_flags_total(Game *g);
int g_flags_found(Game *g);
int g_width(Game *g);
int g_height(Game *g);
void g_flag(Game *g);
int g_unveil(Game *g);
int checkflags(Game *g);
void g_kill(Game *g);
