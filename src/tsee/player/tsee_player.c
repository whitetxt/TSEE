#include "../tsee.h"

/**
 * @brief Sets the player's speed in u/s
 * 
 * @param tsee TSEE object which the player is in
 * @param speed New speed of the player
 */
void TSEE_Player_SetSpeed(TSEE *tsee, float speed) {
	tsee->player->speed = speed;
}

/**
 * @brief Sets the player's jump force as a multiple of the gravity.
 * 
 * @param tsee TSEE object which the player is in
 * @param jump_force New jump force of the player
 */
void TSEE_Player_SetJumpForce(TSEE *tsee, float jump_force) {
	tsee->player->jump_force = jump_force;
}

void TSEE_Player_HandleInput(TSEE *tsee) {
	TSEE_Player *player = tsee->player;
	if (!player->object) return;

	TSEE_Vec2_Add(&player->object->physics.force, (TSEE_Vec2){(player->movement.right - player->movement.left) * player->speed, 0});

	if (player->movement.up && player->grounded) {
		TSEE_Vec2 grav = tsee->world->gravity;
		TSEE_Vec2_Multiply(&grav, player->jump_force);
		TSEE_Vec2_Multiply(&grav, -1);
		TSEE_Vec2_Add(&player->object->physics.force, grav);
		TSEE_Log("Held Up: %f\n", player->held_up);
		if (player->held_up > 0.1f) {
			player->grounded = false;
		}
		player->held_up += tsee->dt;
	} else {
		if (!player->movement.up) {
			player->grounded = false;
		}
		player->held_up = 0;
	}
}