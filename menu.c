#include "menu.h"

SDL_Texture* font_texture = NULL;

MenuItem play_button, quit_button, resume_button, exit_button, level1, level2, level3,
	score_header, score_display;

Menu main_menu, pause_menu, level_menu, play_menu;

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

	Letter* ptr = realloc(letters, strlen(s)*sizeof(Letter));
	if (ptr == NULL) exit(1);
	letters = ptr;

	for (int i = 0; i < strlen(s); i++) 
	{
		int j = (int)(s[i] - '0');
		if (j > 9) j -= 7;

		letters[i].texture_coord = j;
		letters[i].texture_coord *= 8;

		//printf("letter_coord %d: %d\n", i, letters[i].texture_coord);
	}

	return letters;
}

void menu_init(SDL_Renderer* renderer)
{
	if (renderer == NULL) return;

	play_button = (MenuItem) {
	.rect = (SDL_Rect){ 300, 300, 300, 200 },
	.text = "PLAY",
	.letters = NULL,
	.num_letters = 4,
	.text_texture = NULL,
	};

	quit_button = (MenuItem) {
		.rect = (SDL_Rect){ 700, 400, 300, 100 },
		.text = "QUIT",
		.letters = NULL,
		.num_letters = 4,
		.text_texture = NULL,
	};

	resume_button = (MenuItem) {
		.rect = (SDL_Rect){ 100, 50, 300, 100 },
		.text = "RESUME",
		.letters = NULL,
		.num_letters = 6,
		.text_texture = NULL,
	};

	exit_button = (MenuItem) {
		.rect = (SDL_Rect){ 100, 200, 300, 100 },
		.text = "EXIT",
		.letters = NULL,
		.num_letters = 4,
		.text_texture = NULL,
	};

	level1 = (MenuItem){
		.rect = (SDL_Rect){ 100, 50, 300, 100 },
		.text = "LEVEL 1",
		.letters = NULL,
		.num_letters = 6,
		.text_texture = NULL,
	};

	level2 = (MenuItem){
		.rect = (SDL_Rect){ 100, 200, 300, 100 },
		.text = "LEVEL 2",
		.letters = NULL,
		.num_letters = 6,
		.text_texture = NULL,
	};

	level3 = (MenuItem){
		.rect = (SDL_Rect){ 100, 350, 300, 100 },
		.text = "LEVEL 3",
		.letters = NULL,
		.num_letters = 6,
		.text_texture = NULL,
	};

	score_header = (MenuItem){
		.rect = (SDL_Rect){ 10, 10, 300, 75 },
		.text = "SCORE",
		.letters = NULL,
		.num_letters = 5,
		.text_texture = NULL,
	};

	score_display = (MenuItem){
		.rect = (SDL_Rect){ 350, 10, 50, 75 },
		.text = "0",
		.letters = NULL,
		.num_letters = 1,
		.text_texture = NULL,
	};

	main_menu = (Menu) {
		.menu_items = NULL,
		.num_menu_items = 2,
		.bkg_texture = NULL,
	};

	pause_menu = (Menu) {
		.menu_items = NULL,
		.num_menu_items = 2,
		.bkg_texture = NULL,
	};

	level_menu = (Menu){
		.menu_items = NULL,
		.num_menu_items = 3,
		.bkg_texture = NULL,
	};

	play_menu = (Menu){
		.menu_items = NULL,
		.num_menu_items = 2,
		.bkg_texture = NULL,
	};

	font_texture = IMG_LoadTexture(renderer, "alphabet.bmp");
	if (font_texture == NULL)
	{
		printf("font not loaded: %s\n", IMG_GetError());
		exit(1);
	}

	//capitalize(play_button.text); // causing seg fault

	// MAIN MENU
	// 
	//play_button.letters = malloc(sizeof(Letter));
	//play_button.letters = map_letter_coords(play_button.text, play_button.letters);
	play_button.text_texture = text_create_texture(play_button.text);
	play_button.text_width = text_get_width();
	play_button.text_height = text_get_height();
	//printf("Play Button Text Texture: %p\n\n", play_button.text_texture);

	//quit_button.letters = malloc(sizeof(Letter));
	//quit_button.letters = map_letter_coords(quit_button.text, quit_button.letters);
	quit_button.text_texture = text_create_texture(quit_button.text);
	quit_button.text_width = text_get_width();
	quit_button.text_height = text_get_height();

	main_menu.menu_items = malloc((size_t)2 * sizeof(MenuItem));
	main_menu.menu_items[0] = play_button;
	main_menu.menu_items[1] = quit_button;

	// PAUSE MENU
	//resume_button.letters = malloc(sizeof(Letter));
	//resume_button.letters = map_letter_coords(resume_button.text, resume_button.letters);
	resume_button.text_texture = text_create_texture(resume_button.text);
	resume_button.text_width = text_get_width();
	resume_button.text_height = text_get_height();

	//exit_button.letters = malloc(sizeof(Letter));
	//exit_button.letters = map_letter_coords(exit_button.text, exit_button.letters);
	exit_button.text_texture = text_create_texture(exit_button.text);
	exit_button.text_width = text_get_width();
	exit_button.text_height = text_get_height();

	pause_menu.menu_items = malloc((size_t)2 * sizeof(MenuItem));
	pause_menu.menu_items[0] = resume_button;
	pause_menu.menu_items[1] = exit_button;

	// LEVEL MENU
	//level1.letters = malloc(sizeof(Letter));
	//level1.letters = map_letter_coords(level1.text, level1.letters);
	level1.text_texture = text_create_texture(level_names[0]);
	level1.text_width = text_get_width();
	level1.text_height = text_get_height();

	//level2.letters = malloc(sizeof(Letter));
	//level2.letters = map_letter_coords(level2.text, level2.letters);
	level2.text_texture = text_create_texture(level_names[1]);
	level2.text_width = text_get_width();
	level2.text_height = text_get_height();

	//level3.letters = malloc(sizeof(Letter));
	//level3.letters = map_letter_coords(level3.text, level3.letters);
	level3.text_texture = text_create_texture(level_names[2]);
	level3.text_width = text_get_width();
	level3.text_height = text_get_height();

	level_menu.menu_items = malloc((size_t)3 * sizeof(MenuItem));
	level_menu.menu_items[0] = level1;
	level_menu.menu_items[1] = level2;
	level_menu.menu_items[2] = level3;

	// PLAY MENU
	//score_header.letters = malloc(sizeof(Letter));
	//score_header.letters = map_letter_coords(score_header.text, score_header.letters);
	score_header.text_texture = text_create_texture(score_header.text);
	score_header.text_width = text_get_width();
	score_header.text_height = text_get_height();

	//score_display.letters = malloc(sizeof(Letter));
	//score_display.letters = map_letter_coords(score_display.text, score_display.letters);
	score_display.text_texture = text_create_texture(score_display.text);
	score_display.text_width = text_get_width();
	score_display.text_height = text_get_height();

	play_menu.menu_items = malloc((size_t)2 * sizeof(MenuItem));
	play_menu.menu_items[0] = score_header;
	play_menu.menu_items[1] = score_display;

	//printf("sizeof MenuItem: %zu\n\n", sizeof(MenuItem));
}

int menu_click(int x, int y)
{
	for (int i = 0; i < cur_menu->num_menu_items; i++)
	{
		if (x > cur_menu->menu_items[i].rect.x && x < cur_menu->menu_items[i].rect.x + cur_menu->menu_items[i].rect.w &&
				y > cur_menu->menu_items[i].rect.y && y < cur_menu->menu_items[i].rect.y + cur_menu->menu_items[i].rect.h)
		{
			//printf("menu item clicked\n\n");
			return i;
		}
	}

	return -1;
}

static void menu_render_letters(SDL_Renderer* renderer, MenuItem* m)
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
	if (cur_menu != &play_menu) SDL_RenderClear(renderer);
	//printf("menu rendering...\n");

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

	for (int i = 0; i < cur_menu->num_menu_items; i++)
	{
		if (cur_menu != &play_menu) SDL_RenderDrawRect(renderer, &cur_menu->menu_items[i].rect);

		//menu_render_letters(renderer, &cur_menu->menu_items[i]);
		//if (cur_menu->menu_items[i].text_texture == NULL) printf("text texture is NULL\n\n");
		SDL_Rect temp_rect = (SDL_Rect){ cur_menu->menu_items[i].rect.x, cur_menu->menu_items[i].rect.y,
			cur_menu->menu_items[i].text_width, cur_menu->menu_items[i].text_height };

		int s = SDL_RenderCopy(renderer, cur_menu->menu_items[i].text_texture, NULL, &temp_rect);
		if (s == -1) printf("Rendering error: %s\n", SDL_GetError());
	}
	
	if (cur_menu != &play_menu) SDL_RenderPresent(renderer);
	//else printf("num menu items: %d\n\n", play_menu.num_menu_items);
}

static void menu_reset_score()
{
	//score_display.num_letters = 1;
	score_display.text = "0";
	//score_display.letters = map_letter_coords(score_display.text, score_display.letters);
	score_display.text_texture = text_update_texture(score_display.text_texture, score_display.text);
	score_display.text_width = text_get_width();
	score_display.text_height = text_get_height();
	play_menu.menu_items[1] = score_display;
}

void menu_set_pause()
{
	cur_menu = &pause_menu;
}

void menu_set_main()
{
	cur_menu = &main_menu;
	menu_reset_score();
}

void menu_set_level()
{
	cur_menu = &level_menu;
}

void menu_set_play()
{
	cur_menu = &play_menu;
}

void menu_update_score(int s)
{
	char str[512] = { 0 };
	sprintf_s(str, (size_t)512, "%d", s);

	score_display.num_letters = strlen(str);
	//score_display.letters = map_letter_coords(str, score_display.letters);

	score_display.text_texture = text_update_texture(score_display.text_texture, str);
	score_display.text_width = text_get_width();
	score_display.text_height = text_get_height();
	//score_display.rect.w = 50 * score_display.num_letters;

	play_menu.menu_items[1] = score_display; // TODO: MAKE MENUS STORE POINTERS TO MENU ITEMS INSTEAD OF COPIES
}

void menu_quit()
{
	free(play_button.letters);
	SDL_DestroyTexture(play_button.text_texture);

	free(quit_button.letters);
	SDL_DestroyTexture(quit_button.text_texture);
	
	free(resume_button.letters);
	SDL_DestroyTexture(resume_button.text_texture);
	
	free(exit_button.letters);
	SDL_DestroyTexture(exit_button.text_texture);
	
	free(level1.letters);
	SDL_DestroyTexture(level1.text_texture);
	
	free(level2.letters);
	SDL_DestroyTexture(level2.text_texture);
	
	free(level3.letters);
	SDL_DestroyTexture(level3.text_texture);
	
	free(score_header.letters);
	SDL_DestroyTexture(score_header.text_texture);
	
	free(score_display.letters);
	SDL_DestroyTexture(score_display.text_texture);

	SDL_DestroyTexture(font_texture);

	free(main_menu.menu_items);
	free(pause_menu.menu_items);
	free(level_menu.menu_items);
	free(play_menu.menu_items);
}
