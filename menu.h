#ifndef TEXT_HEADER
#define TEXT_HEADER

#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

SDL_Texture* font_texture;

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
} MenuItem;

typedef struct Menu {
	MenuItem* menu_items;
	SDL_Texture* bkg_texture;
} Menu;

Menu main_menu;
Menu pause_menu;

extern Menu* menus[3]; // [ NULL, main_menu, pause_menu ] mapped to global GameState enum values

void menu_init(); // load font_texture, load Letters' texture_coords (could be done manually but less scalable) 

void pause(); // maybe put this in main.c instead

void capitalize(char* s);

void map_font_texture(char* c, Letter* letter);

void menu_render(SDL_Renderer* renderer, Menu* menu);

#endif
