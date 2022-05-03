void TSEE_Physics_SetObjectMass(TSEE_Object *obj, float mass);
void TSEE_Physics_PerformStep();
void TSEE_Physics_UpdateObject(TSEE *tsee, TSEE_Object *obj);
void TSEE_Physics_CheckCollisions(TSEE *tsee, TSEE_Object *obj);
void TSEE_Physics_ResolveCollision(TSEE *tsee, TSEE_Object *first, TSEE_Object *second);