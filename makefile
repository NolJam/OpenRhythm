game: main.c beat.c track.c level.c menu.c score.c
	gcc main.c beat.c level.c track.c menu.c score.c -lSDL2 -lSDL2_mixer -lSDL2_image -Wall -g -o game
