#include "menu.h"

SDL_Texture* font_texture = NULL;

MenuItem play_button = {
	.x = 100,
	.y = 100,
	.rect = (SDL_Rect){ 300, 300, 300, 200 },
	.text = "play",
	.letters = NULL,
};

Menu main_menu = NULL;
Menu pause_menu = NULL;

Menu* menus = [ NULL, main_menu, pause_menu ];

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

		letters[i]->texture_coord = j;
		letters[i]->texture_coord *= 8;
	}
}

void menu_init()
{
	main_menu = malloc(sizeof(Menu));


	main_menu->menu_items = malloc(sizeof(MenuItem));
	*main_menu->menu_items[0] = play_button;
}
