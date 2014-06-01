#pragma once

#include "Animation.h"
#include "CollisionInterface.h"

namespace mario{
	class SolidAnimation : public Animation, public CollisionInterface
	{
	public:
		SHARED(SolidAnimation);
	public:
		SolidAnimation( string const& _folderPath, shared_ptr<class Application> _owner );
		pcl::PointXYZRGBA const getSearchPoint() final override;
		void oneLoop();
		void setVelocity( double _x, double _y, double _z );
		void setVelocity( Eigen::Vector3d const & _v );
		Eigen::Vector3d getVelocity();
	private:
		boost::mutex velocityMutex;
		Eigen::Vector3d velocity;
		void oneLoop( uint _x, uint _y ) final override;
	};
};