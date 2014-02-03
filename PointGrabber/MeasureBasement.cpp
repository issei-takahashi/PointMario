#include "MeasureBasement.h"
#include "pcl_utils.h"
#include "FileIO.h"

static string const mouseMsg3D ("Mouse coordinates in PCL Visualizer");
static string const keyMsg3D ("Key event for PCL Visualizer");

mario::MeasureBasement::MeasureBasement()
	:isInitDone(false),rgb_data(0), rgb_data_size(0), measureCount(0)
{

}

mario::MeasureBasement::~MeasureBasement()
{

}

void mario::MeasureBasement::start()
{
	this->upEventHelper = ( unique_ptr<EventHelper> )( new EventHelper(this) );
	std::string device_id = "";
	//pcl::console::parse_argument (argc, argv, "-dev", device_id);

	this->upGrabberInterface = ( unique_ptr<pcl::Grabber> )( new pcl::OpenNIGrabber (device_id) );

	this->spVisualizer.reset (new pcl::visualization::PCLVisualizer ("OpenNI Viewer"));

	this->spVisualizer->registerMouseCallback   ( &mario::MeasureBasement::mouse_callback   , (void*)(&mouseMsg3D) );    
	this->spVisualizer->registerKeyboardCallback( &mario::MeasureBasement::keyboard_callback, (void*)(&keyMsg3D) );
	
	// boostのスレッド関連
	this->bindedFunction = boost::bind( &EventHelper::cloud_cb, this->upEventHelper.get(), _1);
	this->signals2Connection = this->upGrabberInterface->registerCallback( this->bindedFunction );

	this->upGrabberInterface->start();
}

void mario::MeasureBasement::stop()
{
	this->cld_mutex.lock();
	this->signals2Connection.disconnect();
	this->bindedFunction.clear();
	this->cld_mutex.unlock();

	this->upEventHelper.reset();
	this->spVisualizer->close();
	this->spVisualizer.reset();
	this->upGrabberInterface->stop();
	this->upGrabberInterface.release();
}

mario::MeasureBasement::EventHelper::EventHelper( mario::MeasureBasement* _aMeasureBasement )
	:aMeasureBasement(_aMeasureBasement)
{

}

void mario::MeasureBasement::EventHelper::cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud)
{
	FPS_CALC ("callback");

	this->aMeasureBasement->cld_mutex.lock ();
	this->aMeasureBasement->redCenter_mutex.lock();

	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud2 ;
	list< pcl::PointCloud<pcl::PointXYZRGBA>::Ptr > l_cluster ;

	if( mario::filterB( cloud, cloud2, l_cluster, this->aMeasureBasement->redCenter ) ){
		//this->aMeasureBasement->redCenter = mario::redDetection(*cloud2);
		this->aMeasureBasement->spcCloud = cloud2->makeShared();
		this->aMeasureBasement->measureCount_mutex.lock();
		this->aMeasureBasement->measureCount++;
		this->aMeasureBasement->measureCount_mutex.unlock();
	}

	this->aMeasureBasement->redCenter_mutex.unlock();
	this->aMeasureBasement->cld_mutex.unlock ();

}

void mario::MeasureBasement::EventHelper::image_callback (const boost::shared_ptr<openni_wrapper::Image>& image)
{
	FPS_CALC ("image callback");

	this->aMeasureBasement->img_mutex.lock ();
	static boost::shared_ptr<openni_wrapper::Image> filtered_image;
	mario::cvt2Mat(image,filtered_image);
	if( filtered_image ){
		this->aMeasureBasement->spImage = image;
	}
	this->aMeasureBasement->img_mutex.unlock ();
}

void mario::MeasureBasement::oneLoop()
{
	// Render and process events in the two interactors
	this->spVisualizer->spinOnce (); // ここで画面更新
	//img->spinOnce ();
	FPS_CALC ("drawing");
	this->showCloud();
	//this->showImage();
	boost::this_thread::sleep (boost::posix_time::microseconds (100));
}

void mario::MeasureBasement::measureLoop()
{
	// Loop
	while ( !this->spVisualizer->wasStopped() ){
		this->oneLoop();
	}
}

bool mario::MeasureBasement::quitEvent()
{
	return this->spVisualizer->wasStopped();
}

bool mario::MeasureBasement::isCloudEmpty()
{
	this->cld_mutex.lock();
	bool ret = false;
	if( this->spcCloud ){
		ret = this->spcCloud->empty();
	}
	this->cld_mutex.unlock();
	return ret;
}

mario::Coordinate<mario::typeM> mario::MeasureBasement::getRedCenter()
{
	this->redCenter_mutex.lock();
	auto ret = this->redCenter * 1000; 
	this->redCenter_mutex.unlock();
	return ret;
}

int mario::MeasureBasement::getMeasureCount()
{
	this->measureCount_mutex.lock();
	int ret = this->measureCount;
	this->measureCount_mutex.unlock();
	return ret;
}

void mario::MeasureBasement::captureForExperiment1002()
{

}

void mario::MeasureBasement::showCloud()
{
	// Add the cloud
	if (spcCloud && cld_mutex.try_lock ())
	{
		// 初期化
		if (!this->isInitDone)
		{
			spVisualizer->getRenderWindow ()->SetSize (640, 480);
			spVisualizer->getRenderWindow ()->SetPosition (200,200);
			this->isInitDone = true;
		}

		pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGBA> handler (spcCloud);
		// ここでspcCloudからビュワーに点群を同期
		if (!spVisualizer->updatePointCloud (spcCloud, handler, "OpenNICloud"))
		{
			spVisualizer->addPointCloud (spcCloud, handler, "OpenNICloud");
			spVisualizer->resetCameraViewpoint ("OpenNICloud");
		}

		cld_mutex.unlock ();
	}
}

void mario::MeasureBasement::showImage()
{
	// Add the image
	if( spImage && img_mutex.try_lock() )
	{
		if( spImage->getEncoding() == openni_wrapper::Image::RGB )
			img->showRGBImage (spImage->getMetaData ().Data (), 
			spImage->getWidth (), spImage->getHeight ());
		else{
			if (rgb_data_size < spImage->getWidth () * spImage->getHeight ()){
				rgb_data_size = spImage->getWidth () * spImage->getHeight ();
				if( rgb_data ) delete rgb_data;
				rgb_data = new unsigned char [rgb_data_size * 3];
			}
			spImage->fillRGB (spImage->getWidth (), spImage->getHeight (), rgb_data);
			img->showRGBImage (rgb_data, spImage->getWidth (), spImage->getHeight ());
		}
		img_mutex.unlock ();
	}
}

// Simple callbacks.
void mario::MeasureBasement::keyboard_callback( const pcl::visualization::KeyboardEvent& _evt, void* cookie )
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

void mario::MeasureBasement::mouse_callback( const pcl::visualization::MouseEvent& mouse_event, void* cookie )
{
	std::string* message = (std::string*) cookie;
	// 左クリック
	if (mouse_event.getType() == pcl::visualization::MouseEvent::MouseButtonPress &&
		mouse_event.getButton() == pcl::visualization::MouseEvent::LeftButton)
	{
		///cout << (*message) << " :: " << mouse_event.getX () << " , " << mouse_event.getY () << endl;
		
	}
}