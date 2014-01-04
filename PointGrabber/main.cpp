#include "MeasureBasement.h"
#include "FileIO.h"

int main()
{
	try{
		FileIO::loadAllData();
		MeasureBasement base;
		return base.measureLoop();
	}
	catch( std::exception& ex ){
		cout << ex.what() << endl;
	}
	return -1;
}