#pragma once

#include "Coordinate.h"

namespace mario{

	class MeasureBasement
	{
	public:
		MeasureBasement();
		~MeasureBasement();
		void start();
		void stop();
		void oneLoop();
		bool quitEvent();
		bool isCloudEmpty();
	public:
		virtual void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud) = 0;
		virtual void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image) = 0;
		
	public:
		bool displayFlag;
		boost::mutex cld_mutex, img_mutex;
		pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr spcCloud;
		boost::shared_ptr<openni_wrapper::Image> spImage;
		// Create the PCLVisualizer object
		boost::shared_ptr<pcl::visualization::PCLVisualizer> spVisualizer;
		boost::shared_ptr<pcl::visualization::ImageViewer> img;
		unsigned char* rgb_data;
		unsigned rgb_data_size;
		unique_ptr<pcl::Grabber> upGrabberInterface;
		boost::function<void(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&) > bindedFunction;
		boost::signals2::connection signals2Connection;
	private:
		void showCloud();
		void showImage();
		//virtual void setEventHelper() = 0;
		virtual void setCallBackFunctions() = 0;
	protected:
		bool isInitDone;
	};

};