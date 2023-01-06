typedef struct TSEE_Manifold {
	TSEE_Vec2 a;
	TSEE_Vec2 b;
	TSEE_Vec2 normal;
	float depth;
} TSEE_Manifold;

#define MANIFOLD_NONE (TSEE_Manifold){{0, 0}, {0, 0}, {0, 0}, -1};