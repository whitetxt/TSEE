#include "../tsee.h"

/**
 * @brief Initialise the input subsystem.
 *
 * @param tsee TSEE object to initialise input for.
 * @return success status
 */
bool TSEE_Input_Init(TSEE *tsee) {
	if (tsee->init->input) {
		return true;
	}
	if (!tsee->init->events) {
		TSEE_Warn(
			"Attempted to inititalise TSEEInput without initialising "
			"TSEEEvents");
		return false;
	}
	tsee->events->keypress = TSEE_Key_Press;
	tsee->events->keyrelease = TSEE_Key_Release;
	tsee->init->input = true;
	return true;
}

/**
 * @brief Callback for key press events.
 *
 * @param tsee TSEE object to update.
 * @param keycode Keycode for the pressed button.
 */
void TSEE_Key_Press(void *tsee, SDL_Keycode keycode) {
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
}

/**
 * @brief Callback for the key release event.
 *
 * @param tsee TSEE object to update.
 * @param keycode Keycode for the released button.
 */
void TSEE_Key_Release(void *tsee, SDL_Keycode keycode) {
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
}