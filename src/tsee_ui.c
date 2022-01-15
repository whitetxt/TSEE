#include "include/tsee.h"

bool TSEEInitUI(TSEE *tsee) {
	if (!tsee->init->text) {
		TSEEError("Attempted to init TSEE UI without initialising TSEE Text.\n");
		return false;
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
	toolbarobj->text->texture->rect.x = sizeBefore;
	toolbarobj->text->texture->rect.y = 0;
	return true;
}

bool TSEERenderUI(TSEE *tsee) {
	if (tsee->ui->toolbar->size > 0) {
		SDL_RenderDrawRect(tsee->window->renderer, &(SDL_Rect){0, 0, tsee->window->width, 32});
	}
	for (size_t i = 0; i < tsee->ui->toolbar->size; i++) {
		TSEE_Toolbar_Object *toolbarobj = TSEEArrayGet(tsee->ui->toolbar, i);
		TSEERenderText(tsee, toolbarobj->text);
	}
	return true;
}