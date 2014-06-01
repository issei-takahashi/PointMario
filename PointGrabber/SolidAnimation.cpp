#include "SolidAnimation.h"
#include "Application.h"

using namespace mario;

SolidAnimation::SolidAnimation( string const& _folderPath, shared_ptr<class Application> _owner )
	:Animation(_folderPath),
	CollisionInterface(_owner),
	velocity( Eigen::Vector3d::Zero() )
{

}


void SolidAnimation::oneLoop() 
{
	this->velocityMutex.lock();
	this->addDisplayPoint(this->velocity);
	this->velocityMutex.unlock();

	auto dp = this->getDisplayPoint();
	Animation::oneLoop(dp.x,dp.y);
}

void SolidAnimation::oneLoop( uint _x, uint _y )
{
	this->oneLoop();
}

pcl::PointXYZRGBA const SolidAnimation::getSearchPoint()
{
	auto dp = this->getDisplayPoint();
	Eigen::Vector4d vec;
	vec.x() = dp.x;
	vec.y() = dp.y;
	vec.z() = dp.z;
	vec.w() = 1;
	if( auto sp = this->ownerApp.lock() ){
		vec = sp->getMtoDMat().inverse() * vec;
	}
	pcl::PointXYZRGBA ret;
	ret.x = vec.x();
	ret.y = vec.y();
	ret.z = vec.z();
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
