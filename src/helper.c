#include "helper.h"
#include <stdlib.h>
#include <string.h>
void clear_char_buff(char **buff, unsigned max_n) {
  if (buff == NULL)
    return;
  char **tmp = buff;
  unsigned i = 0;
  while (*tmp != NULL && i < max_n) {
    free(*tmp);
    tmp++;
    i++;
  }
  free(buff);
}

struct dimension buff_max_dimensions(char **buff, unsigned max_n) {
  unsigned i = 0;
  int max = 0;
  while (*buff != NULL && i < max_n) {
    max = strlen(*buff) > max ? strlen(*buff) : max;
    buff++;
    i++;
  }
  struct dimension max_d;
  max_d.len = i;
  max_d.width = max;
  return max_d;
}
