#include "../tsee.h"

bool TSEEInitText(TSEE *tsee, bool loadDefault) {
	if (tsee->init->text) {
		return true;
	}
	if (TTF_Init() == -1) {
		TSEEError("Failed to init SDL_TTF (%s)\n", TTF_GetError());
		return false;
	}
	tsee->fonts = TSEEArrayCreate();
	if (loadDefault) {
		if (TSEELoadFont(tsee, "fonts/default.ttf", 16, "_default")) {
			tsee->init->text = true;
			return true;
		} else {
			tsee->init->text = false;
			return false;
		}
	}
	tsee->init->text = true;
	return true;
}

bool TSEELoadFont(TSEE *tsee, char *path, int size, char *name) {
	TSEE_Font *font = xmalloc(sizeof(*font));
	font->font = TTF_OpenFont(path, size);
	if (font->font == NULL) {
		TSEEWarn("Failed to load font `%s` (%s)\n", path, TTF_GetError());
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
			xfree(font->name);
			xfree(font);
			TSEEArrayDelete(tsee->fonts, i);
			return true;
		}
	}
	TSEEWarn("Attempted to unload non-existant font `%s`\n", name);
	return false;
}

bool TSEEUnloadAllFonts(TSEE *tsee) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *font = TSEEArrayGet(tsee->fonts, i);
		TTF_CloseFont(font->font);
		xfree(font->name);
		xfree(font);
	}
	TSEEDestroyArray(tsee->fonts);
	return true;
}

TTF_Font *TSEEGetFont(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *data = TSEEArrayGet(tsee->fonts, i);
		if (strcmp(data->name, name) == 0) {
			return data->font;
		}
	}
	TSEEWarn("Failed to find font `%s`\n", name);
	return NULL;
}

TSEE_Text *TSEECreateText(TSEE *tsee, char *fontName, char *text, SDL_Color color) {
	TSEE_Text *textObj = xmalloc(sizeof(*textObj));
	TTF_Font *font = TSEEGetFont(tsee, fontName);
	if (!font) {
		TSEEWarn("Failed to create text `%s` with font `%s` (Failed to get font)\n", text, fontName);
		return NULL;
	}
	textObj->text = strdup(text);
	textObj->texture = xmalloc(sizeof(*textObj->texture));
	textObj->texture->path = strdup("Rendered Text");
	SDL_Surface *surf = TTF_RenderText_Blended(font, text, color);
	if (!surf) {
		TSEEWarn("Failed to create text `%s` with font `%s` (Failed to create surface)\n", text, fontName);
		return NULL;
	}
	textObj->texture->texture = SDL_CreateTextureFromSurface(tsee->window->renderer, surf);
	textObj->texture->rect.x = 0;
	textObj->texture->rect.y = 0;
	SDL_QueryTexture(textObj->texture->texture, NULL, NULL, &textObj->texture->rect.w, &textObj->texture->rect.h);
	SDL_FreeSurface(surf);
	return textObj;
}

void TSEEDestroyText(TSEE_Text *text, bool destroyTexture) {
	if (destroyTexture) {
		TSEEDestroyTexture(text->texture);
	}
	xfree(text->text);
	xfree(text);
}

bool TSEERenderText(TSEE *tsee, TSEE_Text *text) {
	if (!text->texture) {
		TSEEWarn("Failed to render text `%s` (No texture)\n", text->text);
		return false;
	}
	if (SDL_RenderCopy(tsee->window->renderer, text->texture->texture, NULL, &text->texture->rect) != 0) {
		TSEEError("Failed to render text `%s` (%s)\n", text->text, SDL_GetError());
		return false;
	}
	return true;
}