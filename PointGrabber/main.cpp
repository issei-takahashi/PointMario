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

				boost::thread th1( boost::bind( &mario::MeasureBasement::measureLoop, &base ) );
				boost::thread th2( boost::bind( &mario::Display::displayLoop, &disp ) );

				th1.join();
				th2.join();

				//base.measureLoop();

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