#include "include/tsee.h"

int TSEECreateObject(TSEE *tsee, TSEE_Texture *texture) {
	TSEE_Object *obj = malloc(sizeof(*obj));
	obj->texture = texture;
	obj->x = texture->rect.x;
	obj->y = texture->rect.y;
	TSEEArrayAppend(tsee->world->objects, obj);
	return tsee->world->objects->size - 1;
}

int TSEECreatePhysicsObject(TSEE *tsee, TSEE_Texture *texture, double mass) {
	int idx = TSEECreateObject(tsee, texture);
	TSEE_Object *obj = TSEEArrayGet(tsee->world->objects, idx);
	TSEE_Physics_Object *pobj = malloc(sizeof(TSEE_Physics_Object));
	pobj->mass = mass;
	pobj->velocity.x = 0;
	pobj->velocity.y = 0;
	pobj->acceleration.x = 0;
	pobj->acceleration.y = 0;
	pobj->force.x = 0;
	pobj->force.y = 0;
	pobj->object = obj;
	TSEEArrayAppend(tsee->world->physics_objects, pobj);
	return tsee->world->physics_objects->size - 1;
}

bool TSEEConvertObjectToPhysicsObject(TSEE *tsee, TSEE_Object *obj, double mass) {
	TSEE_Physics_Object *pobj = malloc(sizeof(TSEE_Physics_Object));
	pobj->mass = mass;
	pobj->velocity.x = 0;
	pobj->velocity.y = 0;
	pobj->acceleration.x = 0;
	pobj->acceleration.y = 0;
	pobj->force.x = 0;
	pobj->force.y = 0;
	pobj->object = obj;
	return TSEEArrayAppend(tsee->world->physics_objects, obj);
}

bool TSEERenderObject(TSEE *tsee, TSEE_Object *obj) {
	if (SDL_RenderCopy(tsee->window->renderer, obj->texture->texture, NULL, &obj->texture->rect) == -1) {
		TSEEError("Failed to render object (%s)\n", SDL_GetError());
		return false;
	}
	return true;
}

bool TSEESetPlayerJumpForce(TSEE *tsee, double force) {
	tsee->player->jump_force = force;
	return true;
}

bool TSEESetPlayerSpeed(TSEE *tsee, double speed) {
	tsee->player->speed = speed;
	return true;
}

bool TSEEPerformPhysics(TSEE *tsee) {
	for (size_t i = 0; i < tsee->world->physics_objects->size; i++) {
		TSEE_Physics_Object *pobj = TSEEArrayGet(tsee->world->physics_objects, i);
		double gravity = tsee->world->gravity * pobj->mass;
		if (tsee->player->physics_object == pobj) {
			TSEE_Vec2 force = {tsee->player->movement.right - tsee->player->movement.left, tsee->player->movement.down - tsee->player->movement.up};
			if (tsee->player->grounded && tsee->player->movement.up) {
				force.y = -gravity * tsee->player->jump_force;
			}
			TSEEVec2Multiply(&force, pobj->mass);
			force.x *= tsee->player->speed;
			TSEEApplyForce(pobj, force);
		}
		TSEEApplyForce(pobj, (TSEE_Vec2){0, gravity});
		TSEEVec2Add(&pobj->velocity, &pobj->acceleration);
		pobj->velocity.x += pobj->acceleration.x * tsee->dt;
		pobj->velocity.y += pobj->acceleration.y * tsee->dt;
		pobj->object->x += pobj->velocity.x * tsee->dt;
		pobj->object->y += pobj->velocity.y * tsee->dt;
		if (tsee->player->physics_object == pobj) {
			double center = pobj->object->x + pobj->object->texture->rect.w / 2;
			if (center > tsee->window->width / 2) {
				double over = center - tsee->window->width / 2;
				pobj->object->x = tsee->window->width / 2 - pobj->object->texture->rect.w / 2;
				tsee->world->scroll_x += over;
				for (size_t j = 0; j < tsee->world->objects->size; j++) {
					TSEE_Object *obj = TSEEArrayGet(tsee->world->objects, j);
					if (obj != pobj->object) {
						obj->x -= over;
						obj->texture->rect.x = obj->x;
					}
				}
			} else if (tsee->world->scroll_x > 0 && center < tsee->window->width / 2) {
				double under = tsee->window->width / 2 - center;	
				pobj->object->x = tsee->window->width / 2 - pobj->object->texture->rect.w / 2;
				tsee->world->scroll_x -= under;
				for (size_t j = 0; j < tsee->world->objects->size; j++) {
					TSEE_Object *obj = TSEEArrayGet(tsee->world->objects, j);
					if (obj != pobj->object) {
						obj->x += under;
						obj->texture->rect.x = obj->x;
					}
				}
			}
			if (pobj->object->x < 0) {
				pobj->object->x = 0;
			}
		}
		pobj->object->texture->rect.x = pobj->object->x;
		pobj->object->texture->rect.y = pobj->object->y;
		if (pobj->object->y + pobj->object->texture->rect.h > tsee->window->height) {
			pobj->velocity.y = 0;
			pobj->object->y = tsee->window->height - pobj->object->texture->rect.h;
			if (tsee->player->physics_object == pobj) {
				tsee->player->grounded = true;
			}
		} else {
			if (tsee->player->physics_object == pobj) {
				tsee->player->grounded = false;
			}
		}
		TSEEVec2Multiply(&pobj->acceleration, 0);
		pobj->velocity.x *= 0.9;
		pobj->velocity.y *= 0.99;
	}
	return TSEEPerformCollision(tsee);
}

bool TSEEPerformCollision(TSEE *tsee) {
	for (size_t i = 0; i < tsee->world->physics_objects->size; i++) {
		TSEE_Physics_Object *pobj = TSEEArrayGet(tsee->world->physics_objects, i);
		for (size_t j = 0; j < tsee->world->objects->size; j++) {
			TSEE_Object *obj = TSEEArrayGet(tsee->world->objects, j);
			if (obj == pobj->object) { // Don't collide with ourselves
				continue;
			}
			// If the tile is too far away, don't waste time checking for collision.
			double diff = pobj->object->x - obj->x;
			if (diff > obj->texture->rect.w || diff < -pobj->object->texture->rect.w) {
				continue;
			}
			double ydiff = pobj->object->y - obj->y;
			if (ydiff > obj->texture->rect.h || ydiff < -pobj->object->texture->rect.h) {
				continue;
			}
			// First, check if it collides.
			if(	pobj->object->texture->rect.x + pobj->object->texture->rect.w > obj->texture->rect.x &&
				pobj->object->texture->rect.x < obj->texture->rect.x + obj->texture->rect.w &&
				pobj->object->texture->rect.y + pobj->object->texture->rect.h > obj->texture->rect.y &&
				pobj->object->texture->rect.y < obj->texture->rect.y + obj->texture->rect.h) {
				// Figure out collision side
				int amtRight = abs(pobj->object->texture->rect.x + pobj->object->texture->rect.w - obj->texture->rect.x);
				int amtLeft = abs(obj->texture->rect.x + obj->texture->rect.w - pobj->object->texture->rect.x);
				int amtTop = abs(obj->texture->rect.y - pobj->object->texture->rect.y + pobj->object->texture->rect.h);
				int amtBottom = abs(pobj->object->texture->rect.y + pobj->object->texture->rect.h - obj->texture->rect.y);

				int values[4] = {amtRight, amtLeft, amtTop, amtBottom};
				int lowest = values[0];
				// Get lowest value, side it collided on
				for (int x = 0; x < 4; x++) {
					if (values[x] < lowest) {
						lowest = values[x];
					}
				}

				// Handle collision side accordingly.
				if (lowest == amtRight) {
					pobj->object->texture->rect.x = obj->texture->rect.x - pobj->object->texture->rect.w;
					pobj->object->x = pobj->object->texture->rect.x;
				} else if (lowest == amtLeft) {
					pobj->object->texture->rect.x = obj->texture->rect.x + obj->texture->rect.w;
					pobj->object->x = pobj->object->texture->rect.x;
				} else if (lowest == amtTop) {
					pobj->object->texture->rect.y = obj->texture->rect.y + obj->texture->rect.h;
					pobj->object->y = pobj->object->texture->rect.y;
					if (pobj->velocity.y < 0)
						pobj->velocity.y = 0;
				} else if (lowest == amtBottom) {
					pobj->object->texture->rect.y = obj->texture->rect.y - pobj->object->texture->rect.h;
					pobj->object->y = pobj->object->texture->rect.y;
					if (pobj == tsee->player->physics_object) {
						tsee->player->grounded = true;
					}
					if (pobj->velocity.y > 0)
						pobj->velocity.y = 0;
				}
			}
		}
	}
	return true;
}

bool TSEESetObjectPosition(TSEE *tsee, int idx, double x, double y) {
	TSEE_Object *obj = TSEEArrayGet(tsee->world->objects, idx);
	if (obj == NULL) {
		return false;
	}
	obj->x = x;
	obj->y = y;
	obj->texture->rect.x = x;
	obj->texture->rect.y = y;
	return true;
}

bool TSEECreatePlayer(TSEE *tsee, TSEE_Physics_Object *pobj) {
	tsee->player->physics_object = pobj;
	return true;
}

bool TSEEApplyForce(TSEE_Physics_Object *pobj, TSEE_Vec2 force) {
	pobj->acceleration.x += force.x / pobj->mass;
	pobj->acceleration.y += force.y / pobj->mass;
	return true;
}

bool TSEEVec2Add(TSEE_Vec2 *final, TSEE_Vec2 *add) {
	final->x += add->x;
	final->y += add->y;
	return true;
}

bool TSEEVec2Normalise(TSEE_Vec2 *vec) {
	double len = sqrt(vec->x * vec->x + vec->y * vec->y);
	if (len == 0) {
		return false;
	}
	vec->x /= len;
	vec->y /= len;
	return true;
}

bool TSEEVec2Multiply(TSEE_Vec2 *vec, double mult) {
	vec->x *= mult;
	vec->y *= mult;
	return true;
}