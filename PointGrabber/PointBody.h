#pragma once

#include "SolidBody.h"
#include "Coordinate.h"

namespace mario{
	class PointBody : public SolidBody
	{
	public:
		SHARED(PointBody);
	public:
		static shared_ptr<PointBody> makeShared( shared_ptr<class Application> _owner );
		pcl::PointXYZRGBA const getSearchPoint() final override;
		void oneLoop();
		// set
		void setVelocity( double _x, double _y, double _z );
		void setVelocity( Eigen::Vector3d const & _v );
		void setPoint( Coordinate<typeD> pt ){ this->point = pt; }
		// get
		Eigen::Vector3d getVelocity();
		Coordinate<typeD> getPoint() const { return this->point; }
	private:
		PointBody( shared_ptr<class Application> _owner );
		Coordinate<typeD> point;
		boost::mutex velocityMutex;
		Eigen::Vector3d velocity;
	};
};