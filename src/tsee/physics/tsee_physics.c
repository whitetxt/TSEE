#include "../tsee.h"

/**
 * @brief Sets an object's mass, also calculating its inverse mass.
 *
 * @param obj Object to change
 * @param mass New mass
 */
void TSEE_Physics_SetObjectMass(TSEE_Object *obj, double mass) {
	if (!TSEE_Object_CheckAttribute(obj, TSEE_ATTRIB_PHYS))
		return;	 // Ignore objects which are not physics objects.
	obj->physics.mass = mass;
	obj->physics.inv_mass = 1 / mass;
}

/**
 * @brief Perform a physics step
 */
void TSEE_Physics_PerformStep(TSEE *tsee) {
	Uint64 start = SDL_GetPerformanceCounter();
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *object = TSEE_Array_Get(tsee->world->objects, i);
		if (TSEE_Object_CheckAttribute(object, TSEE_ATTRIB_PHYS)) {
			TSEE_Physics_UpdateObject(tsee, object);
			// Check collisions now that we have moved
			for (size_t j = i + 1; j < tsee->world->objects->size; j++) {
				TSEE_Object *other = tsee->world->objects->data[j];

				TSEE_Manifold manifold =
					(TSEE_Manifold){object, other, {0, 0}, -1};
				if (!TSEE_Physics_GetAABBAABBManifold(&manifold))
					continue;

				TSEE_Physics_ResolveCollision(tsee, &manifold);
				TSEE_Physics_PositionalCorrection(&manifold);
			}
		}
	}
	if (tsee->player->object) {
		TSEE_World_ScrollToObject(tsee, tsee->player->object);
	}
	Uint64 end = SDL_GetPerformanceCounter();
	tsee->debug->physics_time +=
		(end - start) * 1000 / (double)SDL_GetPerformanceFrequency();
}

/**
 * @brief Update a physics object
 *
 * @param obj Object to update
 */
void TSEE_Physics_UpdateObject(TSEE *tsee, TSEE_Object *obj) {
	// Apply gravity
	// F = ma
	TSEE_Vec2 grav = tsee->world->gravity;
	double dt = tsee->dt;

	// a * m
	TSEE_Vec2_Multiply(&grav, obj->physics.mass);

	// F += ma
	TSEE_Vec2_Add(&obj->physics.force, grav);

	// F = ma
	// a = F / m
	// a = F * (1 / m)
	TSEE_Vec2 accel = obj->physics.force;
	TSEE_Vec2_Multiply(&accel, obj->physics.inv_mass);

	TSEE_Vec2_Multiply(&accel, dt);
	// Euler Explicit
	TSEE_Vec2_Add(&obj->physics.velocity, accel);

	TSEE_Vec2 vel = obj->physics.velocity;
	TSEE_Vec2_Multiply(&vel, dt);

	TSEE_Vec2 pos = obj->position;
	TSEE_Vec2_Add(&pos, vel);
	// TSEE_Log("Velocity: %f, %f\n", vel.x, vel.y);
	// TSEE_Log("Position: %f, %f\n", pos.x, pos.y);

	TSEE_Object_SetPositionVec2(tsee, obj, pos);

	obj->physics.force = (TSEE_Vec2){0, 0};

	// "friction"
	obj->physics.velocity.y *= 0.99;
	obj->physics.velocity.x *= 0.975;
}

/**
 * @brief Check for collisions with other objects
 *
 * @param obj Object to check collisions for
 *
 * @return The object collided with or NULL
 */
TSEE_Object *TSEE_Physics_CheckCollisions(TSEE *tsee, TSEE_Object *obj) {
	if (TSEE_Object_CheckAttribute(obj, TSEE_ATTRIB_PARALLAX))
		return NULL;
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *other = tsee->world->objects->data[i];
		if (other == obj)
			continue;

		if (!TSEE_IsRectNull(TSEE_Object_GetCollisionRect(obj, other))) {
			return other;
		}
	}
	return NULL;
}

/**
 * @brief Corrects for floating point errors in physics calculations.
 *
 * @param manifold Manifold to correct.
 */
void TSEE_Physics_PositionalCorrection(TSEE_Manifold *manifold) {
	TSEE_Object *A = manifold->a;
	TSEE_Object *B = manifold->b;
	float percent = 0.2;  // usually 20% to 80%
	float slop = 0.01;	  // usually 0.01 to 0.1
	TSEE_Vec2 correction = TSEE_Vec2_RMultiply(
		manifold->normal, max(manifold->depth - slop, 0.0f) /
							  (A->physics.inv_mass + B->physics.inv_mass) *
							  percent);
	TSEE_Vec2_Subtract(&A->position,
					   TSEE_Vec2_RMultiply(correction, A->physics.inv_mass));
	TSEE_Vec2_Add(&B->position,
				  TSEE_Vec2_RMultiply(correction, B->physics.inv_mass));
}

/**
 * @brief Generates a manifold for an AABB-AABB Collision.
 *
 * @param m Manifold to generate, with the objects already pointed to.
 * @return bool
 */
bool TSEE_Physics_GetAABBAABBManifold(TSEE_Manifold *m) {
	TSEE_Object *a = m->a;
	TSEE_Object *b = m->b;

	if (TSEE_IsRectNull(TSEE_Object_GetCollisionRect(a, b)))
		return false;

	float amtRight = fabs(a->position.x + a->texture->rect.w - b->position.x);
	float amtLeft = fabs(b->position.x + b->texture->rect.w - a->position.x);
	float amtTop = fabs(b->position.y - a->position.y + a->texture->rect.h);
	float amtBottom = fabs(a->position.y + a->texture->rect.h - b->position.y);

	float values[4] = {amtRight, amtLeft, amtTop, amtBottom};
	float lowest = values[0];
	// Get lowest value, side it collided on
	for (int x = 1; x < 4; x++) {
		if (values[x] < lowest) {
			lowest = values[x];
		}
	}

	if (lowest < 0)
		return false;
	m->depth = lowest;
	if (lowest == amtRight) {
		m->normal = (TSEE_Vec2){1, 0};
	} else if (lowest == amtLeft) {
		m->normal = (TSEE_Vec2){-1, 0};
	} else if (lowest == amtTop) {
		if (TSEE_Object_CheckAttribute(a, TSEE_ATTRIB_PLAYER)) {
			a->is_grounded = true;
		}
		m->normal = (TSEE_Vec2){0, -1};
	} else if (lowest == amtBottom) {
		if (TSEE_Object_CheckAttribute(b, TSEE_ATTRIB_PLAYER)) {
			b->is_grounded = true;
		}
		m->normal = (TSEE_Vec2){0, 1};
	}
	return true;
}

/**
 * @brief Returns if a manifold is none.
 *
 * @param man manifold to check
 * @return if the manifold is none
 */
bool TSEE_Manifold_IsNone(TSEE_Manifold man) {
	return man.depth <= 0;
}

/**
 * @brief Resolves a collision between two objects
 *
 * @param a First object
 * @param b Second object
 */
void TSEE_Physics_ResolveCollision(TSEE *tsee, TSEE_Manifold *manifold) {
	TSEE_Object *a = manifold->a;
	TSEE_Object *b = manifold->b;
	if (true || (TSEE_Object_CheckAttribute(a, TSEE_ATTRIB_PHYS) &&
				 TSEE_Object_CheckAttribute(b, TSEE_ATTRIB_PHYS))) {
		// NEW METHOD!!
		TSEE_Vec2 normal = manifold->normal;
		TSEE_Vec2 rv =
			TSEE_Vec2_RSubtract(b->physics.velocity, a->physics.velocity);
		float velAlongNorm = TSEE_Vec2_Dot(rv, normal);
		if (velAlongNorm > 0)
			return;
		// e is resititution
		float e = min(a->physics.restitution, b->physics.restitution);
		e = 1;
		// j is the impulse scalar
		float j = -(1 + e) * velAlongNorm;
		j /= (a->physics.inv_mass + b->physics.inv_mass);

		// Apply impulse
		TSEE_Vec2 impulse = TSEE_Vec2_RMultiply(normal, j);
		float mass_sum = a->physics.mass + b->physics.mass;
		float ratio = a->physics.mass / mass_sum;
		TSEE_Vec2_Subtract(&a->physics.velocity,
						   TSEE_Vec2_RMultiply(impulse, ratio));
		ratio = b->physics.mass / mass_sum;
		TSEE_Vec2_Add(&b->physics.velocity,
					  TSEE_Vec2_RMultiply(impulse, ratio));
		return;
	}
	int amtRight = fabs(a->position.x + a->texture->rect.w - b->position.x);
	int amtLeft = fabs(b->position.x + b->texture->rect.w - a->position.x);
	int amtTop = fabs(b->position.y - a->position.y + a->texture->rect.h);
	int amtBottom = fabs(a->position.y + a->texture->rect.h - b->position.y);

	int values[4] = {amtRight, amtLeft, amtTop, amtBottom};
	int lowest = values[0];
	// Get lowest value, side it collided on
	for (int x = 1; x < 4; x++) {
		if (values[x] < lowest) {
			lowest = values[x];
		}
	}

	if (TSEE_Object_CheckAttribute(a, TSEE_ATTRIB_PHYS)) {
		if (lowest == amtRight) {
			if (amtTop <= tsee->player->step_size) {
				TSEE_Object_SetPosition(tsee, a, a->position.x,
										b->position.y + a->texture->rect.h);
			} else {
				TSEE_Object_SetPosition(
					tsee, a, b->position.x - a->texture->rect.w, a->position.y);
				a->physics.velocity.x = 0;
			}
		} else if (lowest == amtLeft) {
			if (amtTop <= tsee->player->step_size) {
				TSEE_Object_SetPosition(tsee, a, a->position.x,
										b->position.y + a->texture->rect.h);
			} else {
				TSEE_Object_SetPosition(
					tsee, a, b->position.x + b->texture->rect.w, a->position.y);
				a->physics.velocity.x = 0;
			}
		} else if (lowest == amtTop) {
			TSEE_Object_SetPosition(tsee, a, a->position.x,
									b->position.y + a->texture->rect.h);
			if (a == tsee->player->object) {
				tsee->player->grounded = true;
			}
			if (a->physics.velocity.y < 0) {
				a->physics.velocity.y = 0;
			}
		} else if (lowest == amtBottom) {
			TSEE_Object_SetPosition(tsee, a, a->position.x,
									b->position.y - b->texture->rect.h);
			if (a->physics.velocity.y > 0) {
				a->physics.velocity.y = 0;
			}
		}
	} else {
		if (lowest == amtRight) {
			if (amtTop <= tsee->player->step_size) {
				TSEE_Object_SetPosition(tsee, b, b->position.x,
										a->position.y + b->texture->rect.h);
			} else {
				TSEE_Object_SetPosition(
					tsee, b, a->position.x - b->texture->rect.w, b->position.y);
				b->physics.velocity.x = 0;
			}
		} else if (lowest == amtLeft) {
			if (amtTop <= tsee->player->step_size) {
				TSEE_Object_SetPosition(tsee, b, b->position.x,
										a->position.y + b->texture->rect.h);
			} else {
				TSEE_Object_SetPosition(
					tsee, b, a->position.x + a->texture->rect.w, b->position.y);
				b->physics.velocity.x = 0;
			}
		} else if (lowest == amtTop) {
			TSEE_Object_SetPosition(tsee, b, b->position.x,
									a->position.y + b->texture->rect.h);
			if (b == tsee->player->object) {
				tsee->player->grounded = true;
			}
			if (b->physics.velocity.y < 0) {
				b->physics.velocity.y = 0;
			}
		} else if (lowest == amtBottom) {
			TSEE_Object_SetPosition(tsee, b, b->position.x,
									a->position.y - a->texture->rect.h);
			if (b->physics.velocity.y > 0) {
				b->physics.velocity.y = 0;
			}
		}
	}
}