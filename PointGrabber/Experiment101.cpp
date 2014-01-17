#include "Experiment.h"
#include "MeasureBasement.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"

void mario::Experiment101::experimentLoop()
{
	mario::Coordinate<mario::typeM> ret;
	cout << "MeasureBasement‚Ì‰Šú‰»’†..." << endl;
	mario::MeasureBasement base;
	base.start();
	cout << "Ô‚¢êŠ‚ÌdS‚ðŒv‘ª‚µ‚Ü‚·..." << endl;
	while(1){
		base.oneLoop();
	}
	base.stop();
	return ;
}
