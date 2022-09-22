#pragma once

#if __STDC_VERSION__ < 199901L
#	if __GNUC__ >= 2
#		define __func__ __FUNCTION__
#	else
#		define __func__ "<unk func>"
#	endif
#endif

// Include required libs
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <libfyaml.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

#include "ext/tinyfiledialogs.h"

// Include TSEE types
#include "resources/tsee_resources_types.h"
#include "util/tsee_util_types.h"
#include "image/tsee_image_types.h"
#include "objects/tsee_object_types.h"
#include "UI/tsee_ui_types.h"
#include "maps/tsee_map_types.h"
#include "events/tsee_event_types.h"
#include "player/tsee_player_types.h"
#include "physics/tsee_physics_types.h"

#include "main/tsee_types.h"

// Include TSEE functions
#include "resources/tsee_resources_functions.h"
#include "util/tsee_util_functions.h"
#include "objects/tsee_object_functions.h"
#include "image/tsee_image_functions.h"
#include "UI/tsee_ui_functions.h"
#include "maps/tsee_map_functions.h"
#include "events/tsee_event_functions.h"
#include "player/tsee_player_functions.h"
#include "physics/tsee_physics_functions.h"

#include "main/tsee_functions.h"