#include "Calibration.h"
#include "utils.h"
#include "FileIO.h"
#include "Experiment.h"
#include "DeruChara.h"
#include "TokyoSky1.h"
#include "TokyoSky2.h"
#include "FallingSnow.h"

using namespace mario;

#define CALIB_PATH "data/MD_calib_matrix.csv"

int appMain()
{
	bool endFlag = false;
	while( endFlag == false ){
		string buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "モード(0～2,101～)を入力してください(0で終了)：" << endl;
			cout << "   1:キャリブレーション" << endl;
			cout << "   2:「でるキャラ」" << endl;
			cout << "   3:「ジオラマ解説」" << endl;
			cout << "   4:「ジオラマと雪」" << endl;
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
				list<Coordinate<typeD> > pointList;
				pointList.push_back( Coordinate<typeD>(0,100,100) );
				pointList.push_back( Coordinate<typeD>(200,100,100) );
				pointList.push_back( Coordinate<typeD>(100,0,100) );
				pointList.push_back( Coordinate<typeD>(100,200,100) );
				pointList.push_back( Coordinate<typeD>(100,100,0) );
				pointList.push_back( Coordinate<typeD>(100,100,200) );
				Calibration::getInstance()->executeCalibration(pointList,CALIB_PATH);
				break;
			}
		case 2:
			{
				Eigen::Matrix4d A;
				FileIO::loadMatrix(CALIB_PATH,A);
				auto deru = DeruChara::makeShared(A);
				deru->mainLoop();
				break;
			}
		case 3:
			{
				Eigen::Matrix4d A;
				FileIO::loadMatrix(CALIB_PATH,A);
				auto app = TokyoSky1::makeShared(A);
				app->mainLoop();
				break;
			}
		case 4:
			{
				Eigen::Matrix4d A;
				FileIO::loadMatrix(CALIB_PATH,A);
				auto app = FallingSnow::makeShared(A);
				app->mainLoop();
				break;
			}
		}
	}
	return 0;
}