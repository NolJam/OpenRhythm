#include "menu.h"

SDL_Texture* font_texture = NULL;

MenuItem play_button = {
	.rect = (SDL_Rect){ 300, 300, 300, 200 },
	.text = "PLAY",
	.letters = NULL,
	.num_letters = 4,
};

MenuItem quit_button = {
	.rect = (SDL_Rect){ 700, 400, 300, 100 },
	.text = "QUIT",
	.letters = NULL,
	.num_letters = 4,
};

MenuItem resume_button = {
	.rect = (SDL_Rect){ 100, 50, 300, 100 },
	.text = "RESUME",
	.letters = NULL,
	.num_letters = 6,
};

MenuItem exit_button = {
	.rect = (SDL_Rect){ 100, 200, 300, 100 },
	.text = "EXIT",
	.letters = NULL,
	.num_letters = 4,
};

Menu main_menu = {
	.menu_items = NULL,
	.num_menu_items = 2,
	.bkg_texture = NULL,
};

Menu pause_menu = {
	.menu_items = NULL,
	.num_menu_items = 2,
	.bkg_texture = NULL,
};

Menu* cur_menu = &main_menu;
//Menu menus = [ main_menu, pause_menu ];

void capitalize(char* s)
{
	for (int i = 0; i < strlen(s); i++) 
	{
		s[i] = toupper(s[i]);
	}

	printf("capitalized string: %s\n\n", s);
}

Letter* map_letter_coords(char* s, Letter* letters)
{
	if (letters == NULL) return NULL;

	letters = realloc(letters, strlen(s)*sizeof(Letter));

	for (int i = 0; i < strlen(s); i++) 
	{
		int j = (int)(s[i] - '0');
		if (j > 9) j -= 7;

		letters[i].texture_coord = j;
		letters[i].texture_coord *= 8;

		printf("letter_coord %d: %d\n", i, letters[i].texture_coord);
	}

	return letters;
}

void menu_init(SDL_Renderer* renderer)
{
	if (renderer == NULL) return;

	font_texture = IMG_LoadTexture(renderer, "alphabet.bmp");
	if (font_texture == NULL)
	{
		printf("font not loaded: %s\n", IMG_GetError());
		exit(1);
	}

	//capitalize(play_button.text); // causing seg fault
	play_button.letters = malloc(sizeof(Letter));
	play_button.letters = map_letter_coords(play_button.text, play_button.letters);

	quit_button.letters = malloc(sizeof(Letter));
	quit_button.letters = map_letter_coords(quit_button.text, quit_button.letters);

	main_menu.menu_items = malloc((size_t)2 * sizeof(MenuItem));
	main_menu.menu_items[0] = play_button;
	main_menu.menu_items[1] = quit_button;


	resume_button.letters = malloc(sizeof(Letter));
	resume_button.letters = map_letter_coords(resume_button.text, resume_button.letters);

	exit_button.letters = malloc(sizeof(Letter));
	exit_button.letters = map_letter_coords(exit_button.text, exit_button.letters);

	pause_menu.menu_items = malloc((size_t)2 * sizeof(MenuItem));
	pause_menu.menu_items[0] = resume_button;
	pause_menu.menu_items[1] = exit_button;

	printf("sizeof MenuItem: %zu\n\n", sizeof(MenuItem));
}

int menu_click(int x, int y)
{
	for (int i = 0; i < cur_menu->num_menu_items; i++)
	{
		if (x > cur_menu->menu_items[i].rect.x && x < cur_menu->menu_items[i].rect.x + cur_menu->menu_items[i].rect.w &&
				y > cur_menu->menu_items[i].rect.y && y < cur_menu->menu_items[i].rect.y + cur_menu->menu_items[i].rect.h)
		{
			printf("menu item clicked\n\n");
			return i;
		}
	}

	return -1;
}

void menu_render_letters(SDL_Renderer* renderer, MenuItem* m)
{
	if (renderer == NULL || m == NULL) return;

	SDL_Rect source_rect = {
		.x = 0,
		.y = 0,
		.h = 8,
		.w = 8,
	};
	SDL_Rect letter_rect = {
		.x = m->rect.x + 20,
		.y = m->rect.y + 20,
		//.x = 100,
		//.y = 100,
		.w = 20,
		.h = 20,
	};

	for (int i = 0; i < m->num_letters; i++)
	{
		source_rect.x = m->letters[i].texture_coord;

		SDL_RenderCopy(renderer, font_texture, &source_rect, &letter_rect);

		letter_rect.x += 25;
	}
}

void menu_render(SDL_Renderer* renderer)
{
	if (renderer == NULL) return;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderClear(renderer);
	//printf("menu rendering...\n");

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	for (int i = 0; i < cur_menu->num_menu_items; i++)
	{
		SDL_RenderDrawRect(renderer, &cur_menu->menu_items[i].rect);

		menu_render_letters(renderer, &cur_menu->menu_items[i]);
	}
	
	SDL_RenderPresent(renderer);
}

void menu_set_pause()
{
	cur_menu = &pause_menu;
}

void menu_set_main()
{
	cur_menu = &main_menu;
}

void menu_quit()
{
	free(play_button.letters);
	free(quit_button.letters);
	free(resume_button.letters);
	free(exit_button.letters);
	//main_menu.menu_items = NULL;
	SDL_DestroyTexture(font_texture);
	free(main_menu.menu_items);
	free(pause_menu.menu_items);
}
