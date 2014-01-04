#pragma once

class MeasureBasement
{
public:
	MeasureBasement();
	int simpleViewLoop();
	int measureLoop();
public:
	boost::mutex cld_mutex, img_mutex;
	pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr spcCloud;
	boost::shared_ptr<openni_wrapper::Image> spImage;
	// Create the PCLVisualizer object
	boost::shared_ptr<pcl::visualization::PCLVisualizer> cld;
	boost::shared_ptr<pcl::visualization::ImageViewer> img;
	unsigned char* rgb_data;
	unsigned rgb_data_size;
public:
	struct EventHelper
	{
		EventHelper( MeasureBasement* _aMeasureBasement );
		MeasureBasement* const aMeasureBasement;
		void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud);
		void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image);
	};
private:
	void showCloud();
	void showImage();
	static void keyboard_callback (const pcl::visualization::KeyboardEvent& _evt, void* cookie);
	static void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie);

private:
	bool isInitDone;
};