#include "LibBase.h"



bool LibBase::initFlag = false;
bool LibBase::quitFlag = false;

void LibBase::initLib()
{
	assert( LibBase::initFlag==false );
	if( LibBase::initFlag == false ){

		LibBase::initFlag = true;
	}
}

void LibBase::quitLib()
{
	assert( LibBase::quitFlag==false );
	if( LibBase::quitFlag == false ){

		LibBase::quitFlag = true;
	}

}
