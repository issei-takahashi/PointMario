#pragma once

#include "MeasureBasement.h"

namespace mario
{
	class PointCapture : public MeasureBasement
	{
	public:
		PointCapture( string const & _fileName );
	private:
		void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud);
		void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image);
		void setCallBackFunctions();
		static void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie);
		static void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie);
	private:
		string const fileName;
		bool isCaptureDone;
		static int const CLOUD_TYPE_NUM = 5;
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr raw;
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr downed;
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr filtered;
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr reded;
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr clustered;
		list< pcl::PointCloud<pcl::PointXYZRGBA>::Ptr > l_cluster;
		
		int showMode; 
	};
};