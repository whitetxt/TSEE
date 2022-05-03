#include "../tsee.h"

/**
 * @brief Perform a physics step
 * 
 * @param tsee TSEE to perform the step for
 */
void TSEE_Physics_PerformStep(TSEE *tsee) {
	Uint64 start = SDL_GetPerformanceCounter();
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *object = tsee->world->objects->data[i];
		if (TSEE_Object_CheckAttribute(object, TSEE_ATTRIB_PHYS_ENABLED)) {
			TSEE_Physics_UpdateObject(tsee, object);
		}
	}
	Uint64 end = SDL_GetPerformanceCounter();
	tsee->debug->physics_time += (end - start) * 1000/ (double) SDL_GetPerformanceFrequency();
}

/**
 * @brief Update a physics object
 * 
 * @param tsee TSEE object this object is in
 * @param obj Object to update
 */
void TSEE_Physics_UpdateObject(TSEE *tsee, TSEE_Object *obj) {
	TSEE_Vec2 grav = tsee->world->gravity;
	float dt = tsee->dt * 1000;
	TSEE_Log("DT: %f\n", dt);
	TSEE_Vec2_Multiply(&grav, obj->physics.mass);
	TSEE_Vec2_Add(&obj->physics.force, &grav);
	TSEE_Vec2_Divide(&obj->physics.force, obj->physics.mass * dt);
	TSEE_Vec2_Add(&obj->physics.velocity, &obj->physics.force);
	TSEE_Log("Force after div: %f, %f\n", obj->physics.force.x, obj->physics.force.y);

	TSEE_Vec2 new_vel = obj->physics.velocity;
	TSEE_Vec2_Multiply(&new_vel, dt);
	TSEE_Log("NewVel: %f, %f\n", new_vel.x, new_vel.y);
	TSEE_Vec2_Add(&obj->position, &new_vel);

	obj->physics.force.x = 0;
	obj->physics.force.y = 0;

	// Check collisions now that we have moved
	TSEE_Physics_CheckCollisions(tsee, obj);
}

/**
 * @brief Check for collisions with other objects
 * 
 * @param tsee TSEE to check for collisions in
 * @param obj Object to check collisions for
 */
void TSEE_Physics_CheckCollisions(TSEE *tsee, TSEE_Object *obj) {
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *other = tsee->world->objects->data[i];
		if (TSEE_Object_CheckAttribute(other, TSEE_ATTRIB_PARALLAX)) continue;

		if (other != obj) {
			if (!TSEE_IsRectNull( TSEE_Object_GetCollisionRect(obj, other) )) {
				TSEE_Physics_ResolveCollision(obj, other);
			}
		}
	}
}

/**
 * @brief Resolves a collision between two objects
 * 
 * @param first First object
 * @param second Second object
 */
void TSEE_Physics_ResolveCollision(TSEE_Object *first, TSEE_Object *second) {
	if (TSEE_Object_CheckAttribute(first, TSEE_ATTRIB_PHYS_ENABLED) && TSEE_Object_CheckAttribute(second, TSEE_ATTRIB_PHYS_ENABLED)) {
		// If this works Github Copilot is good
		TSEE_Vec2 first_vel = first->physics.velocity;
		TSEE_Vec2 second_vel = second->physics.velocity;

		TSEE_Vec2 first_vel_norm = first_vel;
		TSEE_Vec2_Normalise(&first_vel_norm);
		TSEE_Vec2 second_vel_norm = second_vel;
		TSEE_Vec2_Normalise(&second_vel_norm);

		// Get the relative velocity
		TSEE_Vec2 rel_vel = first_vel;
		TSEE_Vec2_Subtract(&rel_vel, second_vel);

		// Check if they're moving away from each other
		if (TSEE_Vec2_Dot(rel_vel, first_vel_norm) > 0) {
			return;
		}

		// Get the overlap
		TSEE_Vec2 overlap = first->physics.velocity;
		TSEE_Vec2_Subtract(&overlap, second->physics.velocity);
		TSEE_Vec2_Multiply(&overlap, 0.5);

		// Apply the overlap to the objects
		TSEE_Vec2_Add(&first->physics.velocity, &overlap);
		TSEE_Vec2_Subtract(&second->physics.velocity, overlap);
	} else {
		int amtRight = fabs(first->position.x + first->texture->rect.w - second->position.x);
		int amtLeft = fabs(second->position.x + second->texture->rect.w - first->position.x);
		int amtTop = fabs(second->position.y - first->position.y + first->texture->rect.h);
		int amtBottom = fabs(first->position.y + first->texture->rect.h - second->position.y);

		int values[4] = {amtRight, amtLeft, amtTop, amtBottom};
		int lowest = values[0];
		// Get lowest value, side it collided on
		for (int x = 0; x < 4; x++) {
			if (values[x] < lowest) {
				lowest = values[x];
			}
		}

		if (TSEE_Object_CheckAttribute(first, TSEE_ATTRIB_PHYS_ENABLED)) {
			if (lowest == amtRight)
				TSEE_Object_SetPosition(first, second->position.x - first->texture->rect.w, first->position.y);
			else if (lowest == amtLeft)
				TSEE_Object_SetPosition(first, second->position.x + second->texture->rect.w, first->position.y);
			else if (lowest == amtTop)
				TSEE_Object_SetPosition(first, first->position.x, second->position.y + second->texture->rect.h);
			else if (lowest == amtBottom)
				TSEE_Object_SetPosition(first, first->position.x, second->position.y - first->texture->rect.h);
		} else {
			if (lowest == amtRight)
				TSEE_Object_SetPosition(second, first->position.x - second->texture->rect.w, second->position.y);
			else if (lowest == amtLeft)
				TSEE_Object_SetPosition(second, first->position.x + first->texture->rect.w, second->position.y);
			else if (lowest == amtTop)
				TSEE_Object_SetPosition(second, second->position.x, first->position.y + first->texture->rect.h);
			else if (lowest == amtBottom)
				TSEE_Object_SetPosition(second, second->position.x, first->position.y - second->texture->rect.h);
		}
	}
}