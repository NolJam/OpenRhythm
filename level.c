#include "level.h"

void level_load(Level* lvl, char* file_name)
{
	FILE* file = fopen(file_name, "r");
	if (!file) 
	{
		printf("file could not be opened.\n\n");
	}

	char line[100];
	int n = 0;
	int beat_block = 10;

	Beat* ptr = realloc(lvl->beats, (size_t)(beat_block) * sizeof(Beat)); 
	if (ptr == NULL) exit(1);
	lvl->beats = ptr;

	while (fgets(line, sizeof(line), file)) 
	{
		if (line[0] == 'b')
		{
			if (n == beat_block)
			{
				beat_block += 10;
				Beat* ptr = realloc(lvl->beats, (size_t)(beat_block) * sizeof(Beat));
				if (ptr == NULL) exit(1);
				lvl->beats = ptr;
				printf("incrementing beats mem.\n");
			}

			int measure = 0;
			sscanf(line, BEAT_FORMAT, &measure, &lvl->beats[n].x);
			lvl->beats[n].x += measure;
			lvl->beats[n].x *= (SCREEN_WIDTH / 4); // assuming 4/4 time sig

			n++;
		}
		else if (line[0] == '>')
		{
			sscanf(line, BPM_FORMAT, &lvl->bpm);
		}
	}
}
