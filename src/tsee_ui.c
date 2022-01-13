#include "include/tsee.h"

bool TSEEInitUI(TSEE *tsee) {
	if (!tsee->init->text) {
		TSEEError("Attempted to init TSEE UI without initialising TSEE Text.\n");
		return false;
	}
	tsee->ui->toolbar = TSEEArrayCreate();
	tsee->init->text = true;
	return true;
}