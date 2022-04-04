typedef enum TSEE_Object_Attributes {
	TSEE_PHYSICS_ENABLED = 1,
	TSEE_PARALLAX = 2,
	TSEE_TEXT_OBJECT = 4,
	TSEE_STATIC = 8,
} TSEE_Object_Attributes;

typedef struct TSEE_Physics_Data {
	TSEE_Vec2 velocity;
	TSEE_Vec2 acceleration;
	float mass;
} TSEE_Physics_Data;

typedef struct TSEE_Parallax_Data {
	float distance;
} TSEE_Parallax_Data;

typedef struct TSEE_Text_Data {
	char *text;
} TSEE_Text_Data;

// TSEE's Object type, keeps track of texture and position.
typedef struct TSEE_Object {
	TSEE_Texture *texture;
	TSEE_Vec2 position;
	TSEE_Object_Attributes attributes;
	union {
		TSEE_Physics_Data physics;
		TSEE_Parallax_Data parallax;
		TSEE_Text_Data text;
	};
} TSEE_Object;

// TSEE fonts, stores the font, its name and size.
typedef struct TSEE_Font {
	TTF_Font *font;
	char *name;
	int size;
} TSEE_Font;