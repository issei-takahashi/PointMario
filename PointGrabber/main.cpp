#include "MeasureBasement.h"
#include "FileIO.h"

int main()
{
	try{
		mario::FileIO::loadAllData();
		mario::MeasureBasement base;
		return base.measureLoop();
	}
	catch( std::exception& ex ){
		cout << ex.what() << endl;
	}
	return -1;
}