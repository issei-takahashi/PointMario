#include "FileIO.h"
#include "utils.h"
#include "LibBase.h"

int appMain();

void init()
{
	LibBase::initLib();
	mario::FileIO::loadAllData();
}

void quit()
{
	LibBase::quitLib();
}

int main(int argc, char** argv)
{
	int ret = 0;
	try{
		::init();
		ret = ::appMain();
	}
	catch( std::exception& ex ){
		cout << ex.what() << endl;
	}
	catch( boost::exception & ex ){
		cout << "boost exception" << endl;
	}
	::quit();
	return ret;
}