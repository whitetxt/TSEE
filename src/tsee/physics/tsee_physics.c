#include "../tsee.h"

/**
 * @brief Perform a physics step
 * 
 * @param tsee TSEE to perform the step for
 */
void TSEE_Physics_PerformStep(TSEE *tsee) {
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *object = tsee->world->objects->data[i];
		if (TSEE_Object_CheckAttribute(object, TSEE_ATTRIB_PHYS_ENABLED)) {
			TSEE_Physics_UpdateObject(tsee, object);
		}
	}
}

/**
 * @brief Update a physics object
 * 
 * @param tsee TSEE object this object is in
 * @param obj Object to update
 */
void TSEE_Physics_UpdateObject(TSEE *tsee, TSEE_Object *obj) {
	TSEE_Vec2 grav = TSEE_Vec2_Copy(tsee->world->gravity);
	TSEE_Vec2_Multiply(&grav, obj->physics.mass);
	TSEE_Vec2_Add(&obj->physics.force, &grav);
	TSEE_Vec2_Divide(&obj->physics.force, obj->physics.mass * tsee->dt);

	TSEE_Vec2 new_vel = TSEE_Vec2_Copy(obj->physics.velocity);
	TSEE_Vec2_Multiply(&new_vel, tsee->dt);
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
		if (other != obj) {
			if (!TSEE_IsRectNull( TSEE_Object_GetCollisionRect(obj, other) )) {
				TSEE_Physics_ResolveCollision(obj, other);
			}
		}
	}
}