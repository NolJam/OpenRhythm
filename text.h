#ifndef TEXT_DEF
#define TEXT_DEF

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "globals.h"

TTF_Font* font;

int temp_text_width;
int temp_text_height;

int text_init();

SDL_Texture* text_create_texture(const char* s);

SDL_Texture* text_update_texture(SDL_Texture* t, const char* s);

int text_get_width();

int text_get_height();

void text_quit();

#endif 