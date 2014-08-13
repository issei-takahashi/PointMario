#include "DownOutMeasure.h"
#include "pcl_utils.h"
#include "FileIO.h"
#include "SolidBody.h"

#define MAKE_CLOUD(dst) dst=(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)(new pcl::PointCloud<pcl::PointXYZRGBA>)

mario::DownOutMeasure::DownOutMeasure( Eigen::Matrix4d const & _mat )
 :MeasureBasement(), MtoDMat(_mat)
{

}

bool mario::DownOutMeasure::collisionDetectionWithCloud( SolidBody & _obj, double _resolution )
{
	this->cld_mutex.lock();
	indices_t indices;
	auto search = _obj.getSearchPoint();
	mario::searchNeighbors_voxel( this->spcCloud, search, _resolution, indices );
	this->cld_mutex.unlock();
	if( indices ){
		if( indices->size() > 10 ){
			return true;
		}
	}
	return false;
}

// Simple callbacks.
void mario::DownOutMeasure::keyboard_callback( const pcl::visualization::KeyboardEvent& _evt, void* cookie )
{
	//std::string* message = (std::string*)cookie;
	//cout << (*message) << " :: ";
	//if (_evt.getKeyCode()){
	//	switch(_evt.getKeyCode()){
	//	case 'r': // �^��J�n
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

void mario::DownOutMeasure::mouse_callback( const pcl::visualization::MouseEvent& mouse_event, void* cookie )
{
	std::string* message = (std::string*) cookie;
	// ���N���b�N
	if (mouse_event.getType() == pcl::visualization::MouseEvent::MouseButtonPress &&
		mouse_event.getButton() == pcl::visualization::MouseEvent::LeftButton)
	{
		///cout << (*message) << " :: " << mouse_event.getX () << " , " << mouse_event.getY () << endl;
		
	}
}


void mario::DownOutMeasure::cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & mcloud)
{
	FPS_CALC ("callback");

	this->cld_mutex.lock ();

	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr dcloud ;

	if( this->convertMtoD_withDownAndOut( mcloud, dcloud ) ){
		this->spcCloud = dcloud->makeShared();
	}

	this->cld_mutex.unlock ();

}

void mario::DownOutMeasure::image_callback ( const boost::shared_ptr<openni_wrapper::Image>& image )
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

bool mario::DownOutMeasure::convertMtoD_withDownAndOut( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _mcloud,
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & _dcloud )
{
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr downed, filtered;
	MAKE_CLOUD(downed);
	MAKE_CLOUD(filtered);

	//downed    = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	if( _dcloud.get() == NULL ){
		MAKE_CLOUD(_dcloud);
	}

	mario::downSamplingFilter( _mcloud, downed );
	mario::outlierFilter( downed, filtered );
	cout << "DownOut:" << filtered->points.size() << endl;
	times(count,0,filtered->points.size()){
		Eigen::Vector4d tmpv;
		tmpv(0) = filtered->points[count].x;
		tmpv(1) = filtered->points[count].y;
		tmpv(2) = filtered->points[count].z;
		tmpv(3) = 1.0;
		tmpv = this->MtoDMat * tmpv;
		pcl::PointXYZRGBA pt;
		pt.x = tmpv(0);
		pt.y = tmpv(1);
		pt.z = tmpv(2);
		_dcloud->push_back( pt );
	}
	return true;
}

static string const mouseMsg3D ("Mouse coordinates in PCL Visualizer");
static string const keyMsg3D ("Key event for PCL Visualizer");
void mario::DownOutMeasure::setCallBackFunctions()
{
	this->spVisualizer->registerMouseCallback   ( &mario::DownOutMeasure::mouse_callback   , (void*)(&mouseMsg3D) );    
	this->spVisualizer->registerKeyboardCallback( &mario::DownOutMeasure::keyboard_callback, (void*)(&keyMsg3D) );
}
