#include "../tsee.h"

void TSEE_Physics_PerformStep(TSEE *tsee) {
	for (size_t i = 0; i < tsee->world->objects->size; i++) {
		TSEE_Object *object = tsee->world->objects->data[i];
		if (object->attributes & TSEE_ATTRIB_PHYS_ENABLED) {
			TSEE_Physics_UpdateObject(tsee, object);
		}
	}
}

void TSEE_Physics_UpdateObject(TSEE *tsee, TSEE_Object *obj) {
	TSEE_Vec2 grav = {0, 0};
	grav.x = tsee->world->gravity.x;
	grav.y = tsee->world->gravity.y;
	TSEE_Vec2_Multiply(&grav, obj->physics.mass);
	TSEE_Vec2_Add(&obj->physics.force, &grav);
	TSEE_Vec2_Divide(&obj->physics.force, obj->physics.mass * tsee->dt);

	TSEE_Vec2 new_vel = {0, 0};
	new_vel.x = obj->physics.velocity.x;
	new_vel.y = obj->physics.velocity.y;
	TSEE_Vec2_Multiply(&new_vel, tsee->dt);
	TSEE_Vec2_Add(&obj->position, &new_vel);

	obj->physics.force.x = 0;
	obj->physics.force.y = 0;
}