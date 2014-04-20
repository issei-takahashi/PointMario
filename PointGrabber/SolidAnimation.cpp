#include "SolidAnimation.h"

using namespace mario;

SolidAnimation::SolidAnimation( string const& _folderPath )
	:Animation(_folderPath)
{

}

SolidAnimation::Ptr SolidAnimation::makeShared( string const& _folderPath )
{
	return (SolidAnimation::Ptr)(new SolidAnimation(_folderPath));
}
