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
				mario::Display disp;

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