#include "highscore.h"
#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void init_state_files(){
	DIR* save_dir = opendir("/var/games/mines/save");
	if (save_dir) {
		// check if USER.asc
	} else if (ENOENT == errno) {
		fprintf(stderr, "Can't load save file");
		exit(EXIT_FAILURE);
		// create directory
	} else {
		// panic other reasons failed
	}
}
