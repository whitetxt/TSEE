#include "../tsee.h"

bool TSEE_UI_Init(TSEE *tsee) {
	if (!tsee->init->text) {
		TSEE_Error("Attempted to init TSEE UI without initialising TSEE Text.\n");
		return false;
	}
	if (SDL_SetRenderDrawBlendMode(tsee->window->renderer, SDL_BLENDMODE_BLEND) != 0) {
		TSEE_Warn("Failed to set blend mode. Toolbar will appear solid\n%s", SDL_GetError());
	}
	tsee->ui->toolbar = TSEE_Array_Create();
	//tsee->ui->toolbar_enabled = false;
	tsee->init->text = true;
	return true;
}

bool TSEE_Toolbar_AddButton(TSEE *tsee, char *font, char *text) {
	TSEE_Toolbar_Object *toolbarobj = xmalloc(sizeof(*toolbarobj));
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

bool TSEE_Toolbar_AddChild(TSEE *tsee, char *parentName, char *font, char *text, void (*cb) (void *tsee)) {
	TSEE_Toolbar_Object *parent = NULL;
	for (size_t i = 0; i < tsee->ui->toolbar->size; i++) {
		TSEE_Toolbar_Object *obj = TSEE_Array_Get(tsee->ui->toolbar, i);
		if (strcmp(obj->text->text, parentName) == 0) {
			parent = obj;
			break;
		}
	}
	if (!parent) {
		TSEE_Error("Failed to find toolbar button with name `%s`\n", parentName);
	}
	TSEE_Toolbar_Child *child = xmalloc(sizeof(*child));
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

bool TSEE_UI_Render(TSEE *tsee) {
	//if (tsee->ui->toolbar_enabled) {
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
					TSEE_Text_Render(tsee, child->text);
				}
			} else {
				toolbarobj->expanded = false;
			}
			TSEE_Text_Render(tsee, toolbarobj->text);
		}
	//}
	return true;
}