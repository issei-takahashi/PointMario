#include "Experiment.h"
#include "RedClusterDetecter.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"

void issei::Experiment101::experimentLoop()
{
	mario::Coordinate<mario::typeM> ret;
	cout << "MeasureBasement�̏�������..." << endl;
	mario::RedClusterDetecter base;
	base.start();
	cout << "�Ԃ��ꏊ�̏d�S���v�����܂�..." << endl;
	while(1){
		base.oneLoop();
	}
	base.stop();
	return ;
}
