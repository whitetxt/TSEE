#include "include/tsee.h"

void TSEEInitAnimation(TSEE *tsee) {
	tsee->animations = xmalloc(sizeof(*tsee->animations));
	tsee->animations->animations = TSEEArrayCreate();
	tsee->init->animation = true;
}