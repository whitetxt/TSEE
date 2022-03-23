// TSEE's Object type, keeps track of texture and position.
typedef struct TSEE_Object {
	TSEE_Texture *texture;
	float x;
	float y;
} TSEE_Object;

// Struct for parallax scrolling backgrounds
typedef struct TSEE_Parallax {
	TSEE_Texture *texture;
	float distance;
} TSEE_Parallax;

// TSEE's Physics Object type, extends TSEE_Object and has physics related information.
typedef struct TSEE_Physics_Object {
	TSEE_Object *object;
	TSEE_Vec2 velocity;
	TSEE_Vec2 acceleration;
	float mass;
} TSEE_Physics_Object;

// TSEE fonts, stores the font, its name and size.
typedef struct TSEE_Font {
	TTF_Font *font;
	char *name;
	int size;
} TSEE_Font;

// TSEE's text wrapper, keeps track of what the text says.
typedef struct TSEE_Text {
	TSEE_Texture *texture;
	char *text;
} TSEE_Text;