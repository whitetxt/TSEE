#include "include/tsee.h"

bool TSEEInitUI(TSEE *tsee) {
	if (!tsee->init->text) {
		TSEEError("Attempted to init TSEEUI without initialising TSEEText");
		return false;
	}
	tsee->ui->toolbar = TSEEArrayCreate();
	tsee->init->text = true;
	return true;
}