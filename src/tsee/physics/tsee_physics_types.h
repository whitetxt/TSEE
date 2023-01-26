typedef struct TSEE_Manifold {
	TSEE_Object *a;
	TSEE_Object *b;
	TSEE_Vec2 normal;
	float depth;
} TSEE_Manifold;

#define MANIFOLD_NONE (TSEE_Manifold){NULL, NULL, VEC_ZERO, -1};