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
		TSEE_Object *object = tsee->world->objects->data[i];
		if (TSEE_Object_CheckAttribute(object, TSEE_ATTRIB_PHYS)) {
			TSEE_Physics_UpdateObject(tsee, object);
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

	// Check collisions now that we have moved
	TSEE_Physics_CheckAndResolveCollisions(tsee, obj);
}

/**
 * @brief Check for collisions with other objects
 *
 * @param obj Object to check collisions for
 *
 * @return The object collided with or NULL
 */
TSEE_Object *TSEE_Physics_CheckCollisions(TSEE *tsee, TSEE_Object *obj) {
	if (TSEE_Object_CheckAttribute(obj, TSEE_ATTRIB_PARALLAX)) return NULL;
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *other = tsee->world->objects->data[i];
		if (other == obj) continue;

		if (!TSEE_IsRectNull(TSEE_Object_GetCollisionRect(obj, other))) {
			return other;
		}
	}
	return NULL;
}

/**
 * @brief Check for collisions with other objects and resolve any resulting
 * collisions
 *
 * @param obj Object to check collisions for
 */
void TSEE_Physics_CheckAndResolveCollisions(TSEE *tsee, TSEE_Object *obj) {
	if (TSEE_Object_CheckAttribute(obj, TSEE_ATTRIB_PARALLAX)) return;
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *other = tsee->world->objects->data[i];
		if (other == obj) continue;

		if (!TSEE_IsRectNull(TSEE_Object_GetCollisionRect(obj, other))) {
			TSEE_Physics_ResolveCollision(tsee, obj, other);
		}
	}
}

/**
 * @brief Generates a manifold for an AABB-AABB Collision.
 * 
 * @param a first object
 * @param b second object
 * @return TSEE_Manifold
 */
TSEE_Manifold TSEE_Physics_GetAABBAABBManifold(TSEE_Object *a, TSEE_Object *b) {
	// https://github.com/St0wy/GPR4400-PhysicsEngine/blob/64c986032b74ec6aacb1b1dcd4733ab263bc1e8b/StowyPhysicsEngine/src/collision/ManifoldFactory.cpp#L69
	double aHalfWidth = a->texture->rect.w / 2;
	double aHalfHeight = a->texture->rect.h / 2;
	TSEE_Vec2 aCenter = {a->position.x + aHalfWidth, a->position.y + aHalfHeight};

	double bHalfWidth = b->texture->rect.w / 2;
	double bHalfHeight = b->texture->rect.h / 2;
	TSEE_Vec2 bCenter = {b->position.x + bHalfWidth, b->position.y + bHalfHeight};

	TSEE_Vec2 aToB = bCenter;
	TSEE_Vec2_Subtract(&aToB, aCenter);

	double xOverlap = aHalfWidth + bHalfWidth - fabs(aToB.x);

	if (xOverlap <= 0) return MANIFOLD_NONE;

	double yOverlap = aHalfHeight + bHalfHeight - fabs(aToB.y);

	if (yOverlap <= 0) return MANIFOLD_NONE;

	if (xOverlap > yOverlap) {
		TSEE_Vec2 normal;
		double aY, bY;

		if (aToB.y > 0) {
			normal = (TSEE_Vec2){0, -1};
			aY = aCenter.y + aHalfHeight;
			bY = bCenter.y - bHalfHeight;
		} else {
			normal = (TSEE_Vec2){0, 1};
			aY = aCenter.y - aHalfHeight;
			bY = bCenter.y + bHalfHeight;
		}

		double x;

		if (aToB.x > 0) {
			x = aCenter.x + aHalfWidth - xOverlap / 2;
		} else {
			x = aCenter.x - aHalfWidth + xOverlap / 2;
		}

		return (TSEE_Manifold){{x, aY}, {x, bY}, normal, yOverlap};
	} else {
		TSEE_Vec2 normal;
		double aX, bX;

		if (aToB.x > 0) {
			normal = (TSEE_Vec2){-1, 0};
			aX = aCenter.x + aHalfWidth;
			bX = bCenter.x - bHalfWidth;
		} else {
			normal = (TSEE_Vec2){1, 0};
			aX = aCenter.x - aHalfWidth;
			bX = bCenter.x + bHalfWidth;
		}

		double y;

		if (aToB.y > 0) {
			y = aCenter.x + aHalfWidth - xOverlap / 2;
		} else {
			y = aCenter.x - aHalfWidth + xOverlap / 2;
		}

		return (TSEE_Manifold){{aX, y}, {bX, y}, normal, xOverlap};
	}
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
void TSEE_Physics_ResolveCollision(TSEE *tsee,
								   TSEE_Object *a,
								   TSEE_Object *b) {
	if (TSEE_Object_CheckAttribute(a, TSEE_ATTRIB_PHYS) &&
		TSEE_Object_CheckAttribute(b, TSEE_ATTRIB_PHYS)) {
		// Time to try again
		TSEE_Vec2 overlap = {a->position.x - b->position.x, a->position.y - b->position.y};
		double divisor = a->physics.mass + b->physics.mass;
		double a_share = a->physics.mass / divisor;
		double b_share = b->physics.mass / divisor;
		if (overlap.x != 0) {
			double x_momentum = a->physics.velocity.x * a->physics.mass + b->physics.velocity.x * b->physics.mass;
			double a_momentum = x_momentum * a_share;
			double b_momentum = x_momentum * b_share;
			a->physics.velocity.x = a_momentum / a->physics.mass;
			b->physics.velocity.x = b_momentum / b->physics.mass;
			if (a->position.x > b->position.x) {
				a->position.x = b->position.x + b->texture->rect.w;
			} else {
				b->position.x = a->position.x + a->texture->rect.w;
			}
		}
		TSEE_Object_UpdatePosition(tsee, a);
		TSEE_Object_UpdatePosition(tsee, b);
		return;
	}
	int amtRight = fabs(a->position.x + a->texture->rect.w -
						b->position.x);
	int amtLeft = fabs(b->position.x + b->texture->rect.w -
						a->position.x);
	int amtTop = fabs(b->position.y - a->position.y +
						a->texture->rect.h);
	int amtBottom = fabs(a->position.y + a->texture->rect.h -
							b->position.y);

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
				TSEE_Object_SetPosition(
					tsee,
					a, a->position.x,
					b->position.y + a->texture->rect.h);
			} else {
				TSEE_Object_SetPosition(
					tsee,
					a,
					b->position.x - a->texture->rect.w,
					a->position.y);
				a->physics.velocity.x = 0;
			}
		} else if (lowest == amtLeft) {
			if (amtTop <= tsee->player->step_size) {
				TSEE_Object_SetPosition(
					tsee,
					a, a->position.x,
					b->position.y + a->texture->rect.h);
			} else {
				TSEE_Object_SetPosition(
					tsee,
					a,
					b->position.x + b->texture->rect.w,
					a->position.y);
				a->physics.velocity.x = 0;
			}
		} else if (lowest == amtTop) {
			TSEE_Object_SetPosition(
				tsee,
				a, a->position.x,
				b->position.y + a->texture->rect.h);
			if (a == tsee->player->object) {
				tsee->player->grounded = true;
			}
			if (a->physics.velocity.y < 0) {
				a->physics.velocity.y = 0;
			}
		} else if (lowest == amtBottom) {
			TSEE_Object_SetPosition(
				tsee,
				a, a->position.x,
				b->position.y - b->texture->rect.h);
			if (a->physics.velocity.y > 0) {
				a->physics.velocity.y = 0;
			}
		}
	} else {
		if (lowest == amtRight) {
			if (amtTop <= tsee->player->step_size) {
				TSEE_Object_SetPosition(
					tsee,
					b, b->position.x,
					a->position.y + b->texture->rect.h);
			} else {
				TSEE_Object_SetPosition(
					tsee,
					b,
					a->position.x - b->texture->rect.w,
					b->position.y);
				b->physics.velocity.x = 0;
			}
		} else if (lowest == amtLeft) {
			if (amtTop <= tsee->player->step_size) {
				TSEE_Object_SetPosition(
					tsee,
					b, b->position.x,
					a->position.y + b->texture->rect.h);
			} else {
				TSEE_Object_SetPosition(
					tsee,
					b,
					a->position.x + a->texture->rect.w,
					b->position.y);
				b->physics.velocity.x = 0;
			}
		} else if (lowest == amtTop) {
			TSEE_Object_SetPosition(
				tsee,
				b, b->position.x,
				a->position.y + b->texture->rect.h);
			if (b == tsee->player->object) {
				tsee->player->grounded = true;
			}
			if (b->physics.velocity.y < 0) {
				b->physics.velocity.y = 0;
			}
		} else if (lowest == amtBottom) {
			TSEE_Object_SetPosition(
				tsee,
				b, b->position.x,
				a->position.y - a->texture->rect.h);
			if (b->physics.velocity.y > 0) {
				b->physics.velocity.y = 0;
			}
		}
	}
}