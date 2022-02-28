#include "include/tsee.h"

void TSEEInitAnimation(TSEE *tsee) {
	tsee->animations = malloc(sizeof(*tsee->animations));
	tsee->animations->animations = TSEEArrayCreate();
	tsee->init->animation = true;
}