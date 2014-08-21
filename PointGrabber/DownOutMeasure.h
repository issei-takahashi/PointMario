#pragma once

#include "MeasureBasement.h"
#include "DownOutMeasure.h"
#include "Coordinate.h"

namespace mario
{
	class DownOutMeasure : public MeasureBasement
	{
	public:
		static shared_ptr<DownOutMeasure> makeShared( Eigen::Matrix4d const & _MtoDmat );
		bool simpleCollisionDetectionAndGetHighestPoint   ( class SolidBody & _obj, double _distance, Coordinate<typeD> & _highDst );
		bool collisionDetectionWithCloud_simple   ( class SolidBody & _obj, double _distance );
		bool collisionDetectionWithCloud_voxel   ( class SolidBody & _obj, double _resolution );
		bool collisionDetectionWithCloud_Knearest( class SolidBody & _obj, float _resolution, int _K );
		bool collisionDetectionWithCloud_radius  ( class SolidBody & _obj, float _resolution, float _radius );
	protected:
		DownOutMeasure( Eigen::Matrix4d const & _MtoDmat );
	private:
		void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud);
		void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image);
		void downAndOut( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _src, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & _dst );
		void setCallBackFunctions();
		static void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie);
		static void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie);
	private:
		Eigen::Matrix4d MtoDMat;
	};
};