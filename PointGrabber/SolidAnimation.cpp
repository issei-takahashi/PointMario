#include "SolidAnimation.h"

using namespace mario;

SolidAnimation::SolidAnimation( string const& _folderPath )
	:Animation(_folderPath), velocity( Eigen::Vector3d::Zero() )
{

}


void SolidAnimation::oneLoop( uint _x, uint _y ) 
{
	Animation::oneLoop(_x,_y);
	this->velocityMutex.lock();
	this->addDisplayPoint(this->velocity);
	this->velocityMutex.unlock();
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
	this->velocityMutex.lock();
	this->velocity.x() = _x;
	this->velocity.y() = _y;
	this->velocity.z() = _z;
	this->velocityMutex.unlock();
}

Eigen::Vector3d SolidAnimation::getVelocity() 
{
	this->velocityMutex.lock();
	auto ret = this->velocity;
	this->velocityMutex.unlock();
	return ret;
}


void SolidAnimation::setVelocity( Eigen::Vector3d const & _v )
{
	this->velocityMutex.lock();
	this->velocity = _v;
	this->velocityMutex.unlock();
}
