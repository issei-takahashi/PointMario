#include "PointCapture.h"
#include "pcl_utils.h"
#include "FileIO.h"

static mario::PointCapture * g_latestPointCapture = NULL;

mario::PointCapture::PointCapture( string const & _fileName )
 : MeasureBasement(), isCaptureDone(false), showMode(0), fileName(_fileName)
{
	g_latestPointCapture = this;
}

// Simple callbacks.
void mario::PointCapture::keyboard_callback( const pcl::visualization::KeyboardEvent& _evt, void* cookie )
{
	std::string* message = (std::string*)cookie;
	cout << (*message) << " :: ";
	if (_evt.getKeyCode()){
		switch(_evt.getKeyCode()){
		case 'p': // 点群表示モード変更
			{
				if (_evt.keyDown()){
					g_latestPointCapture->showMode = (g_latestPointCapture->showMode+1) % CLOUD_TYPE_NUM;
					cout << "p is pressed" << endl;
				}
				else{
					//cout << "p is released" << endl;
				}
				break;
			}
		}
	}
	else{
		cout << "the special key \'" << _evt.getKeySym() << "\' was";
	}
}

void mario::PointCapture::mouse_callback( const pcl::visualization::MouseEvent& mouse_event, void* cookie )
{
	std::string* message = (std::string*) cookie;
	// 左クリック
	if (mouse_event.getType() == pcl::visualization::MouseEvent::MouseButtonPress &&
		mouse_event.getButton() == pcl::visualization::MouseEvent::LeftButton)
	{
		///cout << (*message) << " :: " << mouse_event.getX () << " , " << mouse_event.getY () << endl;
		
	}
}


void mario::PointCapture::cloud_cb (const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud)
{
	FPS_CALC ("callback");

	this->cld_mutex.lock ();

	if( this->isCaptureDone == false ){
		/* 初期化 */
		this->raw = cloud->makeShared();
		this->downed = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
		this->filtered = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
		this->reded = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
		this->clustered = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);

		/* 各種フィルタ */
		mario::downSamplingFilter( this->raw, this->downed );
		mario::outlierFilter( this->downed, this->filtered );
		mario::redExtraction( this->filtered, this->reded );
		mario::clusterize( this->reded, this->clustered, this->l_cluster, 4 );

		/* ファイルへ書き込み */
		pcl::PCDWriter writer;
		writer.write<pcl::PointXYZRGBA>( "pcd_files/"+this->fileName+"_raw"+".pcd", *this->raw, false );
		writer.write<pcl::PointXYZRGBA>( "pcd_files/"+this->fileName+"_downed"+".pcd", *this->downed, false );
		writer.write<pcl::PointXYZRGBA>( "pcd_files/"+this->fileName+"_filtered"+".pcd", *this->filtered, false );
		writer.write<pcl::PointXYZRGBA>( "pcd_files/"+this->fileName+"_reded"+".pcd", *this->reded, false );
		writer.write<pcl::PointXYZRGBA>( "pcd_files/"+this->fileName+"_clustered"+".pcd", *this->clustered, false );

		/* キャプチャ終了フラグを立てる */
		this->isCaptureDone = true;
	}

	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr arr[CLOUD_TYPE_NUM] = { this->raw, this->downed, this->filtered, this->reded, this->clustered };
	this->spcCloud = arr[this->showMode]->makeShared();

	this->cld_mutex.unlock ();

}

void mario::PointCapture::image_callback ( const boost::shared_ptr<openni_wrapper::Image>& image )
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

static string const mouseMsg3D ("Mouse coordinates in PCL Visualizer");
static string const keyMsg3D ("Key event for PCL Visualizer");
void mario::PointCapture::setCallBackFunctions()
{
	this->spVisualizer->registerMouseCallback   ( &mario::PointCapture::mouse_callback   , (void*)(&mouseMsg3D) );    
	this->spVisualizer->registerKeyboardCallback( &mario::PointCapture::keyboard_callback, (void*)(&keyMsg3D) );
}
