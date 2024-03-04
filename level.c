#include "level.h"

const char* BEAT_FORMAT = "b %d %f\n"; // track, beat in measure 
const char* MEASURE_FORMAT = "m %d\n";
const char* BPM_FORMAT = "> %f\n";
const char* TRACK_FORMAT = "t %d %d\n"; // x, y

void level_load(Level* lvl, char* file_name)
{
	if (lvl == NULL) return;

	FILE* file = fopen(file_name, "r");
	if (file == NULL)
	{
		printf("file could not be opened.\n\n");
	}

	char line[1024];
	// int n = 0;
	int beat_block = 10;
	int track_index = 0;
	int measure = 0;

    for (int i = 0; i < 5; i++)
    {
        Beat* ptr = realloc(lvl->tracks[i].beats, (size_t)(beat_block) * sizeof(Beat));
        if (ptr == NULL) exit(1);
        lvl->tracks[i].beats = ptr;

        lvl->tracks[i].cur_beat = 0;

	lvl->tracks[i].num_beats = 0;

	lvl->tracks[i].beat_block = 10;

	//printf("index: %d\n", i);
	//printf("track beats realloc'd\n\n");
	//printf("beat block: %d\n\n", lvl->tracks[i].beat_block);
    }

	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == 'b')
		{
			//int track_marker = 0;
            		int track_marker = line[2] - '0';
			if (track_marker > 4 || track_marker < 0) continue;

			if (lvl->tracks[track_marker].num_beats == lvl->tracks[track_marker].beat_block)
			{
				//printf("incrementing beats mem.\n");
				lvl->tracks[track_marker].beat_block += 10;
				Beat* ptr = realloc(lvl->tracks[track_marker].beats, (size_t)(lvl->tracks[track_marker].beat_block) * sizeof(Beat));
				if (ptr == NULL) exit(1);
				lvl->tracks[track_marker].beats = ptr;
				//printf("track marker: %d\n", track_marker);
				//printf("beat address: %p\n", lvl->tracks[track_marker].beats);
			}

			//int measure = 0;
			float b = 0.0f;
			//int track_marker = 0; // defined earlier
			sscanf(line, BEAT_FORMAT, &track_marker, &b);
			//printf("beat value: %f\n", b);
			//printf("^track marker: %d\n\n", track_marker);

			Beat load_beat;

			load_beat.x = b;
			load_beat.x *= (SCREEN_WIDTH / 4); // assuming 4/4 time sig
			load_beat.x += measure * SCREEN_WIDTH;

			load_beat.x += lvl->tracks[track_marker].x;
			load_beat.y = lvl->tracks[track_marker].y + 8;

			load_beat.sprite.w = 64;
			load_beat.sprite.h = 64;

			lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats] = load_beat;

			//printf("%f\n\n", lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats].x);

			lvl->tracks[track_marker].num_beats++;
		}
		else if (line[0] == 'm')
		{
			sscanf(line, MEASURE_FORMAT, &measure);
			//printf("writing measure: %d\n\n", measure);
		}
		else if (line[0] == '>')
		{
			sscanf(line, BPM_FORMAT, &lvl->bpm);
		}
		else if (line[0] == 't')
		{
			int track_x, track_y = 0;
			sscanf(line, TRACK_FORMAT, &track_x, &track_y);
			track_init(&lvl->tracks[track_index], track_x, track_y);
			track_index++;
		}
	}

	//lvl->num_beats = n;
	// printf("%d\n", lvl->num_beats);
	// printf("bpm: %f\n", lvl->bpm);
	//lvl->cur_beat = 0;

	lvl->speed = (SCREEN_WIDTH / 4000.0f) * (lvl->bpm / 60.0f);

	lvl->num_tracks = track_index;

	fclose(file);
}

void level_free(Level* lvl)
{
	if (lvl == NULL) return;

	for (int i = 0; i < 5; i++)
	{
		free(lvl->tracks[i].beats);
		lvl->tracks[i].beats = NULL;
	}

	free(lvl);
}

