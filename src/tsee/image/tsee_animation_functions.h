#pragma once

// This shouldn't be needed but VS Code wants it for some reason????
#include "../tsee.h"

bool TSEE_Animation_Init(TSEE *tsee);
bool TSEE_Animation_LoadFromSpriteSheet(TSEE *tsee, TSEE_Object *obj, char *fn, int numFrames, bool spriteFileIsHorizontal);
bool TSEE_Animation_RunStep(TSEE *tsee);