#include "level.h"

const char* BEAT_FORMAT = "b %d %d %f\n"; // track, measure, beat in measure
const char* BPM_FORMAT = "> %f\n";
const char* TRACK_FORMAT = "t %d %d\n"; // x, y

void level_load(Level* lvl, char* file_name)
{
	FILE* file = fopen(file_name, "r");
	if (!file)
	{
		printf("file could not be opened.\n\n");
	}

	char line[100];
	// int n = 0;
	int beat_block = 10;
	int track_index = 0;

    for (int i = 0; i < 5; i++)
    {
        Beat* ptr = realloc(lvl->tracks[i].beats, (size_t)(beat_block) * sizeof(Beat));
        if (ptr == NULL) exit(1);
        lvl->tracks[i].beats = ptr;

        lvl->tracks[i].cur_beat = 0;

		lvl->tracks[i].beat_block = 10;

		// printf("index: %d\n", i);
		printf("track beats realloc'd\n\n");
		printf("beat block: %d\n\n", lvl->tracks[i].beat_block);
    }

	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == 'b')
		{
            int track_marker = line[2] - '0';
			printf("track marker: %d\n\n", track_marker);

			if (lvl->tracks[track_marker].num_beats == lvl->tracks[track_marker].beat_block)
			{
				printf("incrementing beats mem.\n");
				lvl->tracks[track_marker].beat_block += 10;
				Beat* ptr = realloc(lvl->tracks[track_marker].beats, (size_t)(lvl->tracks[track_marker].beat_block) * sizeof(Beat));
				if (ptr == NULL) exit(1);
				lvl->tracks[track_marker].beats = ptr;
			}

			int measure = 0;
			float b = 0;
			//int track_marker = 0; // defined earlier
			sscanf(line, BEAT_FORMAT, &track_marker, &measure, &b);
			printf("beat value: %f\n", b);

			lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats].x = b;
			lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats].x *= (SCREEN_WIDTH / 4); // assuming 4/4 time sig
			lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats].x += measure * SCREEN_WIDTH;

			lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats].x += lvl->tracks[track_marker].x;
			lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats].y = lvl->tracks[track_marker].y;

			lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats].sprite.w = 64;
			lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats].sprite.h = 64;

			printf("%f\n", lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats].x);

			lvl->tracks[track_marker].num_beats++;
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
