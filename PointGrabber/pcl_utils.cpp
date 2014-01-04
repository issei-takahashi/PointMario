#include "pcl_utils.h"

pcl::PointIndices::Ptr 
issei::segmentate(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr& cloud, double threshould)
{
    pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers (new pcl::PointIndices);

    // Create the segmentation object
    pcl::SACSegmentation<pcl::PointXYZRGBA> seg;
    // Optional
    seg.setOptimizeCoefficients (true);
    // Mandatory
    seg.setModelType (pcl::SACMODEL_PLANE);
    seg.setMethodType (pcl::SAC_RANSAC);
    seg.setDistanceThreshold (threshould);

    seg.setInputCloud (cloud);
    seg.segment (*inliers, *coefficients);

    for (size_t i = 0; i < inliers->indices.size (); ++i) {
        cloud->points[inliers->indices[i]].r = 255;
        cloud->points[inliers->indices[i]].g = 0;
        cloud->points[inliers->indices[i]].b = 0;
    }

    return inliers;
}

pcl::PointCloud<pcl::PointXYZRGBA>::Ptr
issei::filter( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr& cloud, pcl::PointIndices::Ptr inliers, bool isNegatibe )
{
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr result =
        pcl::PointCloud<pcl::PointXYZRGBA>::Ptr( new pcl::PointCloud<pcl::PointXYZRGBA>() );

    //フィルタリング
    pcl::ExtractIndices<pcl::PointXYZRGBA> extract;
    extract.setInputCloud( cloud );
    extract.setIndices( inliers );

    // true にすると平面を除去、false にすると平面以外を除去
    extract.setNegative( isNegatibe );
    extract.filter( *result );

    return result;
}

bool compare( const pcl::PointXYZRGBA& left, const pcl::PointXYZRGBA& right )
{
	 float lx = left.x/left.z;
	 float ly = left.y/left.z;
	 float rx = right.x/right.z;
	 float ry = right.y/right.z;
	 if( ly == ry ){
		 return lx < rx;
	 }
	 else{
		 return ly < ry;
	 }
}


void issei::redIteration(pcl::PointCloud<pcl::PointXYZRGBA>& cloud )
{
	std::sort(cloud.points.begin(),cloud.points.end(),compare);
	static int count = 0;
	cloud.points[count].r = 255;
	cloud.points[count].g = 0;
	cloud.points[count].b = 0;
	count = count+1;
	static float zmax = -1;
	if( count > cloud.points.size() ){
		count = 0;
	};
}

void issei::redDetection( pcl::PointCloud<pcl::PointXYZRGBA>& cloud )
{
	float x=0,y=0,z=0;
	int rcount=0;
	for(int count=0;count<cloud.points.size();count++){
		if( cloud.points[count].r > 50 && 
			cloud.points[count].r > cloud.points[count].g*1.5 &&
			cloud.points[count].r > cloud.points[count].b*1.5 ){
			cloud.points[count].r = 0;
			cloud.points[count].g = 255;
			cloud.points[count].b = 0;

			x += cloud.points[count].x;
			y += cloud.points[count].y;
			z += cloud.points[count].z;
			rcount++;
		}
	}
	x/=rcount;
	y/=rcount;
	z/=rcount;
	cout<<x<<" "<<y<<" "<<z<<endl;
}

void issei::filterA( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst)
{
	// フィルタリング
	if( cloud ){
		static pcl::PassThrough<pcl::PointXYZRGBA> pass; // 外れ値除去フィルタ
		static pcl::VoxelGrid< pcl::PointXYZRGBA > sor; // ダウンサンプリングフィルタ
		static bool isInitDone = false;
		if( isInitDone == false ){
			// 外れ値除去フィルタの設定
			pass.setFilterFieldName ("z");
			pass.setFilterLimits (0.0, 3.0);
			// ダウンサンプリングフィルタの設定
			static double leaf = 0.01; // 大きいほど除去する
			sor.setLeafSize (leaf,leaf, leaf);
			isInitDone = true;
		}

		static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_pass_filtered (new pcl::PointCloud<pcl::PointXYZRGBA>);
		static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_down_filtered (new pcl::PointCloud<pcl::PointXYZRGBA>);
		// はずれ値除去フィルタ
		pass.setInputCloud (cloud);
		///pass.setFilterLimitsNegative (true);
		pass.filter (*cloud_pass_filtered);
		// ダウンサンプリングフィルタ
		sor.setInputCloud (cloud_pass_filtered);
		sor.filter (*cloud_down_filtered);
		// 平面抽出
		//auto inliers = issei::segmentate( cloud_down_filtered, 0.001 ); //大きいほどアバウトに検出
		//auto cloud_plane_filtered = filter( cloud_down_filtered, inliers, true );
		//inliers = issei::segmentate( cloud_plane_filtered, 0.001 ); //大きいほどアバウトに検出
		// 格納されている順番に赤く着色
		//issei::redIteration( *cloud_down_filtered );
		// 赤色を検出して緑色に変換
		issei:redDetection( *cloud_down_filtered );
		dst = cloud_down_filtered->makeShared();
	}
}


void issei::cvt2Mat( const boost::shared_ptr<openni_wrapper::Image>& input, boost::shared_ptr<openni_wrapper::Image>& output )
{
	if( input ){
		try{
			const xn::ImageMetaData & meta = input->getMetaData();
			const XnUInt8 * pImage = meta.Data();
			if( pImage ){
				//cvCvtColor( imgColor, dst, CV_RGB2BGR);
				static unsigned int rgb_data_size = 0;
				static unsigned char* rgb_data = 0;
				if (rgb_data_size < input->getWidth () * input->getHeight ())
				{
					rgb_data_size = input->getWidth () * input->getHeight ();
					if( rgb_data ){
						delete rgb_data;
					}
					rgb_data = new unsigned char [rgb_data_size * 3];
				}
				input->fillRGB (input->getWidth (), input->getHeight (), rgb_data);

				IplImage* imgColor = cvCreateImage(cvSize(input->getWidth(), input->getHeight()),IPL_DEPTH_8U,3); //16Uでうまくいく
				imgColor->imageData = (char*) rgb_data;
				// iplImageはcvShowImageで表示  
				cvCvtColor(imgColor,imgColor,CV_BGR2RGB);
				
				cv::Mat mat(imgColor);
				cv::namedWindow("cv::Mat");
				cv::imshow("cv::Mat",mat);
				cv::waitKey(1);
			}
		}
		catch (std::exception & exp){
			cout<<exp.what()<<endl;
		}
		catch(...){
			cout<<"unknown exception"<<endl;
		}
	}
}