#include "Base.h"
#include "pcl_utils.h"

void Base::printHelp (int argc, char **argv)
{
	using pcl::console::print_error;
	using pcl::console::print_info;
	print_error ("Syntax is: %s <options>\n", argv[0]);
	print_info ("  where options are:\n");
	print_info ("                     -dev device_id           = device to be used\n");
	print_info ("                                                maybe \"#n\", with n being the number of the device in device list.\n");
	print_info ("                                                maybe \"bus@addr\", with bus and addr being the usb bus and address where device is connected.\n");
	print_info ("                                                maybe \"serial\", with serial being the serial number of the device.\n");
	print_info ("\n");
}


Base::EventHelper::EventHelper( Base* _aBase )
	:aBase(_aBase)
{

}

void Base::EventHelper::cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud)
{
	FPS_CALC ("callback");

	this->aBase->cld_mutex.lock ();	

	static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr filtered_cloud;
	issei::filterA(cloud,filtered_cloud);
	if( filtered_cloud ){
		this->aBase->g_cloud = filtered_cloud->makeShared();
	}

	this->aBase->cld_mutex.unlock ();

}

void Base::EventHelper::image_callback (const boost::shared_ptr<openni_wrapper::Image>& image)
{
	FPS_CALC ("image callback");

	this->aBase->img_mutex.lock ();
	static boost::shared_ptr<openni_wrapper::Image> filtered_image;
	issei::cvt2Mat(image,filtered_image);
	if( filtered_image ){
		this->aBase->g_image = image;
	}
	this->aBase->img_mutex.unlock ();
}


// Simple callbacks.
void keyboard_callback (const pcl::visualization::KeyboardEvent& event, void* cookie)
{
	std::string* message = (std::string*)cookie;
	cout << (*message) << " :: ";
	if (event.getKeyCode())
		cout << "the key \'" << event.getKeyCode() << "\' (" << (int)event.getKeyCode() << ") was";
	else
		cout << "the special key \'" << event.getKeySym() << "\' was";
	if (event.keyDown())
		cout << " pressed" << endl;
	else
		cout << " released" << endl;
}

void mouse_callback (const pcl::visualization::MouseEvent& mouse_event, void* cookie)
{
	std::string* message = (std::string*) cookie;
	if (mouse_event.getType() == pcl::visualization::MouseEvent::MouseButtonPress && mouse_event.getButton() == pcl::visualization::MouseEvent::LeftButton)
	{
		cout << (*message) << " :: " << mouse_event.getX () << " , " << mouse_event.getY () << endl;
	}
}

int Base::run ()
{

	EventHelper event_helper(this);
	std::string device_id = "";
	//pcl::console::parse_argument (argc, argv, "-dev", device_id);

	pcl::Grabber* interface = new pcl::OpenNIGrabber (device_id);

	cld.reset (new pcl::visualization::PCLVisualizer ("OpenNI Viewer"));

	std::string mouseMsg3D ("Mouse coordinates in PCL Visualizer");
	std::string keyMsg3D ("Key event for PCL Visualizer");
	cld->registerMouseCallback (&mouse_callback, (void*)(&mouseMsg3D));    
	cld->registerKeyboardCallback(&keyboard_callback, (void*)(&keyMsg3D));
	boost::function<void(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&) > f = boost::bind (&EventHelper::cloud_cb, &event_helper, _1);
	boost::signals2::connection c1 = interface->registerCallback (f);

	img.reset (new pcl::visualization::ImageViewer ("OpenNI Viewer"));
	// Register callbacks
	std::string keyMsg2D ("Key event for image viewer");
	std::string mouseMsg2D ("Mouse coordinates in image viewer");
	img->registerMouseCallback (&mouse_callback, (void*)(&mouseMsg2D));
	img->registerKeyboardCallback(&keyboard_callback, (void*)(&keyMsg2D));
	boost::function<void (const boost::shared_ptr<openni_wrapper::Image>&) > image_cb = boost::bind (&EventHelper::image_callback, &event_helper, _1);
	boost::signals2::connection image_connection = interface->registerCallback (image_cb);
	unsigned char* rgb_data = 0;
	unsigned rgb_data_size = 0;

	interface->start ();
	bool cld_init = false;
	// Loop
	while (!cld->wasStopped ())
	{
		// Render and process events in the two interactors
		cld->spinOnce (); // ここで画面更新
		img->spinOnce ();
		FPS_CALC ("drawing");

		// Add the cloud
		if (g_cloud && cld_mutex.try_lock ())
		{
			// 初期化
			if (!cld_init)
			{
				cld->getRenderWindow ()->SetSize (640, 480);
				cld->getRenderWindow ()->SetPosition (200,200);
				cld_init = !cld_init;
			}

			pcl::visualization::PointCloudColorHandlerRGBField<pcl::PointXYZRGBA> handler (g_cloud);
			// ここでg_cloudからビュワーに点群を同期
			if (!cld->updatePointCloud (g_cloud, handler, "OpenNICloud"))
			{
				cld->addPointCloud (g_cloud, handler, "OpenNICloud");
				cld->resetCameraViewpoint ("OpenNICloud");
			}

			cld_mutex.unlock ();
		}

		// Add the image
		if (g_image && img_mutex.try_lock ())
		{
			if (g_image->getEncoding() == openni_wrapper::Image::RGB)
				img->showRGBImage (g_image->getMetaData ().Data (), 
				g_image->getWidth (), g_image->getHeight ());
			else
			{
				if (rgb_data_size < g_image->getWidth () * g_image->getHeight ())
				{
					rgb_data_size = g_image->getWidth () * g_image->getHeight ();
					rgb_data = new unsigned char [rgb_data_size * 3];
				}
				g_image->fillRGB (g_image->getWidth (), g_image->getHeight (), rgb_data);
				img->showRGBImage (rgb_data, g_image->getWidth (), g_image->getHeight ());
			}
			img_mutex.unlock ();
		}
		boost::this_thread::sleep (boost::posix_time::microseconds (100));
	}

	interface->stop ();

	return 1;
}
