#include "level.h"

const char* BEAT_FORMAT = "b %d %f\n";
const char* BPM_FORMAT = "> %f\n";
const char* TRACK_FORMAT = "t %d %d\n";

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
			lvl->beats[n].x *= (SCREEN_WIDTH / 4); // assuming 4/4 time sig
			lvl->beats[n].x += measure * SCREEN_WIDTH;
			lvl->beats[n].y = 300.0f;
			lvl->beats[n].sprite.w = 64;
			lvl->beats[n].sprite.h = 64;

			printf("%f\n", lvl->beats[n].x);

			n++;
		}
		else if (line[0] == '>')
		{
			sscanf(line, BPM_FORMAT, &lvl->bpm);
		}
		else if (line[0] == 't')
		{
			//sscanf(line, TRACK_FORMAT, &lvl->tracks[0].x, &lvl->tracks[0].y);
			track_init(&lvl->tracks[0]);
		}
	}

	lvl->num_beats = n;
	// printf("%d\n", lvl->num_beats);
	// printf("bpm: %f\n", lvl->bpm);
	lvl->cur_beat = 0;

	fclose(file);
}
