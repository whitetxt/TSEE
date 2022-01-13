#pragma once

#if __STDC_VERSION__ < 199901L
#	if __GNUC__ >= 2
#		define __func__ __FUNCTION__
#	else
#		define __func__ "<unknown>"
#	endif
#endif

#include "tsee_include.h"
#include "tsee_types.h"
#include "tsee_functions.h"