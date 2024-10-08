#include "text.h"

int text_init()
{
	temp_text_width = 0;
	temp_text_height = 0;

	int r = 0;

	r = TTF_Init();
	if (r == -1) printf("TTF could not init\n\n");

	font = TTF_OpenFont("montserrat/Montserrat-Regular.ttf", 24);
	if (font == NULL) printf("font could not be loaded\n\n");

	return r;
}

SDL_Texture* text_create_texture(const char* s)
{
	//if (texture != NULL) SDL_DestroyTexture(texture);
	//texture = NULL;

	SDL_Surface* text_surface = TTF_RenderText_Solid(font, s, (SDL_Color) { 0, 0, 0 });

	if (text_surface == NULL) printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());

	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	//printf("texture adress: %p\n\n", text_texture);

	if (text_texture == NULL) printf("text texture could not be created\n\n");

	temp_text_width = text_surface->w;
	temp_text_height = text_surface->h;

	SDL_FreeSurface(text_surface);

	//printf("texture adress: %p\n\n", text_texture);
	return text_texture;
}

SDL_Texture* text_update_texture(SDL_Texture* t, const char* s)
{
	if (t == NULL || s == NULL) return NULL;

	SDL_DestroyTexture(t);
	t = NULL;

	SDL_Surface* text_surface = TTF_RenderText_Solid(font, s, (SDL_Color) { 0, 0, 0 });
	if (text_surface == NULL) printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());

	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

	temp_text_width = text_surface->w;
	temp_text_height = text_surface->h;

	SDL_FreeSurface(text_surface);

	return text_texture;
}

int text_get_width()
{
	return temp_text_width;
}

int text_get_height()
{
	return temp_text_height;
}

void text_quit()
{
	TTF_CloseFont(font);
	TTF_Quit();
}