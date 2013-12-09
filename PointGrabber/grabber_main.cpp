#include "main.h"
#include "defines.h"
#include "pcl_utils.h"

using namespace std;

class SimpleOpenNIViewer
{
public:
	SimpleOpenNIViewer () : viewer ("PCL OpenNI Viewer") {}

	void cloud_cb_ (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr &cloud)
	{
		if (!viewer.wasStopped()){
			static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_pass_filterd (new pcl::PointCloud<pcl::PointXYZRGBA>);
			static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_down_filtered (new pcl::PointCloud<pcl::PointXYZRGBA>);
			// はずれ値除去フィルタ
			this->pass.setInputCloud (cloud);
			///pass.setFilterLimitsNegative (true);
			this->pass.filter (*cloud_pass_filterd);
			// ダウンサンプリングフィルタ
			this->sor.setInputCloud (cloud_pass_filterd);
			this->sor.filter (*cloud_down_filtered);
			// 平面抽出
			issei::segmentate( *cloud_down_filtered, 0.005 );
			// 表示
			viewer.showCloud (cloud_down_filtered);
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

		// 外れ値除去フィルタの設定
		this->pass.setFilterFieldName ("z");
		this->pass.setFilterLimits (0.0, 0.8);
		// ダウンサンプリングフィルタの設定
		this->sor.setLeafSize (0.01f, 0.01f, 0.01f);

		while (!viewer.wasStopped())
		{

			//boost::this_thread::sleep( boost::posix_time::milliseconds(2000) ); 
		}

		grab->stop ();
	}

	pcl::visualization::CloudViewer viewer;
	pcl::PassThrough<pcl::PointXYZRGBA> pass; // 外れ値除去フィルタ
	pcl::VoxelGrid< pcl::PointXYZRGBA > sor; // ダウンサンプリングフィルタ
};


int main ()
{
	SimpleOpenNIViewer v;
	v.run ();
	return 0;
}