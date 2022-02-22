#include "include/tsee.h"

bool TSEEInitUI(TSEE *tsee) {
	if (!tsee->init->text) {
		TSEEError("Attempted to init TSEE UI without initialising TSEE Text.\n");
		return false;
	}
	if (SDL_SetRenderDrawBlendMode(tsee->window->renderer, SDL_BLENDMODE_BLEND) != 0) {
		TSEEWarn("Failed to set blend mode. Toolbar will appear solid\n%s", SDL_GetError());
	}
	tsee->ui->toolbar = TSEEArrayCreate();
	tsee->init->text = true;
	return true;
}

bool TSEEAddToolbarButton(TSEE *tsee, char *font, char *text) {
	TSEE_Toolbar_Object *toolbarobj = malloc(sizeof(*toolbarobj));
	toolbarobj->buttons = TSEEArrayCreate();
	if (!toolbarobj->buttons) {
		TSEEError("Failed to create toolbar button array.\n");
		free(toolbarobj);
		return false;
	}
	toolbarobj->text = TSEECreateText(tsee, font, text, (SDL_Color){255, 255, 255, 255});
	if (!toolbarobj->text) {
		TSEEError("Failed to create toolbar button texture\n");
		TSEEArrayFree(toolbarobj->buttons);
		free(toolbarobj);
		return false;
	}
	TSEEArrayAppend(tsee->ui->toolbar, toolbarobj);
	int idx = tsee->ui->toolbar->size - 1;
	int sizeBefore = 0;
	for (int i = 0; i < idx; i++) {
		sizeBefore += ((TSEE_Toolbar_Object *)TSEEArrayGet(tsee->ui->toolbar, i))->text->texture->rect.w + 16;
	}
	toolbarobj->text->texture->rect.x = sizeBefore + 8;
	toolbarobj->text->texture->rect.y = toolbarobj->text->texture->rect.h / 2;
	return true;
}

bool TSEEAddToolbarChild(TSEE *tsee, TSEE_Toolbar_Object *parent, char *font, char *text, void (*cb) (void *tsee)) {
	TSEE_Toolbar_Child *child = malloc(sizeof(*child));
	child->text = TSEECreateText(tsee, font, text, (SDL_Color){255, 255, 255, 255});
	if (!child->text) {
		TSEEError("Failed to create toolbar child texture\n");
		free(child);
		return false;
	}
	child->text->texture->rect = (SDL_Rect){parent->text->texture->rect.x + parent->text->texture->rect.w - child->text->texture->rect.w, 
											(parent->buttons->size + 1) * 32 + child->text->texture->rect.h / 2,
											child->text->texture->rect.w,
											child->text->texture->rect.h};
	child->callback = cb;
	TSEEArrayAppend(parent->buttons, child);
	return true;
}

bool TSEEUIClick(TSEE *tsee, int x, int y) {
	SDL_Point m = {x, y};
	for (size_t i = 0; i < tsee->ui->toolbar->size; i++) {
		TSEE_Toolbar_Object *toolbarobj = TSEEArrayGet(tsee->ui->toolbar, i);
		SDL_Rect fatRect = {toolbarobj->text->texture->rect.x - 8, 0, toolbarobj->text->texture->rect.w + 16, 32 + (toolbarobj->buttons->size * 32) * toolbarobj->expanded};
		if (SDL_PointInRect(&m, &fatRect)) {
			SDL_Rect fatCopy = (SDL_Rect){fatRect.x, 0, fatRect.w, 32};
			for (size_t j = 0; j < toolbarobj->buttons->size; j++) {
				TSEE_Toolbar_Child *child = TSEEArrayGet(toolbarobj->buttons, j);
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

bool TSEERenderUI(TSEE *tsee) {
	if (tsee->ui->toolbar->size > 0) {
		SDL_SetRenderDrawColor(tsee->window->renderer, 255, 255, 255, 75);
		SDL_RenderFillRect(tsee->window->renderer, &(SDL_Rect){0, 0, tsee->window->width, 32});
	}
	for (size_t i = 0; i < tsee->ui->toolbar->size; i++) {
		TSEE_Toolbar_Object *toolbarobj = TSEEArrayGet(tsee->ui->toolbar, i);
		SDL_Rect fatRect = {toolbarobj->text->texture->rect.x - 8, 0, toolbarobj->text->texture->rect.w + 16, 32 + (toolbarobj->buttons->size * 32) * toolbarobj->expanded};
		if (SDL_PointInRect(&tsee->window->mouse, &fatRect)) {
			// If hovered over
			toolbarobj->expanded = true;
			SDL_SetRenderDrawColor(tsee->window->renderer, 255, 255, 255, 75);
			SDL_Rect fatCopy = (SDL_Rect){fatRect.x, 0, fatRect.w, 32};
			SDL_RenderFillRect(tsee->window->renderer, &fatCopy);
			for (size_t j = 0; j < toolbarobj->buttons->size; j++) {
				fatCopy.y += 32;
				TSEE_Toolbar_Child *child = TSEEArrayGet(toolbarobj->buttons, j);
				SDL_SetRenderDrawColor(tsee->window->renderer, 255, 255, 255, 75);
				SDL_RenderFillRect(tsee->window->renderer, &fatCopy);
				if (SDL_PointInRect(&tsee->window->mouse, &fatCopy)) {
					// If hovered over
					SDL_SetRenderDrawColor(tsee->window->renderer, 255, 255, 255, 50);
					SDL_RenderFillRect(tsee->window->renderer, &fatCopy);
				}
				TSEERenderText(tsee, child->text);
			}
		} else {
			toolbarobj->expanded = false;
		}
		TSEERenderText(tsee, toolbarobj->text);
	}
	return true;
}