#include "../tsee.h"

bool TSEE_Text_Init(TSEE *tsee, bool loadDefault) {
	if (tsee->init->text) {
		return true;
	}
	if (TTF_Init() == -1) {
		TSEE_Error("Failed to init SDL_TTF (%s)\n", TTF_GetError());
		return false;
	}
	tsee->fonts = TSEE_Array_Create();
	if (loadDefault) {
		if (TSEE_Font_Load(tsee, "fonts/default.ttf", 16, "_default")) {
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

bool TSEE_Font_Load(TSEE *tsee, char *path, int size, char *name) {
	TSEE_Font *font = xmalloc(sizeof(*font));
	font->font = TTF_OpenFont(path, size);
	if (font->font == NULL) {
		TSEE_Warn("Failed to load font `%s` (%s)\n", path, TTF_GetError());
		return false;
	}
	font->name = strdup(name);
	font->size = size;
	TSEE_Array_Append(tsee->fonts, font);
	return true;
}

bool TSEE_Font_Unload(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *font = tsee->fonts->data[i];
		if (strcmp(font->name, name) == 0) {
			TTF_CloseFont(font->font);
			xfree(font->name);
			xfree(font);
			TSEE_Array_Delete(tsee->fonts, i);
			return true;
		}
	}
	TSEE_Warn("Attempted to unload non-existant font `%s`\n", name);
	return false;
}

bool TSEE_Font_UnloadAll(TSEE *tsee) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *font = TSEE_Array_Get(tsee->fonts, i);
		TTF_CloseFont(font->font);
		xfree(font->name);
		xfree(font);
	}
	TSEE_Array_Destroy(tsee->fonts);
	return true;
}

TTF_Font *TSEE_Font_Get(TSEE *tsee, char *name) {
	for (size_t i = 0; i < tsee->fonts->size; i++) {
		TSEE_Font *data = TSEE_Array_Get(tsee->fonts, i);
		if (strcmp(data->name, name) == 0) {
			return data->font;
		}
	}
	TSEE_Warn("Failed to find font `%s`\n", name);
	return NULL;
}

TSEE_Text *TSEE_Text_Create(TSEE *tsee, char *fontName, char *text, SDL_Color color) {
	TSEE_Text *textObj = xmalloc(sizeof(*textObj));
	TTF_Font *font = TSEE_Font_Get(tsee, fontName);
	if (!font) {
		TSEE_Warn("Failed to create text `%s` with font `%s` (Failed to get font)\n", text, fontName);
		return NULL;
	}
	textObj->text = strdup(text);
	textObj->texture = xmalloc(sizeof(*textObj->texture));
	textObj->texture->path = strdup("Rendered Text");
	SDL_Surface *surf = TTF_RenderText_Blended(font, text, color);
	if (!surf) {
		TSEE_Warn("Failed to create text `%s` with font `%s` (Failed to create surface)\n", text, fontName);
		return NULL;
	}
	textObj->texture->texture = SDL_CreateTextureFromSurface(tsee->window->renderer, surf);
	textObj->texture->rect.x = 0;
	textObj->texture->rect.y = 0;
	SDL_QueryTexture(textObj->texture->texture, NULL, NULL, &textObj->texture->rect.w, &textObj->texture->rect.h);
	SDL_FreeSurface(surf);
	return textObj;
}

void TSEE_Text_Destroy(TSEE_Text *text, bool destroyTexture) {
	if (destroyTexture) {
		TSEE_Texture_Destroy(text->texture);
	}
	xfree(text->text);
	xfree(text);
}

bool TSEE_Text_Render(TSEE *tsee, TSEE_Text *text) {
	if (!text->texture) {
		TSEE_Warn("Failed to render text `%s` (No texture)\n", text->text);
		return false;
	}
	if (SDL_RenderCopy(tsee->window->renderer, text->texture->texture, NULL, &text->texture->rect) != 0) {
		TSEE_Error("Failed to render text `%s` (%s)\n", text->text, SDL_GetError());
		return false;
	}
	return true;
}