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
	std::string device_id = "";
	//pcl::console::parse_argument (argc, argv, "-dev", device_id);

	this->upGrabberInterface = ( unique_ptr<pcl::Grabber> )( new pcl::OpenNIGrabber (device_id) );

	this->spVisualizer.reset (new pcl::visualization::PCLVisualizer ("OpenNI Viewer"));

	this->setCallBackFunctions();
	
	// boostのスレッド関連
	this->bindedFunction = boost::bind( &MeasureBasement::cloud_cb, this, _1);
	this->signals2Connection = this->upGrabberInterface->registerCallback( this->bindedFunction );

	this->upGrabberInterface->start();
}

void mario::MeasureBasement::stop()
{
	this->cld_mutex.lock();
	this->signals2Connection.disconnect();
	this->bindedFunction.clear();
	this->cld_mutex.unlock();

	this->spVisualizer->close();
	this->spVisualizer.reset();
	this->upGrabberInterface->stop();
	this->upGrabberInterface.release();
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
