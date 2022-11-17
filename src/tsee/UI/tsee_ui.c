#include "../tsee.h"

/**
 * @brief Initialises the UI subsystem.
 * 
 * @param tsee TSEE object to initialise UI for.
 * @return success status
 */
bool TSEE_UI_Init(TSEE *tsee) {
	if (!tsee->init->text) {
		TSEE_Error("Attempted to init TSEE UI without initialising TSEE Text.\n");
		return false;
	}
	if (SDL_SetRenderDrawBlendMode(tsee->window->renderer, SDL_BLENDMODE_BLEND) != 0) {
		TSEE_Warn("Failed to set blend mode. Toolbar will appear solid\n%s", SDL_GetError());
	}
	tsee->ui->toolbar = TSEE_Array_Create();
	tsee->ui->toolbar_enabled = false;
	tsee->init->ui = true;
	return true;
}

/**
 * @brief Adds a button to the toolbar of a TSEE object.
 * 
 * @param tsee TSEE to add the button to.
 * @param font Font to use for the button.
 * @param text Text for the button.
 * @return success status
 */
bool TSEE_Toolbar_AddButton(TSEE *tsee, char *font, char *text) {
	TSEE_Toolbar_Object *toolbarobj = xmalloc(sizeof(*toolbarobj));
	if (!toolbarobj) {
		TSEE_Error("Failed to malloc for the toolbar.\n");
		return false;
	}
	toolbarobj->buttons = TSEE_Array_Create();
	if (!toolbarobj->buttons) {
		TSEE_Error("Failed to create toolbar button array.\n");
		xfree(toolbarobj);
		return false;
	}
	toolbarobj->text = TSEE_Text_Create(tsee, font, text, (SDL_Color){255, 255, 255, 255});
	if (!toolbarobj->text) {
		TSEE_Error("Failed to create toolbar button texture\n");
		TSEE_Array_Destroy(toolbarobj->buttons);
		xfree(toolbarobj);
		return false;
	}
	TSEE_Array_Append(tsee->ui->toolbar, toolbarobj);
	int idx = tsee->ui->toolbar->size - 1;
	int sizeBefore = 0;
	for (int i = 0; i < idx; i++) {
		sizeBefore += ((TSEE_Toolbar_Object *)TSEE_Array_Get(tsee->ui->toolbar, i))->text->texture->rect.w + 16;
	}
	toolbarobj->text->texture->rect.x = sizeBefore + 8;
	toolbarobj->text->texture->rect.y = toolbarobj->text->texture->rect.h / 2;
	return true;
}

/**
 * @brief Adds a child button to a top-level button in the toolbar.
 * 
 * @param tsee TSEE object to add the child to.
 * @param parentName Name of the parent (top-level) button.
 * @param font Font to use for the text.
 * @param text Text to display on the child.
 * @param cb Callback to use when the button is clicked.
 * @return success status
 */
bool TSEE_Toolbar_AddChild(TSEE *tsee, char *parentName, char *font, char *text, void (*cb) (void *tsee)) {
	TSEE_Toolbar_Object *parent = NULL;
	for (size_t i = 0; i < tsee->ui->toolbar->size; i++) {
		TSEE_Toolbar_Object *obj = TSEE_Array_Get(tsee->ui->toolbar, i);
		if (strcmp(obj->text->text.text, parentName) == 0) {
			parent = obj;
			break;
		}
	}

	if (!parent) {
		TSEE_Error("Failed to find toolbar button with name `%s`\n", parentName);
		return false;
	}

	TSEE_Toolbar_Child *child = xmalloc(sizeof(*child));
	if (!child) {
		TSEE_Error("Failed to malloc for toolbar child.\n");
		return false;
	}
	if (strcmp(font, "") == 0) font = "_default";

	child->text = TSEE_Text_Create(tsee, font, text, (SDL_Color){255, 255, 255, 255});
	if (!child->text) {
		TSEE_Error("Failed to create toolbar child texture\n");
		xfree(child);
		return false;
	}

	child->text->texture->rect = (SDL_Rect){parent->text->texture->rect.x + parent->text->texture->rect.w / 2 - child->text->texture->rect.w / 2, 
											(parent->buttons->size + 1) * 32 + child->text->texture->rect.h / 2,
											child->text->texture->rect.w,
											child->text->texture->rect.h};
	child->callback = cb;
	TSEE_Array_Append(parent->buttons, child);
	return true;
}

/**
 * @brief Handles if a button is clicked when the mouse is pressed.
 * 
 * @param tsee TSEE object to check against.
 * @param x X position of the mouse.
 * @param y Y position of the mouse.
 * @return success status
 */
bool TSEE_UI_Click(TSEE *tsee, int x, int y) {
	SDL_Point m = {x, y};
	for (size_t i = 0; i < tsee->ui->toolbar->size; i++) {
		TSEE_Toolbar_Object *toolbarobj = TSEE_Array_Get(tsee->ui->toolbar, i);
		SDL_Rect fatRect = {toolbarobj->text->texture->rect.x - 8, 0, toolbarobj->text->texture->rect.w + 16, 32 + (toolbarobj->buttons->size * 32) * toolbarobj->expanded};
		if (SDL_PointInRect(&m, &fatRect)) {
			SDL_Rect fatCopy = (SDL_Rect){fatRect.x, 0, fatRect.w, 32};
			for (size_t j = 0; j < toolbarobj->buttons->size; j++) {
				TSEE_Toolbar_Child *child = TSEE_Array_Get(toolbarobj->buttons, j);
				fatCopy.y += 32;
				if (SDL_PointInRect(&m, &fatCopy)) {
					child->callback(tsee);
					return true;
				}
			}
		}
	}
	return false;
}

/**
 * @brief Renders the UI of a TSEE object.
 * 
 * @param tsee TSEE object to render the UI of.
 * @return success status
 */
bool TSEE_UI_Render(TSEE *tsee) {
	if (tsee->ui->toolbar_enabled) {
		if (tsee->ui->toolbar->size > 0) {
			SDL_SetRenderDrawColor(tsee->window->renderer, 255, 255, 255, 75);
			SDL_RenderFillRect(tsee->window->renderer, &(SDL_Rect){0, 0, tsee->window->width, 32});
		}
		for (size_t i = 0; i < tsee->ui->toolbar->size; i++) {
			TSEE_Toolbar_Object *toolbarobj = TSEE_Array_Get(tsee->ui->toolbar, i);
			SDL_Rect fatRect = {toolbarobj->text->texture->rect.x - 8, 0, toolbarobj->text->texture->rect.w + 16, 32 + (toolbarobj->buttons->size * 32) * toolbarobj->expanded};
			if (SDL_PointInRect(&tsee->window->mouse, &fatRect)) {
				// If hovered over
				toolbarobj->expanded = true;
				SDL_SetRenderDrawColor(tsee->window->renderer, 255, 255, 255, 75);
				SDL_Rect fatCopy = (SDL_Rect){fatRect.x, 0, fatRect.w, 32};
				SDL_RenderFillRect(tsee->window->renderer, &fatCopy);
				for (size_t j = 0; j < toolbarobj->buttons->size; j++) {
					fatCopy.y += 32;
					TSEE_Toolbar_Child *child = TSEE_Array_Get(toolbarobj->buttons, j);
					SDL_SetRenderDrawColor(tsee->window->renderer, 255, 255, 255, 75);
					SDL_RenderFillRect(tsee->window->renderer, &fatCopy);
					if (SDL_PointInRect(&tsee->window->mouse, &fatCopy)) {
						// If hovered over
						SDL_SetRenderDrawColor(tsee->window->renderer, 255, 255, 255, 50);
						SDL_RenderFillRect(tsee->window->renderer, &fatCopy);
					}
					TSEE_Object_Render(tsee, child->text);
				}
			} else {
				toolbarobj->expanded = false;
			}
			TSEE_Object_Render(tsee, toolbarobj->text);
		}
	}
	return true;
}