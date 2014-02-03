#include "Experiment.h"
#include "MeasureBasement.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"
#include "Timer.h"

void mario::Experiment003::experimentLoop()
{
	mario::Coordinate<mario::typeM> ret;
	cout << "MeasureBasement‚Ì‰Šú‰»’†..." << endl;
	mario::MeasureBasement base;
	base.start();
	static int const DISP_X_mm = FileIO::getConst("DISP_X_mm");
	static int const DISP_Y_mm = FileIO::getConst("DISP_Y_mm");
	static int const DISP_Z_mm = FileIO::getConst("DISP_Z_mm");
	static int const DISP_X_px = FileIO::getConst("DISP_X_px");
	static int const DISP_Y_px = FileIO::getConst("DISP_Y_px");
	static mario::Display disp( DISP_X_mm, DISP_Y_mm, DISP_X_px, DISP_Y_px );
	disp.start();
	disp.setScreenMode( true );
	Eigen::Matrix4d A;
	FileIO::loadTranslation( "data/MtoD.csv", A );
	bool endFlag = false;
	while( endFlag == false ){
		auto ms1 = mario::DisplayTimer::getTime();
		//base.oneLoop();
		auto thisRed = base.getRedCenter();
		//thisRed *= 1000;
		Eigen::Vector4d tmpv;
		tmpv(0) = thisRed.x;
		tmpv(1) = thisRed.y;
		tmpv(2) = thisRed.z;
		tmpv(3) = 1.0;
		tmpv = A*tmpv;
		Coordinate<typeD> pD(tmpv(0),tmpv(1),tmpv(2));
		cout << pD.x << "," << pD.y << "," << pD.z << endl;
		disp.drawCross( pD, true );
		if( disp.quitEvent() ){
			endFlag = true;
		}
		auto ms2 = mario::DisplayTimer::getTime();
		static int const FPS = FileIO::getConst("FPS");
		if( ms2 - ms1 < 1000.0/FPS ){
			disp.wait( 1000.0/FPS - ( ms2 - ms1 ) );
		}
		auto ms3 = mario::DisplayTimer::getTime();
	}
	disp.stop();
	base.stop();
	return ;
}
