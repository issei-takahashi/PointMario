#pragma once

#include "Animation.h"
#include "CollisionInterface.h"

namespace mario{
	class SolidAnimation : public Animation, public CollisionInterface
	{
	public:
		SHARED(SolidAnimation);
	public:
		static SolidAnimation::Ptr makeShared( string const& _folderPath );
		void oneLoop( uint _x, uint _y ) final override;
		pcl::PointXYZRGBA const getSearchPoint() const final override;
	
		void setVelocity( double _x, double _y, double _z );
		void setVelocity( Eigen::Vector3d const & _v ){ this->velocity = _v; }
		Eigen::Vector3d getVelocity() const { return this->velocity; }
	private:
		SolidAnimation( string const& _folderPath );
		Eigen::Vector3d velocity;
	};
};