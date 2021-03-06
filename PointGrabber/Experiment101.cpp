#include "Experiment.h"
#include "RedClusterDetecter.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"

void issei::Experiment101::experimentLoop()
{
	mario::Coordinate<mario::typeM> ret;
	cout << "MeasureBasementの初期化中..." << endl;
	mario::RedClusterDetecter base;
	base.start();
	cout << "赤い場所の重心を計測します..." << endl;
	while(1){
		base.oneLoop();
	}
	base.stop();
	return ;
}
