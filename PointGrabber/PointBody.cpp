#include "PointBody.h"
#include "Application.h"

using namespace mario;

shared_ptr<PointBody> PointBody::makeShared( shared_ptr<class Application> _owner )
{
	return (shared_ptr<PointBody>)(new PointBody(_owner));
}

PointBody::PointBody(  shared_ptr<class Application> _owner )
	:SolidBody(_owner),
	velocity( Eigen::Vector3d::Zero() )
{

}

pcl::PointXYZRGBA const PointBody::getSearchPoint()
{
	auto dp = this->point;
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

void PointBody::setVelocity( double _x, double _y, double _z )
{
	this->velocityMutex.lock();
	this->velocity.x() = _x;
	this->velocity.y() = _y;
	this->velocity.z() = _z;
	this->velocityMutex.unlock();
}

Eigen::Vector3d PointBody::getVelocity() 
{
	this->velocityMutex.lock();
	auto ret = this->velocity;
	this->velocityMutex.unlock();
	return ret;
}


void PointBody::setVelocity( Eigen::Vector3d const & _v )
{
	this->velocityMutex.lock();
	this->velocity = _v;
	this->velocityMutex.unlock();
}

void PointBody::oneLoop()
{
	this->velocityMutex.lock();
	auto v = this->velocity;
	this->velocityMutex.unlock();
	this->point += v;
}