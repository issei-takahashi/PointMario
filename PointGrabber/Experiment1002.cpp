#include "Experiment.h"
#include "ExperimentData.h"
#include "MeasureBasement.h"

void mario::Experiment1002::experimentLoop()
{
	mario::MeasureBasement base;
	base.start();
	base.captureForExperiment1002();
	base.stop();
}