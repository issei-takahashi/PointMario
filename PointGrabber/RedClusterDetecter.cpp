#include "RedClusterDetecter.h"
#include "pcl_utils.h"
#include "FileIO.h"

mario::RedClusterDetecter::RedClusterDetecter()
 :MeasureBasement(), measureCount(0)
{}

// Simple callbacks.
void mario::RedClusterDetecter::keyboard_callback( const pcl::visualization::KeyboardEvent& _evt, void* cookie )
{
	//std::string* message = (std::string*)cookie;
	//cout << (*message) << " :: ";
	//if (_evt.getKeyCode()){
	//	switch(_evt.getKeyCode()){
	//	case 'r': // 録画開始
	//		{
	//			cout << "rec start" << endl;
	//			break;
	//		}
	//	}
	//}
	//else{
	//	cout << "the special key \'" << _evt.getKeySym() << "\' was";
	//}
	//if (_evt.keyDown()){
	//	cout << " pressed" << endl;
	//}
	//else{
	//	cout << " released" << endl;
	//}
}

void mario::RedClusterDetecter::mouse_callback( const pcl::visualization::MouseEvent& mouse_event, void* cookie )
{
	std::string* message = (std::string*) cookie;
	// 左クリック
	if (mouse_event.getType() == pcl::visualization::MouseEvent::MouseButtonPress &&
		mouse_event.getButton() == pcl::visualization::MouseEvent::LeftButton)
	{
		///cout << (*message) << " :: " << mouse_event.getX () << " , " << mouse_event.getY () << endl;
		
	}
}


void mario::RedClusterDetecter::cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud)
{
	FPS_CALC ("callback");

	this->cld_mutex.lock ();
	this->redCenter_mutex.lock();

	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud2 ;
	list< pcl::PointCloud<pcl::PointXYZRGBA>::Ptr > l_cluster ;

	if( mario::filterB( cloud, cloud2, l_cluster, this->redCenter ) ){
		this->spcCloud = cloud2->makeShared();
		this->measureCount_mutex.lock();
		this->measureCount++;
		this->measureCount_mutex.unlock();
	}

	this->redCenter_mutex.unlock();
	this->cld_mutex.unlock ();

}

void mario::RedClusterDetecter::image_callback ( const boost::shared_ptr<openni_wrapper::Image>& image )
{
	FPS_CALC ("image callback");

	this->img_mutex.lock ();
	static boost::shared_ptr<openni_wrapper::Image> filtered_image;
	mario::cvt2Mat(image,filtered_image);
	if( filtered_image ){
		this->spImage = image;
	}
	this->img_mutex.unlock ();
}

mario::Coordinate<mario::typeM> mario::RedClusterDetecter::getRedCenter()
{
	this->redCenter_mutex.lock();
	auto ret = this->redCenter * 1000; 
	this->redCenter_mutex.unlock();
	return ret;
}

int mario::RedClusterDetecter::getMeasureCount()
{
	this->measureCount_mutex.lock();
	int ret = this->measureCount;
	this->measureCount_mutex.unlock();
	return ret;
}

static string const mouseMsg3D ("Mouse coordinates in PCL Visualizer");
static string const keyMsg3D ("Key event for PCL Visualizer");
void mario::RedClusterDetecter::setCallBackFunctions()
{
	this->spVisualizer->registerMouseCallback   ( &mario::RedClusterDetecter::mouse_callback   , (void*)(&mouseMsg3D) );    
	this->spVisualizer->registerKeyboardCallback( &mario::RedClusterDetecter::keyboard_callback, (void*)(&keyMsg3D) );
}
