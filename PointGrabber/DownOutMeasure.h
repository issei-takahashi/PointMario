#pragma once

#include "MeasureBasement.h"
#include "DownOutMeasure.h"
#include "Coordinate.h"

namespace mario
{
	class DownOutMeasure : public MeasureBasement
	{
	public:
		DownOutMeasure( Eigen::Matrix4d const & _MtoDmat );
		bool collisionDetectionWithCloud( class SolidBody & _obj, double _resolution );
	private:
		void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud);
		void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image);
		bool convertMtoD_withDownAndOut( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _mcloud,
			pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & _dcloud );
	private:
		void setCallBackFunctions();
		static void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie);
		static void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie);
	private:
		Eigen::Matrix4d MtoDMat;
	};
};