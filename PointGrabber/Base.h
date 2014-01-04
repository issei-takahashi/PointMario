#pragma once

class Base
{
public:
	boost::mutex cld_mutex, img_mutex;
	pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr g_cloud;
	boost::shared_ptr<openni_wrapper::Image> g_image;

	void printHelp (int argc, char **argv);

	// Create the PCLVisualizer object
	boost::shared_ptr<pcl::visualization::PCLVisualizer> cld;
	boost::shared_ptr<pcl::visualization::ImageViewer> img;


	struct EventHelper
	{
		EventHelper( Base* _aBase );
		Base* const aBase;
		void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud);
		void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image);
	};
	
	int run ();
};