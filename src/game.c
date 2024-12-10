#include "game.h"
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MINE 0b0010000
#define UNVLD 0b0100000
#define FLAGGED 0b1000000

typedef struct game {
  char *mines;
  int width;
  int height;
  int length;
  int flagstotal;
  int flagsfound;
  int unveiled;
  GameState state;
  time_t started;
  Cord cord;
} *GameInstance;

char is_mine(char c) {
  char m = MINE;
  if (c & m)
    return 1;
  return 0;
}

char is_flagged(char c) {
  char f = FLAGGED;
  if (c & f)
    return 1;
  return 0;
}

char is_blank(char c) {
  if (c & 0b1111)
    return 0;
  return 1;
}

char is_unveiled(char c) {
  char u = UNVLD;
  if (c & u)
    return 1;
  return 0;
}

GameInstance createGameInstance(int width, int height, int amount_mines) {
  GameInstance g = calloc(1, sizeof(struct game));
  int length = width * height;
  int total = 0;
  char *mines = malloc(length * sizeof(char));
  double minesp = (double)amount_mines / (double)length;
  srand48(time(0));
  for (int i = 0; i < length; i++) {
    mines[i] = 0;
    double rand = drand48();
    if (rand < minesp) {
      if (total == amount_mines)
        continue;
      mines[i] = MINE;
      total += 1;
    }
  }
  while (total < amount_mines) {
    minesp = (double)(amount_mines - total) / (double)length;
    for (int i = 0; i < length; i++) {
      double rand = drand48();
      if (rand < minesp && !is_mine(mines[i])) {
        if (total == amount_mines)
          continue;
        mines[i] = MINE;
        total += 1;
      }
    }
  }
  // compute amount of near mines
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (!is_mine(mines[i * width + j])) {
        int mcount = 0;
        for (int k = i - 1; k <= i + 1; k++) {
          for (int l = j - 1; l <= j + 1; l++) {
            if (k >= 0 && k < height && l >= 0 && l < width) {
              if (is_mine(mines[k * width + l])) {
                mcount++;
              }
            }
          }
        }
        mines[i * width + j] = mcount;
      }
    }
  }

  g->length = length;
  g->mines = mines;
  g->width = width;
  g->height = height;
  g->flagstotal = total;
  g->flagsfound = 0;
  g->unveiled = 0;
  g->state = PLAYING;
  Cord c;
  c.x = 0;
  c.y = 0;
  g->cord = c;
  time_t start;
  time(&start);
  g->started = start;
  return g;
}

void deleteGameInstance(GameInstance g) {
  free(g->mines);
  free(g);
}

GameView createView(GameInstance g) {
  GameView view = calloc(1, sizeof(struct game_view));
  CellType *cells = calloc(g->length, sizeof(enum cell_type));
  view->cells = cells;
  view->player = g->cord;
  time_t current;
  time(&current);
  view->mines = g->flagstotal - g->flagsfound;
  view->time = current - g->started;
  view->width = g->width;
  view->state = g->state;
  for (int i = 0; i < g->length; i++) {
    if (is_flagged(g->mines[i])) {
      view->cells[i] = FLAG;
    } else if (is_unveiled(g->mines[i])) {
      switch (g->mines[i] & 0b1111) {
      case 0:
        view->cells[i] = UNVEILED;
        break;
      case 1:
        view->cells[i] = ONE;
        break;
      case 2:
        view->cells[i] = TWO;
        break;
      case 3:
        view->cells[i] = THREE;
        break;
      case 4:
        view->cells[i] = FOUR;
        break;
      case 5:
        view->cells[i] = FIVE;
        break;
      case 6:
        view->cells[i] = SIX;
        break;
      case 7:
        view->cells[i] = SEVEN;
        break;
      case 8:
        view->cells[i] = EIGHT;
        break;
      default:
        break;
      }
    } else {
      view->cells[i] = UNTOUCHED;
    }
  }
  view->height = g->height;
  view->width = g->width;
  return view;
}
GameView createViewGameover(GameInstance g) {
  GameView view = calloc(1, sizeof(struct game_view));
  CellType *cells = calloc(g->length, sizeof(enum cell_type));
  view->cells = cells;
  view->player = g->cord;
  time_t current;
  time(&current);
  view->mines = g->flagstotal - g->flagsfound;
  view->time = current - g->started;
  view->width = g->width;
  view->state = g->state;
  for (int i = 0; i < g->length; i++) {
    if (is_flagged(g->mines[i])) {
      if (is_mine(g->mines[i])) {
        view->cells[i] = FLAG;
      } else {
        view->cells[i] = FALSE_FLAG;
      }
    } else {
      if (is_mine(g->mines[i])) {
        view->cells[i] = FLAG_NOT_FOUND;
        continue;
      }
      switch (g->mines[i] & 0b1111) {
      case 0:
        view->cells[i] = UNVEILED;
        break;
      case 1:
        view->cells[i] = ONE;
        break;
      case 2:
        view->cells[i] = TWO;
        break;
      case 3:
        view->cells[i] = THREE;
        break;
      case 4:
        view->cells[i] = FOUR;
        break;
      case 5:
        view->cells[i] = FIVE;
        break;
      case 6:
        view->cells[i] = SIX;
        break;
      case 7:
        view->cells[i] = SEVEN;
        break;
      case 8:
        view->cells[i] = EIGHT;
        break;
      default:
        break;
      }
    }
  }
  view->cells[g->cord.y * g->width + g->cord.x] = FALSE_FLAG;
  view->height = g->height;
  view->width = g->width;
  return view;
}

void deleteView(GameView view) {
  free(view->cells);
  free(view);
}

GameState game_state(GameInstance game) { return game->state; }
Cord player_position(GameInstance g) { return g->cord; }
void set_player_position_x(GameInstance g, int x) { g->cord.x = x; }
void set_player_position_y(GameInstance g, int y) { g->cord.y = y; }
int field_width(GameInstance g) { return g->width; }
int field_height(GameInstance g) { return g->height; }
unsigned total_mines(GameInstance g) { return g->flagstotal; }

void flag_cell(GameInstance g) {
  if (is_flagged(g->mines[g->cord.y * g->width + g->cord.x])) {
    g->mines[g->cord.y * g->width + g->cord.x] ^= FLAGGED;
    g->flagsfound -= 1;
  } else {
    if (is_unveiled(g->mines[g->cord.y * g->width + g->cord.x]) ||
        g->flagsfound == g->flagstotal)
      return;
    g->mines[g->cord.y * g->width + g->cord.x] |= FLAGGED;
    g->flagsfound += 1;
  }
}

void unveil_recursive(GameInstance game, Cord position) {
  if (is_unveiled(game->mines[position.y * game->width + position.x]))
    return;
  game->mines[position.y * game->width + position.x] |= UNVLD;
  game->unveiled++;
  if (game->unveiled + game->flagstotal == game->width * game->height)
    game->state = WON;
  if (!is_blank(game->mines[position.y * game->width + position.x])) {
    return;
  }
  for (int y = position.y - 1; y <= position.y + 1; y++) {
    for (int x = position.x - 1; x <= position.x + 1; x++) {
      if (x < 0 || y < 0 || x >= game->width || y >= game->height)
        continue;
      Cord cord;
      cord.x = x;
      cord.y = y;
      unveil_recursive(game, cord);
    }
  }
}

void unveil_cell(GameInstance g) {
  if (is_flagged(g->mines[g->cord.y * g->width + g->cord.x])) {
    return;
  }
  if (is_mine(g->mines[g->cord.y * g->width + g->cord.x])) {
    g->state = LOST;
    return;
  }
  if (g->mines[g->cord.y * g->width + g->cord.x] > 0) {
    g->mines[g->cord.y * g->width + g->cord.x] |= UNVLD;
    g->unveiled++;
    if (g->unveiled + g->flagstotal == g->width * g->height)
      g->state = WON;
    return;
  }
  unveil_recursive(g, g->cord);
}
void validate_flags(GameInstance g) {
  int fit = 0;
  for (int i = 0; i < g->length; i++) {
    if (is_mine(g->mines[i]) && is_flagged(g->mines[i]))
      fit += 1;
  }
  if (fit == g->flagstotal)
    g->state = WON;
}

Highscore generate_highscore(GameInstance g) {
  time_t current;
  time(&current);
  if (g->state != WON)
    exit(EXIT_FAILURE);
  Highscore h;
  h.width = g->width;
  h.height = g->height;
  h.mines = g->flagstotal;
  h.time = current - g->started;
  h.date = current;
  return h;
}
