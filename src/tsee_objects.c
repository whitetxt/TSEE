#include "include/tsee.h"

bool TSEECreateObject(TSEE *tsee, TSEE_Texture *texture) {
	TSEE_Object *obj = malloc(sizeof(*obj));
	obj->texture = texture;
	obj->x = texture->rect.x;
	obj->y = texture->rect.y;
	return TSEEArrayAppend(tsee->world->objects, obj);
}

int TSEECreatePhysicsObject(TSEE *tsee, TSEE_Texture *texture, double mass) {
	TSEE_Object *obj = malloc(sizeof(TSEE_Object));
	obj->texture = texture;
	obj->x = texture->rect.x;
	obj->y = texture->rect.y;
	TSEEArrayAppend(tsee->world->objects, obj);
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
			} else if (tsee->world->scroll_x > 0 && center < tsee->window->width / 2) {
				double under = tsee->window->width / 2 - center;	
				pobj->object->x = tsee->window->width / 2 - pobj->object->texture->rect.w / 2;
				tsee->world->scroll_x -= under;
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