#ifndef HELPER_H
#define HELPER_H
struct dimension {
  unsigned width;
  unsigned len;
};
void clear_char_buff(char **, unsigned);
struct dimension buff_max_dimensions(char **buff, unsigned);
#endif
