#include "menu.h"

SDL_Texture* font_texture = NULL;

MenuItem play_button = {
	.x = 100,
	.y = 100,
	.rect = (SDL_Rect){ 300, 300, 300, 200 },
	.text = "play",
	.letters = NULL,
	.num_letters = 4,
};

Menu main_menu = {
	.menu_items = NULL,
	.num_menu_items = 1,
	.bkg_texture = NULL,
};

Menu pause_menu = {
	.bkg_texture = NULL,
};

//Menu menus = [ main_menu, pause_menu ];

void capitalize(char* s)
{
	for (int i = 0; i < strlen(s); i++) 
	{
		s[i] = toupper(s[i]);
	}
}

void map_letter_coords(char* s, Letter* letters)
{
	letters = realloc(letters, strlen(s)*sizeof(Letter));

	for (int i = 0; i < strlen(s); i++) 
	{
		int j = (int)(s[i] - '0');
		if (j > 9) j -= 7;

		letters[i].texture_coord = j;
		letters[i].texture_coord *= 8;
	}
}

void menu_init(SDL_Renderer* renderer)
{
	font_texture = IMG_LoadTexture(renderer, "alphabet.bmp");
	if (font_texture == NULL)
	{
		printf("font not loaded: %s\n", IMG_GetError());
		exit(1);
	}

	play_button.letters = malloc(sizeof(Letter));
	map_letter_coords(play_button.text, play_button.letters);

	main_menu.menu_items = malloc(sizeof(MenuItem));
	main_menu.menu_items[0] = play_button;
}

void menu_render_letters(SDL_Renderer* renderer, MenuItem* m)
{
	SDL_Rect source_rect = {
		.x = 0,
		.y = 0,
		.h = 8,
		.w = 8,
	};
	SDL_Rect letter_rect = {
		.x = m->x + 20,
		.y = m->y + 20,
		.w = 20,
		.h = 20,
	};

	for (int i = 0; i < m->num_letters; i++)
	{
		source_rect.x = m->letters[i].texture_coord;

		SDL_RenderCopy(renderer, font_texture, &source_rect, &letter_rect);

		letter_rect.x += 20;
	}
}

void menu_render(SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	for (int i = 0; i < main_menu.num_menu_items; i++)
	{
		SDL_RenderDrawRect(renderer, &main_menu.menu_items[i].rect);

		menu_render_letters(renderer, &main_menu.menu_items[i]);
	}
}

void menu_quit()
{
	free(play_button.letters);
	free(main_menu.menu_items);
}
