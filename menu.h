#ifndef MENU_HEADER
#define MENU_HEADER

#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//SDL_Texture* font_texture;

typedef struct Letter {
	int x;
	int y;
	SDL_Rect rect;
	int texture_coord;
} Letter;

typedef struct MenuItem {
	int x;
	int y;
	SDL_Rect rect;
	char* text;
	Letter* letters;
	int num_letters;
} MenuItem;

typedef struct Menu {
	MenuItem* menu_items;
	int num_menu_items;
	SDL_Texture* bkg_texture;
} Menu;

//Menu main_menu;
//Menu pause_menu;

//extern Menu menus[2]; // [ NULL, main_menu, pause_menu ] mapped to global GameState enum values

void capitalize(char* s);

Letter* map_letter_coords(char* s, Letter* letters);

void menu_init(SDL_Renderer* renderer); // load font_texture, load Letters' texture_coords (could be done manually but less scalable) 

void menu_render(SDL_Renderer* renderer);

void menu_quit();

#endif
