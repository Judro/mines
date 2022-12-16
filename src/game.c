#include "game.h"
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
typedef struct Game {
  char *mines;
  char *select;
  char *floating;
  int width;
  int height;
  int length;
  int flagstotal;
  int flagsfound;
  int flagsfit;
  time_t started;
  Cord cord;
} *GameInstance;

GameInstance createGameInstance(int width, int height, int minesamount) {
  GameInstance g = calloc(1, sizeof(struct Game));
  int length = width * height;
  int total = 0;
  char *mines = malloc(length * sizeof(char));
  char *select = malloc(length * sizeof(char));
  char *floating = malloc(length * sizeof(char));
  double minesp = (double)minesamount / (double)length;
  srand48(time(0));
  for (int i = 0; i < length; i++) {
    mines[i] = 0;
    select[i] = 0;
    floating[i] = 0;
    double rand = drand48();
    if (rand < minesp) {
      if (total == minesamount)
        continue;
      mines[i] = -1;
      total += 1;
    }
  }
  while (total < minesamount) {
    for (int i = 0; i < length; i++) {
      double rand = drand48();
      if (rand < minesp && mines[i] != -1) {
        if (total == minesamount)
          continue;
        mines[i] = -1;
        total += 1;
      }
    }
  }
  // compute amount of near mines
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (mines[i * width + j] != -1) {
        int mcount = 0;
        for (int k = i - 1; k <= i + 1; k++) {
          for (int l = j - 1; l <= j + 1; l++) {
            if (k >= 0 && k < height && l >= 0 && l < width) {
              if (mines[k * width + l] == -1) {
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
  g->floating = floating;
  g->select = select;
  g->width = width;
  g->height = height;
  g->flagstotal = total;
  g->flagsfound = 0;
  g->flagsfit = 0;
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
  free(g->select);
  free(g->floating);
  free(g);
}

PrintableInstance createPrintable(GameInstance g) {
  PrintableInstance gp = calloc(1, sizeof(struct GPrintable));
  GPrint *gpr = calloc(g->length, sizeof(GPrint));
  gp->fields = gpr;
  gp->player = g->cord;
  for (int i = 0; i < g->length; i++) {
    if (g->select[i] == 2) {
      gp->fields[i] = FLAG;
    } else if (g->select[i] == 1) {
      switch (g->mines[i]) {
      case 0:
        gp->fields[i] = UNVEILED;
        break;
      case 1:
        gp->fields[i] = ONE;
        break;
      case 2:
        gp->fields[i] = TWO;
        break;
      case 3:
        gp->fields[i] = THREE;
        break;
      case 4:
        gp->fields[i] = FOUR;
        break;
      case 5:
        gp->fields[i] = FIVE;
        break;
      case 6:
        gp->fields[i] = SIX;
        break;
      case 7:
        gp->fields[i] = SEVEN;
        break;
      case 8:
        gp->fields[i] = EIGHT;
        break;
      default:
        break;
      }
    } else {
      gp->fields[i] = UNTOUCHED;
    }
  }
  gp->height = g->height;
  gp->width = g->width;
  return gp;
}
PrintableInstance createPrintableGameover(GameInstance g) {
  PrintableInstance gp = calloc(1, sizeof(struct GPrintable));
  GPrint *gpr = calloc(g->length, sizeof(GPrint));
  gp->fields = gpr;
  gp->player = g->cord;
  for (int i = 0; i < g->length; i++) {
    if (g->select[i] == 2) {
      if (g->mines[i] == -1) {
        gp->fields[i] = FLAG;
      } else {
        gp->fields[i] = FALSE_FLAG;
      }
    } else {
      switch (g->mines[i]) {
      case -1:
        gp->fields[i] = FLAG_NOT_FOUND;
        break;
      case 0:
        gp->fields[i] = UNVEILED;
        break;
      case 1:
        gp->fields[i] = ONE;
        break;
      case 2:
        gp->fields[i] = TWO;
        break;
      case 3:
        gp->fields[i] = THREE;
        break;
      case 4:
        gp->fields[i] = FOUR;
        break;
      case 5:
        gp->fields[i] = FIVE;
        break;
      case 6:
        gp->fields[i] = SIX;
        break;
      case 7:
        gp->fields[i] = SEVEN;
        break;
      case 8:
        gp->fields[i] = EIGHT;
        break;
      default:
        break;
      }
    }
  }
  gp->fields[g->cord.y * g->width + g->cord.x] = FALSE_FLAG;
  gp->height = g->height;
  gp->width = g->width;
  return gp;
}

PrintableHeaderInstance createPrintableHeader(GameInstance g) {
  PrintableHeaderInstance gp = calloc(1, sizeof(struct GPrintableH));
  time_t current;
  time(&current);
  gp->mines = g->flagstotal - g->flagsfound;
  gp->time = current - g->started;
  gp->width = g->width;
  return gp;
}

void deletePrintable(PrintableInstance gp) {
  free(gp->fields);
  free(gp);
}

Cord g_player_position(GameInstance g) { return g->cord; }
void g_set_player_position_x(GameInstance g, int x) { g->cord.x = x; }
void g_set_player_position_y(GameInstance g, int y) { g->cord.y = y; }
int g_flags_total(GameInstance g) { return g->flagstotal; }
int g_flags_found(GameInstance g) { return g->flagsfound; }
int g_width(GameInstance g) { return g->width; }
int g_height(GameInstance g) { return g->height; }
time_t g_start(GameInstance g) { return g->started; }

void g_flag(GameInstance g) {
  if (g->select[g->cord.y * g->width + g->cord.x] == 2) {
    g->select[g->cord.y * g->width + g->cord.x] = 0;
    g->flagsfound -= 1;
  } else {
    g->select[g->cord.y * g->width + g->cord.x] = 2;
    g->flagsfound += 1;
  }
}

int unveil_iteration(GameInstance g, int x, int y, int iteration) {
  if (g->mines[y * g->width + x] >= 0) {
    if (g->select[y * g->width + x] != 2)
      g->select[y * g->width + x] = 1;
    if (g->mines[y * g->width + x] == 0) {
      if (g->floating[y * g->width + x] == 0) {
        g->floating[y * g->width + x] = iteration + 1;
        return 1;
      }
    }
  }
  return 0;
}

int floating_unveil(GameInstance g, int iteration) {
  int amoun_unveiled = 0;
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (g->floating[i * g->width + j] == iteration) {
        for (int k = i - 1; k <= i + 1; k++) {
          for (int l = j - 1; l <= j + 1; l++) {
            if (l >= 0 && k >= 0 && k < g->height && l < g->width) {
              amoun_unveiled += unveil_iteration(g, l, k, iteration);
            }
          }
        }
      }
    }
  }
  return amoun_unveiled;
}

int g_unveil(GameInstance g) {
  if (g->select[g->cord.y * g->width + g->cord.x] == 2) {
    return 0;
  }
  if (g->mines[g->cord.y * g->width + g->cord.x] == -1) {
    return -1;
  }
  if (g->mines[g->cord.y * g->width + g->cord.x] > 0) {
    g->select[g->cord.y * g->width + g->cord.x] = 1;
    return 0;
  }
  g->floating[g->cord.y * g->width + g->cord.x] = 1;
  int ui = 1;
  int tmp = floating_unveil(g, ui);
  while (tmp > 0) {
    tmp = 0;
    ui++;
    tmp = floating_unveil(g, ui);
  }
  for (int i = 0; i < g->length; i++) {
    g->floating[i] = 0;
  }

  return 0;
}
int checkflags(GameInstance g) {
  int fit = 0;
  for (int i = 0; i < g->length; i++) {
    if (g->mines[i] == -1 && g->select[i] == 2)
      fit += 1;
  }
  if (fit == g->flagstotal)
    return 1;
  return 0;
}
