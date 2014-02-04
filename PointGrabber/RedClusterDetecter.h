#pragma once

#include "MeasureBasement.h"

namespace mario
{
	class RedClusterDetecter : public MeasureBasement
	{
	public:
		Coordinate<typeM> getRedCenter();
	private:
			void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud);
			void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image);

	private:
		void setCallBackFunctions();
		//void setEventHelper();
		static void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie);
		static void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie);
	private:
		Coordinate<typeM> redCenter;
		boost::mutex  redCenter_mutex;
	};
};