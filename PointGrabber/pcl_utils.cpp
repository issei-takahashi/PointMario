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

    //�t�B���^�����O
    pcl::ExtractIndices<pcl::PointXYZRGBA> extract;
    extract.setInputCloud( cloud );
    extract.setIndices( inliers );

    // true �ɂ���ƕ��ʂ������Afalse �ɂ���ƕ��ʈȊO������
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
	// �t�B���^�����O
	if( cloud ){
		static pcl::PassThrough<pcl::PointXYZRGBA> pass; // �O��l�����t�B���^
		static pcl::VoxelGrid< pcl::PointXYZRGBA > sor; // �_�E���T���v�����O�t�B���^
		static bool isInitDone = false;
		if( isInitDone == false ){
			// �O��l�����t�B���^�̐ݒ�
			pass.setFilterFieldName ("z");
			pass.setFilterLimits (0.0, 3.0);
			// �_�E���T���v�����O�t�B���^�̐ݒ�
			static double leaf = 0.01; // �傫���قǏ�������
			sor.setLeafSize (leaf,leaf, leaf);
			isInitDone = true;
		}

		static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_pass_filtered (new pcl::PointCloud<pcl::PointXYZRGBA>);
		static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_down_filtered (new pcl::PointCloud<pcl::PointXYZRGBA>);
		// �͂���l�����t�B���^
		pass.setInputCloud (cloud);
		///pass.setFilterLimitsNegative (true);
		pass.filter (*cloud_pass_filtered);
		// �_�E���T���v�����O�t�B���^
		sor.setInputCloud (cloud_pass_filtered);
		sor.filter (*cloud_down_filtered);
		// ���ʒ��o
		//auto inliers = issei::segmentate( cloud_down_filtered, 0.001 ); //�傫���قǃA�o�E�g�Ɍ��o
		//auto cloud_plane_filtered = filter( cloud_down_filtered, inliers, true );
		//inliers = issei::segmentate( cloud_plane_filtered, 0.001 ); //�傫���قǃA�o�E�g�Ɍ��o
		// �i�[����Ă��鏇�ԂɐԂ����F
		//issei::redIteration( *cloud_down_filtered );
		// �ԐF�����o���ėΐF�ɕϊ�
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

				IplImage* imgColor = cvCreateImage(cvSize(input->getWidth(), input->getHeight()),IPL_DEPTH_8U,3); //16U�ł��܂�����
				imgColor->imageData = (char*) rgb_data;
				// iplImage��cvShowImage�ŕ\��  
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