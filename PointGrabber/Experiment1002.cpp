#include "Experiment.h"
#include "ExperimentData.h"
#include "PointCapture.h"

void mario::Experiment1002::experimentLoop()
{
	string fileName;
	cout << "pcd�t�@�C��������͂��Ă��������F";
	cin >> fileName;
	mario::PointCapture base(fileName);
	base.start();
	while( base.quitEvent() == false ){
		base.oneLoop();
	}
	base.stop();
}