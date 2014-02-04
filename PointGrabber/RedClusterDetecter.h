#pragma once

#include "MeasureBasement.h"

namespace mario
{
	class RedClusterDetecter : public MeasureBasement
	{
	public:
		RedClusterDetecter();
		Coordinate<typeM> getRedCenter();
		int getMeasureCount();
	private:
		void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud);
		void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image);

	private:
		void setCallBackFunctions();
		static void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie);
		static void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie);
	private:
		Coordinate<typeM> redCenter;
		boost::mutex redCenter_mutex;
		boost::mutex measureCount_mutex;
		int measureCount;
	};
};