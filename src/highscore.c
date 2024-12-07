#include "highscore.h"
#include <dirent.h>
#include <fcntl.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

FILE *init_state_files() {
  char username_n[100];
  char *username = getenv("USER");
  if (strcmp(username, "root") == 0) {
    printf("Please enter your username ($USER): ");
    scanf("%s[^\n]", username_n);
    username = username_n;
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
      perror("Failed to get user info");
      exit(EXIT_FAILURE);
    }

    int file = open(save_path, O_CREAT);
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
    if (chmod(save_path, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1) {
      perror("Failed to change file permission");
      exit(EXIT_FAILURE);
    }
    printf("Game save files have been successfully created. Please restart the "
           "game in non-superuser mode.\n");

    exit(EXIT_SUCCESS);
  }
  return fopen(save_path, "a+");
}

UserHighscore *load_highscores() {
  DIR *save_dir = opendir(save_directory);
  while (save_dir != NULL) {
    struct dirent *dir_ent = readdir(save_dir);
    if (dir_ent->d_type == DT_REG) {
      char highscore_file[strlen(save_directory) + dir_ent->d_namlen + 1];
      strcpy(highscore_file, save_directory);
      strcpy(highscore_file + strlen(save_directory), dir_ent->d_name);
      highscore_file[strlen(save_directory) + dir_ent->d_namlen] = 0;
      FILE *high_score = fopen(highscore_file, "r");
      unsigned counter = 0;
      struct highscore h;
      while (fscanf(high_score, "%u,%u,%u,%u,%ld%*c", &h.width, &h.height,
                    &h.mines, &h.time, &h.date) != EOF) {
        // push to array enlarge if necceserry
      }
      exit(EXIT_SUCCESS);
    }
  }
}

int save_highscore(Highscore h, FILE *f) {
  fprintf(f, "%u,%u,%u,%u,%ld\n", h.width, h.height, h.mines, h.time, h.date);
  return 0;
}
