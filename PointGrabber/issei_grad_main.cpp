#include "utils.h"
#include "Experiment.h"

int issei_grad_appMain()
{
	bool endFlag = false;
	while( endFlag == false ){
		string buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "ƒ‚[ƒh(0`2,101`)‚ð“ü—Í‚µ‚Ä‚­‚¾‚³‚¢(0‚ÅI—¹)F";
			cin  >> buf;
		}
		int mode = utils::string2int(buf);
		switch( mode ){
		case 0:
			{
				endFlag = true;
				break;
			}
		case 1:
			{
				issei::Experiment001 exp001;
				exp001.experimentLoop();
				break;
			}
		case 2:
			{
				issei::Experiment002 exp002;
				exp002.experimentLoop();
				break;
			}
		case 3:
			{
				issei::Experiment003 exp003("data/MtoD.csv");
				exp003.experimentLoop();
				break;
			}
		case 101:
			{
				issei::Experiment101 exp101;
				exp101.experimentLoop();
				break;
			}
		case 1001:
			{
				issei::Experiment1001 exp1001;
				exp1001.experimentLoop();
				break;
			}
		case 1002:
			{
				issei::Experiment1002 exp1002;
				exp1002.experimentLoop();
				break;
			}
		}
	}
	return 0;
}
