#include "level.h"

const char* BEAT_FORMAT = "b %d %f\n"; // track, beat in measure 
const char* MEASURE_FORMAT = "m %d\n";
const char* BPM_FORMAT = "> %f\n";
const char* TRACK_FORMAT = "t %d %d\n"; // x, y
const char* PRACTICE_FORMAT = "* %d\n"; // scan practice_pos

void level_load(Level* lvl, char* file_name)
{
	if (lvl == NULL) return;

	FILE* file = fopen(file_name, "r");
	if (file == NULL)
	{
		printf("file could not be opened.\n\n");
		return;
	}

	char line[1024];
	// int n = 0;
	int beat_block = 10;
	int track_index = 0;
	int measure = 0;
	int practice_pos = 0;

    for (int i = 0; i < 5; i++)
    {
        Beat* ptr = realloc(lvl->tracks[i].beats, (size_t)(beat_block) * sizeof(Beat));
        if (ptr == NULL) exit(1);
        lvl->tracks[i].beats = ptr;

        lvl->tracks[i].cur_beat = 0;

		lvl->tracks[i].num_beats = 0;

		lvl->tracks[i].beat_block = beat_block;
    }

	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == 'b')
		{
			float b = 0.0f;

			int track_marker = 0;

			sscanf_s(line, BEAT_FORMAT, &track_marker, &b);

			if (b > 4.0f || b < 0.0f) continue;
			if (track_marker > 4 || track_marker < 0) continue;

			if (lvl->tracks[track_marker].num_beats == lvl->tracks[track_marker].beat_block)
			{
				lvl->tracks[track_marker].beat_block += 10;
				Beat* ptr = realloc(lvl->tracks[track_marker].beats, (size_t)(lvl->tracks[track_marker].beat_block) * sizeof(Beat));
				if (ptr == NULL) exit(1);
				lvl->tracks[track_marker].beats = ptr;
			}

			Beat load_beat;

			load_beat.x = b;
			load_beat.x *= (SCREEN_WIDTH / 4); // assuming 4/4 time sig
			load_beat.x += measure * SCREEN_WIDTH;

			load_beat.x += lvl->tracks[track_marker].x;
			load_beat.y = lvl->tracks[track_marker].y;

			load_beat.sprite.w = 64;
			load_beat.sprite.h = 64;

			lvl->tracks[track_marker].beats[lvl->tracks[track_marker].num_beats] = load_beat;

			lvl->tracks[track_marker].num_beats++;
		}
		else if (line[0] == 'm')
		{
			sscanf_s(line, MEASURE_FORMAT, &measure);
		}
		else if (line[0] == '>')
		{
			sscanf_s(line, BPM_FORMAT, &lvl->bpm);
		}
		else if (line[0] == 't')
		{
			int track_x, track_y = 0;
			sscanf_s(line, TRACK_FORMAT, &track_x, &track_y);
			track_init(&lvl->tracks[track_index], track_x, track_y);
			track_index++;
		}
		else if (line[0] == '*')
		{
			sscanf_s(line, PRACTICE_FORMAT, &practice_pos);
			printf("practice pos: %d\n\n", practice_pos);
			lvl->start_pos = 1.0 / (double)lvl->bpm * 60.0 * 4.0 * practice_pos;
			printf("start pos: %f\n\n", lvl->start_pos);
		}
	}

	fclose(file);
	file = NULL;

	lvl->speed = (SCREEN_WIDTH / 4000.0f) * (lvl->bpm / 60.0f);

	lvl->num_tracks = track_index;

	if (lvl->start_pos > 0.0)
	{
		for (int i = 0; i < lvl->num_tracks; i++)
		{
			for (int j = 0; j < lvl->tracks[i].num_beats; j++)
			{
				lvl->tracks[i].beats[j].x -= lvl->speed * 1000 * (int)lvl->start_pos; // prac pos rounded bc mixer rounds start pos
				if (lvl->tracks[i].beats[j].x < lvl->tracks[i].x) lvl->tracks[i].cur_beat++;
			}
		}
	}
}

void level_free(Level* lvl)
{
	if (lvl == NULL) return;

	for (int i = 0; i < 5; i++)
	{
		if (lvl->tracks[i].beats == NULL) continue;
		free(lvl->tracks[i].beats);
		lvl->tracks[i].beats = NULL;
	}

	free(lvl);
}

