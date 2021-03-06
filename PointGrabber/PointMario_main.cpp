#include "Calibration.h"
#include "utils.h"
#include "FileIO.h"
#include "Experiment.h"
#include "DeruChara.h"

using namespace mario;

#define CALIB_PATH "data/MD_calib_matrix.csv"

int appMain()
{
	bool endFlag = false;
	while( endFlag == false ){
		string buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "モード(0〜2,101〜)を入力してください(0で終了)：" << endl;
			cout << "   1:キャリブレーション" << endl;
			cout << "   2:「でるキャラ」" << endl;
			cout << "   3:「ジオラマ」" << endl;
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
				DeruChara deru(A);
				deru.mainLoop();
				break;
			}
		case 3:
			{
				Eigen::Matrix4d A;
				FileIO::loadMatrix(CALIB_PATH,A);
				DeruChara deru(A);
				deru.mainLoop();
				break;
			}
		}
	}
	return 0;
}