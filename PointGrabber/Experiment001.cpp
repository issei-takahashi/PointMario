#include "Experiment.h"
#include "MeasureBasement.h"
#include "FileIO.h"
#include "Display.h"
#include "utils.h"

void mario::Experiment001::experimentLoop()
{
	try{
		this->inputRealPositionLoop();
		this->measureRedPointsLoop();
		this->showCrossAndRegisterCrossLoop();
	}
	catch( std::exception& ex ){
		cout << ex.what() << endl;
	}
	catch( boost::exception & ex ){
		cout << "boost exception" << endl;
	}
}

void mario::Experiment001::inputRealPositionLoop()
{
	mario::typeR xR, yR, zR;
	bool is_pR_ok = false;
	while( is_pR_ok == false ){
		string buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "xR(mm)����͂��Ă��������F";
			cin  >> buf;
		}
		xR = utils::string2int(buf);
		buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "yR(mm)����͂��Ă��������F";
			cin  >> buf;
		}
		yR = utils::string2int(buf);
		buf = "";
		while( utils::isNumber(buf) == false ){
			cout << "zR(mm)����͂��Ă��������F";
			cin  >> buf;
		}
		zR = utils::string2int(buf);
		cout << "(" << xR << "," << yR << "," << zR << ") �ł�낵���ł����H(y/n)�F";
		string okStr;
		cin  >> okStr;
		if( okStr == "y" ){
			is_pR_ok = true;
		}
	}
}

void mario::Experiment001::measureRedPointsLoop()
{
	cout << "MeasureBasement�̏�������..." << endl;
	mario::MeasureBasement base;
	base.start();
	cout << "�Ԃ��ꏊ�̏d�S��60��v�����܂�..." << endl;
	times(i,0,60){
		auto ms1 = mario::DisplayTimer::getTime();
		base.oneLoop();
		auto ms2 = mario::DisplayTimer::getTime();
		static int const FPS = FileIO::getConst("FPS");
		if( ms2 - ms1 < 1000.0/FPS ){
			Sleep( 1000.0/FPS - ( ms2 - ms1 ) );
		}
		auto ms3 = mario::DisplayTimer::getTime();
	}
	base.stop();
	cout << "�Ԃ��ꏊ�̏d�S��60��v�����܂����D���ϒl��" << 999 << "�ł��D" << endl;

}

void mario::Experiment001::showCrossAndRegisterCrossLoop()
{
	static int const DISP_X_mm = FileIO::getConst("DISP_X_mm");
	static int const DISP_Y_mm = FileIO::getConst("DISP_Y_mm");
	static int const DISP_Z_mm = FileIO::getConst("DISP_Z_mm");
	static int const DISP_X_px = FileIO::getConst("DISP_X_px");
	static int const DISP_Y_px = FileIO::getConst("DISP_Y_px");
	cout << "�v�������ʒu��+�}�[�N���o���܂��D����Ă���������L�[��+���ړ����č��킹�Ă��������D" << endl;
	mario::Display disp( DISP_X_mm, DISP_Y_mm, DISP_X_px, DISP_Y_px );
	disp.start();
	disp.changeScreenMode();
	disp.set_crossPos( Coordinate<typeD>(DISP_X_mm/2,DISP_Y_mm/2,DISP_Z_mm/2) );
	bool endFlag = false;
	bool keyUpdateFlag = false;
	while( endFlag == false ){
		auto ms1 = mario::DisplayTimer::getTime();
		if( keyUpdateFlag ){
			endFlag = disp.keyInputEvent1();
		}
		keyUpdateFlag = !keyUpdateFlag;
		disp.drawCross( disp.get_crossPos(), true );
		disp.quitEvent();
		auto ms2 = mario::DisplayTimer::getTime();
		static int const FPS = FileIO::getConst("FPS");
		if( ms2 - ms1 < 1000.0/FPS ){
			disp.wait( 1000.0/FPS - ( ms2 - ms1 ) );
		}
		auto ms3 = mario::DisplayTimer::getTime();
	}
	disp.stop();
}