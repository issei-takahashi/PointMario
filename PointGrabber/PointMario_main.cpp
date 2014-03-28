#include "Calibration.h"

int appMain()
{
	mario::Calibration::getInstance()->executeCalibration(6,"data/MDmatrix.csv");
	return 1;
}