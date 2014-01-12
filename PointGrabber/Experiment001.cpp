#include "Experiment.h"
#include "MeasureBasement.h"
#include "FileIO.h"
#include "Display.h"

void mario::Experiment001::experimentLoop()
{
	try{
		static int const DISP_X_mm = FileIO::getConst("DISP_X_mm");
		static int const DISP_Y_mm = FileIO::getConst("DISP_Y_mm");
		static int const DISP_X_px = FileIO::getConst("DISP_X_px");
		static int const DISP_Y_px = FileIO::getConst("DISP_Y_px");
		while(1){
			mario::typeR xR, yR, zR;
			bool is_pR_ok = false;
			while( is_pR_ok == false ){
				cout << "xR(mm)����͂��Ă��������F";
				cin  >> xR;
				cout << "yR(mm)����͂��Ă��������F";
				cin  >> yR;
				cout << "zR(mm)����͂��Ă��������F";
				cin  >> zR;
				cout << "(" << xR << "," << yR << "," << zR << ") �ł�낵���ł����H(y/n)" << endl;
				string okStr;
				cin  >> okStr;
				if( okStr == "y" ){
					is_pR_ok = true;
				}
			}
			cout << "�Ԃ��ꏊ�̏d�S��60��v�����܂�..." << endl;
			mario::MeasureBasement base;
			base.start();
			mario::Display disp( DISP_X_mm, DISP_Y_mm, DISP_X_px, DISP_Y_px );
			disp.start();
			times(i,60){
				auto ms1 = mario::DisplayTimer::getTime();
				base.oneLoop();
				disp.oneLoop();
				auto ms2 = mario::DisplayTimer::getTime();
				static int const FPS = FileIO::getConst("FPS");
				if( ms2 - ms1 < 1000/FPS ){
					disp.wait( 1000/FPS - ( ms2 - ms1 ) );
				}
				auto ms3 = mario::DisplayTimer::getTime();
			}
			cout << "�Ԃ��ꏊ�̏d�S��60��v�����܂����D���ϒl��" << 999 << "�ł��D" << endl;
			base.stop();
			disp.stop();
		}
	}
	catch( std::exception& ex ){
		cout << ex.what() << endl;
	}
}

