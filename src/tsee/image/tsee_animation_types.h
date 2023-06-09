#pragma once

typedef struct TSEE_Object_Animation {
	double timer;
	double time_between_frames;
	int current_frame;
	TSEE_Array *frames;
	bool active;
	TSEE_Object *obj;
} TSEE_Object_Animation;

typedef struct TSEE_Animations {
	TSEE_Array *animations;
	double global_timer;
	bool global_active;
} TSEE_Animations;