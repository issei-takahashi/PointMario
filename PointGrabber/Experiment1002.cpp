#include "Experiment.h"
#include "ExperimentData.h"
#include "PointCapture.h"

void mario::Experiment1002::experimentLoop()
{
	string fileName;
	cout << "pcdファイル名を入力してください：";
	cin >> fileName;
	mario::PointCapture base(fileName);
	base.start();
	while( base.quitEvent() == false ){
		base.oneLoop();
	}
	base.stop();
}