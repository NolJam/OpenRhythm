#ifndef MENU_HEADER
#define MENU_HEADER

#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include "text.h"
#include "globals.h"

//SDL_Texture* font_texture;

typedef struct Letter {  // vestigial
	int x;
	int y;
	SDL_Rect rect;
	int texture_coord;
} Letter;

typedef struct MenuItem {
	SDL_Rect rect;
	char* text;
	Letter* letters; // vestigial
	int num_letters; // vestigial
	SDL_Texture* text_texture;
	int text_width;
	int text_height;
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

int menu_click(int x, int y);

void menu_render(SDL_Renderer* renderer);

void menu_set_pause(); // set current meny to pause for menu_render func

void menu_set_main(); // set current menu to main for menu_render func

void menu_set_level();

void menu_set_play(); // set cur menu to play_menu

void menu_update_score(int s);

void menu_quit();

#endif
