#pragma once

#include "MeasureBasement.h"

namespace mario
{
	class MarkerDetecter : public MeasureBasement
	{
	public:
		MarkerDetecter();
		Coordinate<typeM> getJigCenter();
		int getMeasureCount();
	private:
		void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud);
		void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image);

	private:
		void setCallBackFunctions();
		static void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie);
		static void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie);
	private:
		Coordinate<typeM> jigCenter;
		boost::mutex jigCenter_mutex;
		boost::mutex measureCount_mutex;
		int measureCount;
	};
};