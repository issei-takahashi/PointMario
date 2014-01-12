#include "main.h"
#include "FileIO.h"
#include "Experiment.h"

int mario::Main::mode = 1;

void mario::Main::init()
{
	mario::FileIO::loadAllData();
}

int mario::Main::main()
{
	bool endFlag = false;
	while( endFlag == false ){
		cout << "モード(1/2)を入力してください．0を入力するとプログラムを終了します．" << endl;
		cin >> Main::mode;
		switch( Main::mode ){
		case 0:
			{
				endFlag = true;
				break;
			}
		case 1:
			{
				mario::Experiment001 exp001;
				exp001.experimentLoop();
				break;
			}
		case 2:
			{
				mario::Experiment002 exp002;
				exp002.experimentLoop();
				break;
			}
		}
	}
	return 0;
}

int main(int argc, char** argv)
{
	mario::Main::init();
	return mario::Main::main();
}