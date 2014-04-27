#include "SolidAnimation.h"

using namespace mario;

SolidAnimation::SolidAnimation( string const& _folderPath )
	:Animation(_folderPath)
{

}


void SolidAnimation::oneLoop( uint _x, uint _y ) 
{
	Animation::oneLoop(_x,_y);
	this->addDisplayPoint(this->velocity);
}

pcl::PointXYZRGBA const SolidAnimation::getSearchPoint()
{
	auto dp = this->getDisplayPoint();
	pcl::PointXYZRGBA ret;
	ret.x = dp.x;
	ret.y = dp.y;
	ret.z = dp.y;
	return ret;
}

void SolidAnimation::setVelocity( double _x, double _y, double _z )
{
	this->velocity.x() = _x;
	this->velocity.y() = _y;
	this->velocity.z() = _z;
}