#include "highscore.h"
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

char print_highscore_flag = 0;

FILE *init_state_files() {
  char username_n[100];
  char *username = getenv("USER");
  char is_root = 0;
  if (strcmp(username, "root") == 0) {
    printf("Please enter your username ($USER): ");
    while (scanf("%s[^\n]", username_n) != 1)
      ;
    username = username_n;
    is_root = 1;
  }
  char save_path[strlen(save_directory) + strlen(username) + 5];
  strcpy(save_path, save_directory);
  strcpy(save_path + strlen(save_directory), username);
  strcpy(save_path + strlen(username) + strlen(save_directory), ".asc");

  if (access(save_path, F_OK) != 0 || access(save_path, W_OK) != 0) {
    int ret = system("mkdir -p /var/games/mines/save");
    if (ret != 0) {
      fprintf(stderr, "Error while creating directory.");
      exit(EXIT_FAILURE);
    }

    struct passwd *pw = getpwnam(username);
    if (!pw) {
      fprintf(stderr, "Failed to get user info. The user %s may not exist.\n",
              username);
      exit(EXIT_FAILURE);
    }

    int file = open(save_path, O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (file == -1) {
      fprintf(stderr, "Unable to create %s\n", save_path);
      fprintf(stderr, "Please check if your user has write access to the file "
                      "and try again.\n");
      exit(EXIT_FAILURE);
    }
    if (chown(save_path, pw->pw_uid, -1) == -1) {
      perror("Failed to change file owner");
      exit(EXIT_FAILURE);
    }
    close(file);
    if (is_root) {
      printf(
          "Game save files have been successfully created. Please restart the "
          "game in non-superuser mode.\n");

      exit(EXIT_SUCCESS);
    }
  }
  return fopen(save_path, "a+");
}

static int _sort_user_highscore(const void *a, const void *b) {
  return (long)((struct user_highscore *)a)->highscore.time -
         (long)((struct user_highscore *)b)->highscore.time;
}

UserHighscore *load_highscores(struct highscore cmp) {
  DIR *save_dir = opendir(save_directory);
  unsigned hs_counter = 0;
  struct highscore h;
  unsigned hs_capacity = 250;
  UserHighscore *highscores =
      malloc(hs_capacity * sizeof(struct user_highscore));
  if (highscores == NULL)
    exit(EXIT_FAILURE);
  struct dirent *dir_ent;
  while ((dir_ent = readdir(save_dir)) != NULL) {
    if (save_dir == NULL)
      break;
    if (dir_ent->d_type == DT_REG) {
      char highscore_file[strlen(save_directory) + strlen(dir_ent->d_name) + 1];
      strcpy(highscore_file, save_directory);
      strcpy(highscore_file + strlen(save_directory), dir_ent->d_name);
      highscore_file[strlen(save_directory) + strlen(dir_ent->d_name)] = 0;
      FILE *high_score = fopen(highscore_file, "r");
      if (high_score == NULL)
        exit(EXIT_FAILURE);
      while (fscanf(high_score, "%u,%u,%u,%u,%ld%*c", &h.width, &h.height,
                    &h.mines, &h.time, &h.date) != EOF) {
        if (h.width != cmp.width || h.height != cmp.height ||
            h.mines != cmp.mines)
          continue;
        highscores[hs_counter].user = malloc(strlen(dir_ent->d_name) + 1);
        if (highscores[hs_counter].user == NULL)
          exit(EXIT_FAILURE);
        strncpy(highscores[hs_counter].user, dir_ent->d_name,
                strlen(dir_ent->d_name) - 3);
        highscores[hs_counter].user[strlen(dir_ent->d_name) - 4] = 0;
        highscores[hs_counter].highscore = h;
        hs_counter++;

        if (hs_counter >= hs_capacity) {
          hs_capacity *= 2;
          UserHighscore *new_hs =
              realloc(highscores, hs_capacity * sizeof(struct user_highscore));
          if (new_hs == NULL)
            exit(EXIT_FAILURE);
          highscores = new_hs;
        }
      }
      fclose(high_score);
    }
  }
  struct user_highscore u;
  u.user = NULL;
  highscores[hs_counter] = u;
  closedir(save_dir);
  if (hs_counter == 0) {
    free(highscores);
    return NULL;
  }
  qsort(highscores, hs_counter, sizeof(struct user_highscore),
        _sort_user_highscore);
  return highscores;
}

char **userHighscores2string(UserHighscore *highscores) {
  char **highscores_str = malloc(highscore_capacity * sizeof(char *));
  if (highscores_str == NULL)
    exit(EXIT_FAILURE);

  if (highscores == NULL) {
    char *error_message = "       No highscores yet.      ";
    char *error_message2 = " Be the first to set a record! ";
    highscores_str[0] = malloc(strlen(error_message) + 1);
    highscores_str[1] = malloc(strlen(error_message2) + 1);
    if (highscores_str[0] == NULL)
      exit(EXIT_FAILURE);
    if (highscores_str[1] == NULL)
      exit(EXIT_FAILURE);

    strcpy(highscores_str[0], error_message);
    strcpy(highscores_str[1], error_message2);

    highscores_str[2] = NULL;
    return highscores_str;
  }

  unsigned index = 0;
  while (highscores[index].user != NULL && index < highscore_capacity) {
    char *tmp;
    char time[32];
    struct tm *local_time = localtime(&highscores[index].highscore.date);
    strftime(time, 32, "%Y-%m-%d", local_time);
    if (asprintf(&tmp, " %02u:%02u  %s  %s ",
                 highscores[index].highscore.time / 60,
                 highscores[index].highscore.time % 60, time,
                 highscores[index].user) == -1)
      exit(EXIT_FAILURE);

    if (tmp == NULL)
      exit(EXIT_FAILURE);
    free(highscores[index].user);
    highscores_str[index] = tmp;
    index++;
  }
  free(highscores);
  if (index < highscore_capacity)
    highscores_str[index] = NULL;
  return highscores_str;
}

int save_highscore(Highscore h, FILE *f) {
  fprintf(f, "%u,%u,%u,%u,%ld\n", h.width, h.height, h.mines, h.time, h.date);
  return 0;
}
