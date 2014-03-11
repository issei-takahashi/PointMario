#include "FileIO.h"
#include "utils.h"

int appMain();

void init()
{
	mario::FileIO::loadAllData();
}

int main(int argc, char** argv)
{
	try{
		::init();
		return ::appMain();
	}
	catch( std::exception& ex ){
		cout << ex.what() << endl;
	}
	catch( boost::exception & ex ){
		cout << "boost exception" << endl;
	}
}