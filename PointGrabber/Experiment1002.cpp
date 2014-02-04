#include "Experiment.h"
#include "ExperimentData.h"
#include "RedClusterDetecter.h"

void mario::Experiment1002::experimentLoop()
{
	mario::RedClusterDetecter base;
	base.start();
	base.captureForExperiment1002();
	base.stop();
}