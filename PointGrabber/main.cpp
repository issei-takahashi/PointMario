#include "MeasureBasement.h"
#include "FileIO.h"
#include "Display.h"

namespace mario
{

	class Main{
		friend int main();
	public:
		static void init()
		{
			mario::FileIO::loadAllData();
		}

		static int main()
		{
			try{
				mario::FileIO::loadAllData();

				mario::MeasureBasement base;
				static int const DISP_X_mm = FileIO::getConst("DISP_X_mm");
				static int const DISP_Y_mm = FileIO::getConst("DISP_Y_mm");
				static int const DISP_X_px = FileIO::getConst("DISP_X_px");
				static int const DISP_Y_px = FileIO::getConst("DISP_Y_px");
				mario::Display disp( DISP_X_mm, DISP_Y_mm, DISP_X_px, DISP_Y_px );

				//boost::thread th1( boost::bind( &mario::MeasureBasement::measureLoop, &base ) );
				//boost::thread th2( boost::bind( &mario::Display::displayLoop, &disp ) );

				//th1.join();
				//th2.join();
				base.start();
				disp.start();
				bool isBaseEnd = false;
				bool isDispEnd = false;
				while(1){
					auto ms1 = mario::DisplayTimer::getTime();
					if( isBaseEnd == false ){
						base.oneLoop();
						isBaseEnd = base.quitEvent();
						if( isBaseEnd ){
							base.stop();
						}
					}
					if( isDispEnd == false ){
						disp.oneLoop();
						isDispEnd = disp.quitEvent();
						if( isDispEnd ){
							disp.stop();
						}
					}
					auto ms2 = mario::DisplayTimer::getTime();
					static int const FPS = FileIO::getConst("FPS");
					if( ms2 - ms1 < 1000/FPS ){
						disp.wait( 1000/FPS - ( ms2 - ms1 ) );
					}
					auto ms3 = mario::DisplayTimer::getTime();
					cout << "fps == " << 1000/(ms3-ms1) << endl;
				}

				return 1;
			}
			catch( std::exception& ex ){
				cout << ex.what() << endl;
			}
			return -1;
		}
	};
};

int main(int argc, char** argv)
{
	return mario::Main::main();
}