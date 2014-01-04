#include "MeasureBasement.h"

int main()
{
	MeasureBasement base;
	try{
		return base.measureLoop();
	}
	catch( std::exception& ex ){
		cout << ex.what() << endl;
	}
	return -1;
}