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
	vec[0] = dp.x;
	vec[1] = dp.y;
	vec[2] = dp.z;
	vec[3] = 1;
	//cout << "D : (" << vec[0] << "," << vec[1] << "," << vec[2] << "," << vec[3] << ") " << endl;
	Eigen::Vector4d vec2;
	if( auto sp = this->ownerApp.lock() ){
		vec2 = sp->getDtoMMat() * vec;
		//cout << "M : (" << vec2[0] << "," << vec2[1] << "," << vec2[2] << ") " << endl;
	}
	static pcl::PointXYZRGBA ret;
	ret.x = vec2[0]/1000;
	ret.y = vec2[1]/1000;
	ret.z = vec2[2]/1000;
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