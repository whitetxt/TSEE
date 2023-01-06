#include "../tsee.h"

/**
 * @brief Initialises the text subsystem.
 *
 * @param loadDefault True to load the default font, false to not.
 * @return success status
 */
bool TSEE_Text_Init(TSEE *tsee, bool loadDefault) {
	if (tsee->init->text)
		return true;

	if (TTF_Init() == -1) {
		TSEE_Error("Failed to init SDL_TTF (%s)\n", TTF_GetError());
		return false;
	}
	if (loadDefault) {
		tsee->init->text =
			TSEE_Font_Load(tsee, "fonts/default.ttf", 16, "_default");
		if (!tsee->init->text)
			TSEE_Error("Failed to load default font.\n");

		return tsee->init->text;
	}
	tsee->init->text = true;
	return true;
}

/**
 * @brief Creates a text object with from a font, text and colour.
 *
 * @param fontName Name of the font to use.
 * @param text Text to display.
 * @param color Colour of the text.
 * @return TSEE_Object*
 */
TSEE_Object *TSEE_Text_Create(TSEE *tsee,
							  char *fontName,
							  char *text,
							  SDL_Color color) {
	TSEE_Object *textObj = xmalloc(sizeof(*textObj));
	if (!textObj) {
		TSEE_Error("Failed to malloc for text object.\n");
		return NULL;
	}
	TSEE_Font *font = TSEE_Font_Get(tsee, fontName);
	if (!font) {
		TSEE_Warn(
			"Failed to create text `%s` with font `%s` (Failed to get font)\n",
			text, fontName);
		xfree(textObj);
		return NULL;
	}
	textObj->text.text = strdup(text);
	textObj->texture = xmalloc(sizeof(*textObj->texture));
	if (!textObj->texture) {
		TSEE_Error("Failed to malloc for text object's texture.\n");
		xfree(textObj);
		return NULL;
	}
	textObj->texture->path = NULL;
	SDL_Surface *surf = TTF_RenderText_Blended(font->font, text, color);
	if (!surf) {
		TSEE_Warn(
			"Failed to create text `%s` with font `%s` (Failed to create "
			"surface)\n",
			text, fontName);
		xfree(textObj->texture);
		xfree(textObj);
		return NULL;
	}
	textObj->texture->texture =
		SDL_CreateTextureFromSurface(tsee->window->renderer, surf);
	textObj->texture->rect.x = 0;
	textObj->texture->rect.y = 0;
	SDL_QueryTexture(textObj->texture->texture, NULL, NULL,
					 &textObj->texture->rect.w, &textObj->texture->rect.h);
	SDL_FreeSurface(surf);
	textObj->attributes = TSEE_ATTRIB_TEXT | TSEE_ATTRIB_UI;
	textObj->position = VEC_ZERO;
	TSEE_Resource_Texture_Store(tsee, textObj->texture);
	return textObj;
}

/**
 * @brief Destroy a text object.
 *
 * @param text Text object to destroy.
 * @param destroyTexture True to destroy the texture, false to not.
 */
void TSEE_Text_Destroy(TSEE *tsee, TSEE_Object *text, bool destroyTexture) {
	if (!TSEE_Object_CheckAttribute(text, TSEE_ATTRIB_TEXT))
		return;
	if (destroyTexture) {
		TSEE_Texture_Destroy(text->texture);
		TSEE_Resource_Texture_Delete(tsee, text->texture);
	}
	xfree(text->text.text);
	xfree(text);
}

/**
 * @brief Render a text object to a TSEE object.
 *
 * @param text Text object to render.
 * @return success status
 */
bool TSEE_Text_Render(TSEE *tsee, TSEE_Object *text) {
	if (!TSEE_Object_CheckAttribute(text, TSEE_ATTRIB_TEXT))
		return false;
	if (!text->texture) {
		TSEE_Warn("Failed to render text `%s` (No texture)\n", text->text.text);
		return false;
	}
	if (SDL_RenderCopy(tsee->window->renderer, text->texture->texture, NULL,
					   &text->texture->rect) != 0) {
		TSEE_Error("Failed to render text `%s` (%s)\n", text->text.text,
				   SDL_GetError());
		return false;
	}
	return true;
}