#include "../tsee.h"

/**
 * @brief Sets an object's mass, also calculating its inverse mass.
 *
 * @param obj Object to change
 * @param mass New mass
 */
void TSEE_Physics_SetObjectMass(TSEE_Object *obj, float mass)
{
    if (!TSEE_Object_CheckAttribute(obj, TSEE_ATTRIB_PHYS))
        return; // Ignore objects which are not physics objects.
    obj->physics.mass = mass;
    obj->physics.inv_mass = 1 / mass;
}

/**
 * @brief Perform a physics step
 *
 * @param tsee TSEE to perform the step for
 */
void TSEE_Physics_PerformStep(TSEE *tsee)
{
    Uint64 start = SDL_GetPerformanceCounter();
    for (size_t i = 0; i < tsee->world->objects->size; i++)
    {
        TSEE_Object *object = tsee->world->objects->data[i];
        if (TSEE_Object_CheckAttribute(object, TSEE_ATTRIB_PHYS))
        {
            TSEE_Physics_UpdateObject(tsee, object);
        }
    }
    if (tsee->player->object)
    {
        TSEE_World_ScrollToObject(tsee, tsee->player->object);
        TSEE_Log("Scroll: %f, %f\n", tsee->world->scroll_x, tsee->world->scroll_y);
    }
    Uint64 end = SDL_GetPerformanceCounter();
    tsee->debug->physics_time += (end - start) * 1000 / (double)SDL_GetPerformanceFrequency();
}

/**
 * @brief Update a physics object
 *
 * @param tsee TSEE object this object is in
 * @param obj Object to update
 */
void TSEE_Physics_UpdateObject(TSEE *tsee, TSEE_Object *obj)
{
    // Apply gravity
    // F = ma
    TSEE_Vec2 grav = tsee->world->gravity;
    float dt = tsee->dt;

    // a * m
    TSEE_Vec2_Multiply(&grav, obj->physics.mass);

    // F += am
    TSEE_Vec2_Add(&obj->physics.force, grav);

    // F = ma
    // a = F / m
    // a = F * (1 / m)
    TSEE_Vec2 accel = (TSEE_Vec2){obj->physics.force.x * obj->physics.inv_mass, obj->physics.force.y * obj->physics.inv_mass};
    TSEE_Vec2_Multiply(&accel, dt);
    // Euler Explicit
    TSEE_Vec2_Add(&obj->physics.velocity, accel);

    TSEE_Vec2 vel = obj->physics.velocity;
    // TSEE_Vec2_Multiply(&vel, dt);

    TSEE_Vec2 pos = obj->position;
    TSEE_Vec2_Add(&pos, vel);

    TSEE_Object_SetPositionVec2(tsee, obj, pos);

    obj->physics.force.x = 0;
    obj->physics.force.y = 0;
    TSEE_Vec2_Multiply(&obj->physics.velocity, 0.99);

    // Check collisions now that we have moved
    TSEE_Physics_CheckCollisions(tsee, obj);
}

/**
 * @brief Check for collisions with other objects
 *
 * @param tsee TSEE to check for collisions in
 * @param obj Object to check collisions for
 */
void TSEE_Physics_CheckCollisions(TSEE *tsee, TSEE_Object *obj)
{
    for (size_t i = 0; i < tsee->world->objects->size; i++)
    {
        TSEE_Object *other = tsee->world->objects->data[i];
        if (TSEE_Object_CheckAttribute(other, TSEE_ATTRIB_PARALLAX))
            continue;
        if (other == obj)
            continue;

        if (!TSEE_IsRectNull(TSEE_Object_GetCollisionRect(obj, other)))
        {
            TSEE_Physics_ResolveCollision(tsee, obj, other);
        }
    }
}

/**
 * @brief Resolves a collision between two objects
 *
 * @param first First object
 * @param second Second object
 */
void TSEE_Physics_ResolveCollision(TSEE *tsee, TSEE_Object *first, TSEE_Object *second)
{
    if (TSEE_Object_CheckAttribute(first, TSEE_ATTRIB_PHYS) && TSEE_Object_CheckAttribute(second, TSEE_ATTRIB_PHYS))
    {
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
        if (TSEE_Vec2_Dot(rel_vel, first_vel_norm) > 0)
        {
            return;
        }

        // Get the overlap
        TSEE_Vec2 overlap = first->physics.velocity;
        TSEE_Vec2_Subtract(&overlap, second->physics.velocity);
        TSEE_Vec2_Multiply(&overlap, 0.5);

        // Apply the overlap to the objects
        TSEE_Vec2_Add(&first->physics.velocity, overlap);
        TSEE_Vec2_Subtract(&second->physics.velocity, overlap);
    }
    else
    {
        int amtRight = fabs(first->position.x + first->texture->rect.w - second->position.x);
        int amtLeft = fabs(second->position.x + second->texture->rect.w - first->position.x);
        int amtTop = fabs(second->position.y - first->position.y + first->texture->rect.h);
        int amtBottom = fabs(first->position.y + first->texture->rect.h - second->position.y);

        int values[4] = {amtRight, amtLeft, amtTop, amtBottom};
        int lowest = values[0];
        // Get lowest value, side it collided on
        for (int x = 1; x < 4; x++)
        {
            if (values[x] < lowest)
            {
                lowest = values[x];
            }
        }

        if (TSEE_Object_CheckAttribute(first, TSEE_ATTRIB_PHYS))
        {
            if (lowest == amtRight)
            {
                if (amtTop <= 5)
                {
                    TSEE_Object_SetPosition(tsee, first, first->position.x, second->position.y + first->texture->rect.h);
                }
                else
                {
                    TSEE_Object_SetPosition(tsee, first, second->position.x - first->texture->rect.w, first->position.y);
                    first->physics.velocity.x = 0;
                }
            }
            else if (lowest == amtLeft)
            {
                if (amtTop <= 5)
                {
                    TSEE_Object_SetPosition(tsee, first, first->position.x, second->position.y + first->texture->rect.h);
                }
                else
                {
                    TSEE_Object_SetPosition(tsee, first, second->position.x + second->texture->rect.w, first->position.y);
                    first->physics.velocity.x = 0;
                }
            }
            else if (lowest == amtTop)
            {
                TSEE_Object_SetPosition(tsee, first, first->position.x, second->position.y + first->texture->rect.h);
                if (first == tsee->player->object)
                {
                    tsee->player->grounded = true;
                }
                if (first->physics.velocity.y < 0)
                {
                    first->physics.velocity.y = 0;
                }
            }
            else if (lowest == amtBottom)
            {
                TSEE_Object_SetPosition(tsee, first, first->position.x, second->position.y - second->texture->rect.h);
                if (first->physics.velocity.y > 0)
                {
                    first->physics.velocity.y = 0;
                }
            }
        }
        else
        {
            if (lowest == amtRight)
            {
                if (amtTop < 5)
                {
                    TSEE_Object_SetPosition(tsee, second, second->position.x, first->position.y + second->texture->rect.h);
                }
                else
                {
                    TSEE_Object_SetPosition(tsee, second, first->position.x - second->texture->rect.w, second->position.y);
                    second->physics.velocity.x = 0;
                }
            }
            else if (lowest == amtLeft)
            {
                if (amtTop < 5)
                {
                    TSEE_Object_SetPosition(tsee, second, second->position.x, first->position.y + second->texture->rect.h);
                }
                else
                {
                    TSEE_Object_SetPosition(tsee, second, first->position.x + first->texture->rect.w, second->position.y);
                    second->physics.velocity.x = 0;
                }
            }
            else if (lowest == amtTop)
            {
                TSEE_Object_SetPosition(tsee, second, second->position.x, first->position.y + second->texture->rect.h);
                if (second == tsee->player->object)
                {
                    tsee->player->grounded = true;
                }
                if (second->physics.velocity.y < 0)
                {
                    second->physics.velocity.y = 0;
                }
            }
            else if (lowest == amtBottom)
            {
                TSEE_Object_SetPosition(tsee, second, second->position.x, first->position.y - first->texture->rect.h);
                if (second->physics.velocity.y > 0)
                {
                    second->physics.velocity.y = 0;
                }
            }
        }
    }
}