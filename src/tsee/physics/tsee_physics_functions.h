void TSEE_Physics_SetObjectMass(TSEE_Object *obj, double mass);
void TSEE_Physics_PerformStep();
void TSEE_Physics_UpdateObject(TSEE *tsee, TSEE_Object *obj);
void TSEE_Physics_CheckAndResolveCollisions(TSEE *tsee, TSEE_Object *obj);
void TSEE_Physics_ResolveCollision(TSEE *tsee, TSEE_Manifold *manifold);
bool TSEE_Physics_GetAABBAABBManifold(TSEE_Manifold *m);