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
		void measureLoop();
		bool quitEvent();
		bool isCloudEmpty();
		Coordinate<typeM> getRedCenter();
		int getMeasureCount();
	public:
		struct EventHelper
		{
			EventHelper( MeasureBasement* _aMeasureBasement );
			MeasureBasement* const aMeasureBasement;
			void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud);
			void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image);
		};
	public:
		boost::mutex cld_mutex, img_mutex, measureCount_mutex, redCenter_mutex;
		pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr spcCloud;
		boost::shared_ptr<openni_wrapper::Image> spImage;
		// Create the PCLVisualizer object
		boost::shared_ptr<pcl::visualization::PCLVisualizer> spVisualizer;
		boost::shared_ptr<pcl::visualization::ImageViewer> img;
		unsigned char* rgb_data;
		unsigned rgb_data_size;
		unique_ptr<pcl::Grabber> upGrabberInterface;
		unique_ptr<EventHelper> upEventHelper;
		boost::function<void(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&) > bindedFunction;
		boost::signals2::connection signals2Connection;
	private:
		void showCloud();
		void showImage();
		static void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie);
		static void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie);

	private:
		bool isInitDone;
		int measureCount;
		Coordinate<typeM> redCenter;
	};

};