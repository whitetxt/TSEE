#include "include/tsee.h"

void TSEELog(char *message, ...) {
	va_list args;
	va_start(args, message);
	printf("TSEE [log]: ");
	vfprintf(stdout, message, args);
}

void TSEEWarn(char *message, ...) {
	va_list args;
	va_start(args, message);
	printf("TSEE [warn]: ");
	vfprintf(stdout, message, args);
}

void TSEEError(char *message, ...) {
	va_list args;
	va_start(args, message);
	printf("TSEE [error]: ");
	vfprintf(stdout, message, args);
}

void TSEECritical(char *message, ...) {
	va_list args;
	va_start(args, message);
	printf("TSEE [!! CRITICAL !!]: ");
	vfprintf(stdout, message, args);
}