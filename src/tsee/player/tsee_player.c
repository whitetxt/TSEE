#include "../tsee.h"

/**
 * @brief Sets the player's speed
 * 
 * @param tsee TSEE object which the player is in
 * @param speed New speed of the player
 */
void TSEE_Player_SetSpeed(TSEE *tsee, float speed) {
	tsee->player->speed = speed;
}

/**
 * @brief Sets the player's jump force
 * 
 * @param tsee TSEE object which the player is in
 * @param jump_force New jump force of the player
 */
void TSEE_Player_SetJumpForce(TSEE *tsee, float jump_force) {
	tsee->player->jump_force = jump_force;
}