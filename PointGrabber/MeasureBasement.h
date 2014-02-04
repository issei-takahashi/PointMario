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
		int getMeasureCount();
		void captureForExperiment1002();
	public:
		struct EventHelper
		{
			EventHelper( MeasureBasement* _aMeasureBasement );
			MeasureBasement* const aMeasureBasement;
			virtual void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud) = 0;
			virtual void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image) = 0;
		};
	public:
		boost::mutex cld_mutex, img_mutex, measureCount_mutex;
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
		virtual void setEventHelper() = 0;
		virtual void setCallBackFunctions() = 0;
	private:
		bool isInitDone;
		int measureCount;
	};

	class RedClusterDetecter : public MeasureBasement
	{
	public:
		Coordinate<typeM> getRedCenter();
	private:
		struct RedClusterDetectEventer : public MeasureBasement::EventHelper
		{
			RedClusterDetectEventer( MeasureBasement* _aMeasureBasement );
			void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud);
			void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image);
		};
	private:
		void setCallBackFunctions();
		void setEventHelper();
		static void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie);
		static void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie);
	private:
		Coordinate<typeM> redCenter;
		boost::mutex  redCenter_mutex;
	};
};