#include "../tsee.h"

/**
 * @brief Initialises the text subsystem.
 * 
 * @param tsee TSEE object to initialise text for.
 * @param loadDefault True to load the default font, false to not.
 * @return true on success, false on fail.
 */
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

/**
 * @brief Creates a text object with from a font, text and colour.
 * 
 * @param tsee TSEE object to create the text for.
 * @param fontName Name of the font to use.
 * @param text Text to display.
 * @param color Colour of the text.
 * @return TSEE_Text* 
 */
TSEE_Object *TSEE_Text_Create(TSEE *tsee, char *fontName, char *text, SDL_Color color) {
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

/**
 * @brief Destroy a text object.
 * 
 * @param text Text object to destroy.
 * @param destroyTexture True to destroy the texture, false to not.
 */
void TSEE_Text_Destroy(TSEE_Text *text, bool destroyTexture) {
	if (destroyTexture) {
		TSEE_Texture_Destroy(text->texture);
	}
	xfree(text->text);
	xfree(text);
}

/**
 * @brief Render a text object to a TSEE object.
 * 
 * @param tsee TSEE object to render the text to.
 * @param text Text object to render.
 * @return true on success, false on fail.
 */
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