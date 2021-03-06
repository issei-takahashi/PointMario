#if 0

#include "pcl_utils.h"

#define MEASURE_FUNCTION_TIME
//#define SHOW_IMAGE !((VTK_MAJOR_VERSION == 5)&&(VTK_MINOR_VERSION <= 4))
#define SHOW_IMAGE 1

#define SHOW_FPS 1
#if SHOW_FPS
#define FPS_CALC(_WHAT_) \
	do \
{ \
	static unsigned count = 0;\
	static double last = pcl::getTime ();\
	double now = pcl::getTime (); \
	++count; \
	if (now - last >= 1.0) \
	{ \
	std::cout << "Average framerate("<< _WHAT_ << "): " << double(count)/double(now - last) << " Hz" <<  std::endl; \
	count = 0; \
	last = now; \
	} \
}while(false)
#else
#define FPS_CALC(_WHAT_) \
	do \
{ \
}while(false)
#endif

boost::mutex cld_mutex, img_mutex;
pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr spcCloud;
boost::shared_ptr<openni_wrapper::Image> spImage;

void printHelp (int argc, char **argv)
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

// Create the PCLVisualizer object
boost::shared_ptr<pcl::visualization::PCLVisualizer> cld;
#if SHOW_IMAGE
boost::shared_ptr<pcl::visualization::ImageViewer> img;
#endif

struct EventHelper
{
	void cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud)
	{
		FPS_CALC ("callback");

		cld_mutex.lock ();	
		static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr filtered_cloud;
		mario::filterA(cloud,filtered_cloud);
		if( filtered_cloud ){
			spcCloud = filtered_cloud->makeShared();
		}
		cld_mutex.unlock ();
	}

#if SHOW_IMAGE
	void image_callback (const boost::shared_ptr<openni_wrapper::Image>& image)
	{
		FPS_CALC ("image callback");

		img_mutex.lock ();
		static boost::shared_ptr<openni_wrapper::Image> filtered_image;
		mario::cvt2Mat(image,filtered_image);
		if( filtered_image ){
			spImage = image;
		}
		img_mutex.unlock ();
	}
#endif  
};
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

/* ---[ */
int main (int argc, char** argv)
{
	if (argc > 1)
	{
		for (int i = 1; i < argc; i++)
		{
			if (std::string (argv[i]) == "-h")
			{
				printHelp (argc, argv);
				return (-1);
			}
		}
	}

	EventHelper event_helper;
	std::string device_id = "";
	pcl::console::parse_argument (argc, argv, "-dev", device_id);

	pcl::Grabber* interface = new pcl::OpenNIGrabber (device_id);

	cld.reset (new pcl::visualization::PCLVisualizer (argc, argv, "OpenNI Viewer"));

	std::string mouseMsg3D ("Mouse coordinates in PCL Visualizer");
	std::string keyMsg3D ("Key event for PCL Visualizer");
	cld->registerMouseCallback (&mouse_callback, (void*)(&mouseMsg3D));    
	cld->registerKeyboardCallback(&keyboard_callback, (void*)(&keyMsg3D));
	boost::function<void(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr&) > f = boost::bind (&EventHelper::cloud_cb, &event_helper, _1);
	boost::signals2::connection c1 = interface->registerCallback (f);

#if SHOW_IMAGE
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
#endif 

	interface->start ();
	bool cld_init = false;
	// Loop
	while (!cld->wasStopped ())
	{
		// Render and process events in the two interactors
		cld->spinOnce (); // ここで画面更新
#if SHOW_IMAGE
		img->spinOnce ();
#endif
		FPS_CALC ("drawing");

		// Add the cloud
		if (spcCloud && cld_mutex.try_lock ())
		{
			// 初期化
			if (!cld_init)
			{
				cld->getRenderWindow ()->SetSize (640, 480);
				cld->getRenderWindow ()->SetPosition (200,200);
				cld_init = !cld_init;
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

#if SHOW_IMAGE
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
					rgb_data = new unsigned char [rgb_data_size * 3];
				}
				spImage->fillRGB (spImage->getWidth (), spImage->getHeight (), rgb_data);
				img->showRGBImage (rgb_data, spImage->getWidth (), spImage->getHeight ());
			}
			img_mutex.unlock ();
		}
#endif
		boost::this_thread::sleep (boost::posix_time::microseconds (100));
	}

	interface->stop ();
}
/* ]--- */


#endif