#include "include/tsee.h"

bool TSEEInitText(TSEE *tsee, bool loadDefault) {
	if (TTF_Init() == -1) {
		TSEEError("Failed to init SDL_TTF: %s\n", TTF_GetError());
	}
	tsee->fonts = TSEEArrayCreate();
	if (loadDefault)
		return TSEELoadFont(tsee, "fonts/default.ttf", 16, "_default");
	return true;
}

bool TSEELoadFont(TSEE *tsee, char *path, int size, char *name) {
	TSEE_Font *font = malloc(sizeof(*font));
	font->font = TTF_OpenFont(path, size);
	if (font->font == NULL) {
		TSEEWarn("Failed to load font: %s. Error: %s\n", path, TTF_GetError());
		return false;
	}
	font->name = strdup(name);
	font->size = size;
	TSEEArrayAppend(tsee->fonts, font);
	return true;
}

bool TSEEUnloadFont(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *font = tsee->fonts->data[i];
		if (strcmp(font->name, name) == 0) {
			TTF_CloseFont(font->font);
			free(font->name);
			free(font);
			TSEEArrayDelete(tsee->fonts, i);
			return true;
		}
	}
	TSEEWarn("Attempted to unload non-existant font: %s\n", name);
	return false;
}

bool TSEEUnloadAllFonts(TSEE *tsee) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *font = tsee->fonts->data[i];
		TTF_CloseFont(font->font);
		free(font->name);
	}
	TSEEArrayFree(tsee->fonts);
	return true;
}

TTF_Font *TSEEGetFont(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *data = tsee->fonts->data[i];
		if (strcmp(data->name, name) == 0) {
			return data->font;
		}
	}
	TSEEWarn("Failed to find font: %s\n", name);
	return NULL;
}

TSEE_Text *TSEECreateText(TSEE *tsee, char *fontName, char *text, SDL_Color color) {
	TSEE_Text *textObj = malloc(sizeof(*textObj));
	TTF_Font *font = TSEEGetFont(tsee, fontName);
	if (!font) {
		TSEEWarn("Failed to create text \"%s\" with font \"%s\"\n", text, fontName);
	}
	textObj->text = strdup(text);
	textObj->texture = malloc(sizeof(*textObj->texture));
	SDL_Surface *surf = TTF_RenderText_Blended(font, text, color);
	if (!surf) {
		TSEEWarn("Failed to create text \"%s\" with font \"%s\"\n", text, fontName);
	}
	textObj->texture->texture = SDL_CreateTextureFromSurface(tsee->window->renderer, surf);
	return textObj;
}

bool TSEERenderText(TSEE *tsee, TSEE_Text *text) {
	if (!text->texture) {
		TSEEWarn("Failed to render text: %s\n", text->text);
		return false;
	}
	if (SDL_RenderCopy(tsee->window->renderer, text->texture->texture, NULL, &text->texture->rect) == -1) {
		TSEEError("Failed to render text \"%s\"\nError: %s\n", text->text, SDL_GetError());
		return false;
	}
	return true;
}