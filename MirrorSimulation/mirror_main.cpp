#include "StdAfx.h"
#include <pcl/io/openni_grabber.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/io/pcd_io.h>
#include <string>
#include <iostream>

using namespace std;

class SimpleOpenNIViewer
{
public:
	SimpleOpenNIViewer () : viewer ("PCL OpenNI Viewer") {}

	void cloud_cb_ (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud)
	{
		if (!viewer.wasStopped()){
			viewer.showCloud (cloud);
		}
		//pcl::io::savePCDFileBinary("frame.pcd", *cloud);
	}

	void run ()
	{
		pcl::Grabber* grab = new pcl::OpenNIGrabber();

		boost::function<void (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&)> f =
			boost::bind (&SimpleOpenNIViewer::cloud_cb_, this, _1);

		grab->registerCallback (f);

		grab->start ();

		while (!viewer.wasStopped())
		{
			
			//boost::this_thread::sleep( boost::posix_time::milliseconds(2000) ); 
		}

		grab->stop ();
	}

	pcl::visualization::CloudViewer viewer;
};

int main ()
{
	SimpleOpenNIViewer v;
	v.run ();
	return 0;
}