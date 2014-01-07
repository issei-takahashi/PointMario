#include "MeasureBasement.h"
#include "pcl_utils.h"

mario::MeasureBasement::MeasureBasement()
	:isInitDone(false),rgb_data(0), rgb_data_size(0)
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

	this->cld.reset (new pcl::visualization::PCLVisualizer ("OpenNI Viewer"));

	std::string mouseMsg3D ("Mouse coordinates in PCL Visualizer");
	std::string keyMsg3D ("Key event for PCL Visualizer");
	this->cld->registerMouseCallback   ( &mario::MeasureBasement::mouse_callback   , (void*)(&mouseMsg3D) );    
	this->cld->registerKeyboardCallback( &mario::MeasureBasement::keyboard_callback, (void*)(&keyMsg3D) );
	
	// boostのスレッド関連
	boost::function<void(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&) > f = boost::bind (&EventHelper::cloud_cb, this->upEventHelper.get(), _1);
	boost::signals2::connection c1 = this->upGrabberInterface->registerCallback (f);

	this->upGrabberInterface->start();
}

void mario::MeasureBasement::stop()
{
	this->upGrabberInterface->stop();
}

mario::MeasureBasement::EventHelper::EventHelper( mario::MeasureBasement* _aMeasureBasement )
	:aMeasureBasement(_aMeasureBasement)
{

}

void mario::MeasureBasement::EventHelper::cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud)
{
	FPS_CALC ("callback");

	this->aMeasureBasement->cld_mutex.lock ();	

	static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr filtered_cloud;
	mario::filterA(cloud,filtered_cloud);
	if( filtered_cloud ){
		this->aMeasureBasement->spcCloud = filtered_cloud->makeShared();
	}

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


int mario::MeasureBasement::simpleViewLoop ()
{

	EventHelper event_helper(this);
	std::string device_id = "";
	//pcl::console::parse_argument (argc, argv, "-dev", device_id);

	pcl::Grabber* interface = new pcl::OpenNIGrabber (device_id);

	cld.reset (new pcl::visualization::PCLVisualizer ("OpenNI Viewer"));

	std::string mouseMsg3D ("Mouse coordinates in PCL Visualizer");
	std::string keyMsg3D ("Key event for PCL Visualizer");
	cld->registerMouseCallback (&mario::MeasureBasement::mouse_callback, (void*)(&mouseMsg3D));    
	cld->registerKeyboardCallback(&mario::MeasureBasement::keyboard_callback, (void*)(&keyMsg3D));
	boost::function<void(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&) > f = boost::bind (&EventHelper::cloud_cb, &event_helper, _1);
	boost::signals2::connection c1 = interface->registerCallback (f);

	//img.reset (new pcl::visualization::ImageViewer ("OpenNI Viewer"));
	//// Register callbacks
	//std::string keyMsg2D ("Key event for image viewer");
	//std::string mouseMsg2D ("Mouse coordinates in image viewer");
	//img->registerMouseCallback (&mouse_callback, (void*)(&mouseMsg2D));
	//img->registerKeyboardCallback(&keyboard_callback, (void*)(&keyMsg2D));
	//boost::function<void (const boost::shared_ptr<openni_wrapper::Image>&) > image_cb = boost::bind (&EventHelper::image_callback, &event_helper, _1);
	//boost::signals2::connection image_connection = interface->registerCallback (image_cb);

	interface->start ();

	// Loop
	while (!cld->wasStopped ())
	{
		// Render and process events in the two interactors
		cld->spinOnce (); // ここで画面更新
		//img->spinOnce ();
		FPS_CALC ("drawing");

		this->showCloud();
		//this->showImage();


		boost::this_thread::sleep (boost::posix_time::microseconds (100));
	}

	interface->stop ();

	return 1;
}


void mario::MeasureBasement::oneLoop()
{
	// Render and process events in the two interactors
	this->cld->spinOnce (); // ここで画面更新
	//img->spinOnce ();
	FPS_CALC ("drawing");
	this->showCloud();
	//this->showImage();
	boost::this_thread::sleep (boost::posix_time::microseconds (100));
}

void mario::MeasureBasement::measureLoop()
{
	// Loop
	while ( !this->cld->wasStopped() ){
		this->oneLoop();
	}
}

bool mario::MeasureBasement::quitEvent()
{
	return this->cld->wasStopped();
}

void mario::MeasureBasement::showCloud()
{
	// Add the cloud
	if (spcCloud && cld_mutex.try_lock ())
	{
		// 初期化
		if (!this->isInitDone)
		{
			cld->getRenderWindow ()->SetSize (640, 480);
			cld->getRenderWindow ()->SetPosition (200,200);
			this->isInitDone = true;
		}

		pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGBA> handler (spcCloud);
		// ここでspcCloudからビュワーに点群を同期
		if (!cld->updatePointCloud (spcCloud, handler, "OpenNICloud"))
		{
			cld->addPointCloud (spcCloud, handler, "OpenNICloud");
			cld->resetCameraViewpoint ("OpenNICloud");
		}

		cld_mutex.unlock ();
	}
}

void mario::MeasureBasement::showImage()
{
	// Add the image
	if (spImage && img_mutex.try_lock ())
	{
		if (spImage->getEncoding() == openni_wrapper::Image::RGB)
			img->showRGBImage (spImage->getMetaData ().Data (), 
			spImage->getWidth (), spImage->getHeight ());
		else
		{
			if (rgb_data_size < spImage->getWidth () * spImage->getHeight ())
			{
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
	std::string* message = (std::string*)cookie;
	cout << (*message) << " :: ";
	if (_evt.getKeyCode()){
		switch(_evt.getKeyCode()){
		case 'r': // 録画開始
			{

				break;
			}
		}
	}
	else{
		cout << "the special key \'" << _evt.getKeySym() << "\' was";
	}
	if (_evt.keyDown()){
		cout << " pressed" << endl;
	}
	else{
		cout << " released" << endl;
	}
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