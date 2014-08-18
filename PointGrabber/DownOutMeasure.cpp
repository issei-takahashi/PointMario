#include "DownOutMeasure.h"
#include "pcl_utils.h"
#include "FileIO.h"
#include "SolidBody.h"

#define MAKE_CLOUD(dst) dst=(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr)(new pcl::PointCloud<pcl::PointXYZRGBA>)

mario::DownOutMeasure::DownOutMeasure( Eigen::Matrix4d const & _mat )
 :MeasureBasement(), MtoDMat(_mat)
{

}

bool mario::DownOutMeasure::collisionDetectionWithCloud_simple( SolidBody & _obj, double _distance )
{
	this->cld_mutex.lock();
	indices_t indices;
	auto search = _obj.getSearchPoint();
	bool ret = mario::searchNeighbors_simple( this->spcCloud, search, _distance, indices );
	times(i,0,indices->size()){
		this->spcCloud->points[(*indices)[i]].r = 255 ;
		this->spcCloud->points[(*indices)[i]].g = 0 ;
		this->spcCloud->points[(*indices)[i]].b  = 0 ;
	}
	this->cld_mutex.unlock();
	return ret;
}

bool mario::DownOutMeasure::collisionDetectionWithCloud_voxel( SolidBody & _obj, double _resolution )
{
	this->cld_mutex.lock();
	indices_t indices;
	auto search = _obj.getSearchPoint();
	bool ret = mario::searchNeighbors_voxel( this->spcCloud, search, _resolution, indices );
	times(i,0,indices->size()){
		this->spcCloud->points[(*indices)[i]].r = 255 ;
		this->spcCloud->points[(*indices)[i]].g = 0 ;
		this->spcCloud->points[(*indices)[i]].b  = 0 ;
	}
	this->cld_mutex.unlock();
	return ret;
}

bool mario::DownOutMeasure::collisionDetectionWithCloud_Knearest( class SolidBody & _obj, float _resolution, int _K )
{
	this->cld_mutex.lock();
	indices_t indices;
	distances_t distances;
	auto search = _obj.getSearchPoint();
	bool ret = mario::searchNeighbors_Knearest( this->spcCloud, search, _resolution,_K, indices, distances );
	times(i,0,indices->size()){
		this->spcCloud->points[(*indices)[i]].r = 255 ;
		this->spcCloud->points[(*indices)[i]].g = 0 ;
		this->spcCloud->points[(*indices)[i]].b  = 0 ;
	}
	this->cld_mutex.unlock();
	return ret;
}

bool mario::DownOutMeasure::collisionDetectionWithCloud_radius( class SolidBody & _obj, float _resolution, float _radius )
{
	this->cld_mutex.lock();
	indices_t indices;
	distances_t distances;
	auto search = _obj.getSearchPoint();
	bool ret = mario::searchNeighbors_radius( this->spcCloud, search, _resolution, _radius, indices, distances );
	times(i,0,indices->size()){
		this->spcCloud->points[(*indices)[i]].r = 255 ;
		this->spcCloud->points[(*indices)[i]].g = 0 ;
		this->spcCloud->points[(*indices)[i]].b  = 0 ;
	}
	this->cld_mutex.unlock();
	return ret;
}

// Simple callbacks.
void mario::DownOutMeasure::keyboard_callback( const pcl::visualization::KeyboardEvent& _evt, void* cookie )
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

void mario::DownOutMeasure::mouse_callback( const pcl::visualization::MouseEvent& mouse_event, void* cookie )
{
	std::string* message = (std::string*) cookie;
	// 左クリック
	if (mouse_event.getType() == pcl::visualization::MouseEvent::MouseButtonPress &&
		mouse_event.getButton() == pcl::visualization::MouseEvent::LeftButton)
	{
		///cout << (*message) << " :: " << mouse_event.getX () << " , " << mouse_event.getY () << endl;
		
	}
}


void mario::DownOutMeasure::cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _cloud)
{
	FPS_CALC ("callback");

	this->cld_mutex.lock ();

	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr outcloud ;

	this->downAndOut( _cloud, outcloud );
	this->spcCloud = outcloud->makeShared();

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

void mario::DownOutMeasure::downAndOut( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _src, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & _dst )
{
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr downed;
	MAKE_CLOUD(downed);
	if( _dst.get() == NULL ){
		MAKE_CLOUD(_dst);
	}

	mario::downSamplingFilter( _src, downed );
	mario::outlierFilter( downed, _dst );
	cout << "DownOut:" << _dst->points.size() << endl;
}

static string const mouseMsg3D ("Mouse coordinates in PCL Visualizer");
static string const keyMsg3D ("Key event for PCL Visualizer");
void mario::DownOutMeasure::setCallBackFunctions()
{
	this->spVisualizer->registerMouseCallback   ( &mario::DownOutMeasure::mouse_callback   , (void*)(&mouseMsg3D) );    
	this->spVisualizer->registerKeyboardCallback( &mario::DownOutMeasure::keyboard_callback, (void*)(&keyMsg3D) );
}
