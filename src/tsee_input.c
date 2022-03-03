#include "include/tsee.h"

bool TSEEInitInput(TSEE *tsee) {
	if (tsee->init->input) {
		return true;
	}
	if (!tsee->init->events) {
		TSEEWarn("Attempted to inititalise TSEEInput without initialising TSEEEvents");
		return false;
	}
	tsee->events->keypress = TSEEKeyPress;
	tsee->events->keyrelease = TSEEKeyRelease;
	tsee->init->input = true;
	return true;
}

void TSEEKeyPress(void *tsee, SDL_Keycode keycode) {
	TSEE *tseeReal = tsee;
	switch (keycode) {
		case SDLK_w:
			tseeReal->player->movement.up = true;
			break;
		case SDLK_s:
			tseeReal->player->movement.down = true;
			break;
		case SDLK_a:
			tseeReal->player->movement.left = true;
			break;
		case SDLK_d:
			tseeReal->player->movement.right = true;
			break;
		case SDLK_F3:
			tseeReal->debug->active = !tseeReal->debug->active;
			break;
		case SDLK_ESCAPE:
			tseeReal->ui->toolbar_enabled = !tseeReal->ui->toolbar_enabled;
			break;
	}
	TSEELog("Movement (WASD): %d, %d, %d, %d\n", tseeReal->player->movement.up, tseeReal->player->movement.left, tseeReal->player->movement.down, tseeReal->player->movement.right);
}

void TSEEKeyRelease(void *tsee, SDL_Keycode keycode) {
	TSEE *tseeReal = tsee;
	switch (keycode) {
		case SDLK_w:
			tseeReal->player->movement.up = false;
			break;
		case SDLK_s:
			tseeReal->player->movement.down = false;
			break;
		case SDLK_a:
			tseeReal->player->movement.left = false;
			break;
		case SDLK_d:
			tseeReal->player->movement.right = false;
			break;
	}
	TSEELog("Movement (WASD): %d, %d, %d, %d\n", tseeReal->player->movement.up, tseeReal->player->movement.left, tseeReal->player->movement.down, tseeReal->player->movement.right);
}