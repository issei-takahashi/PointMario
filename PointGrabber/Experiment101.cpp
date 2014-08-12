#include "Experiment.h"
#include "MarkerDetecter.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"

void issei::Experiment101::experimentLoop()
{
	mario::Coordinate<mario::typeM> ret;
	cout << "MeasureBasement‚Ì‰Šú‰»’†..." << endl;
	mario::MarkerDetecter base;
	base.start();
	cout << "Ô‚¢êŠ‚ÌdS‚ðŒv‘ª‚µ‚Ü‚·..." << endl;
	while(1){
		base.oneLoop();
	}
	base.stop();
	return ;
}
