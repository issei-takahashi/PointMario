#include "pcl_utils.h"
#include "FileIO.h"

pcl::PointIndices::Ptr 
mario::segmentate(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr& cloud, double threshould)
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
mario::filter( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr& cloud, pcl::PointIndices::Ptr inliers, bool isNegatibe )
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


void mario::redIteration(pcl::PointCloud<pcl::PointXYZRGBA>& cloud )
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

void mario::redDetection( pcl::PointCloud<pcl::PointXYZRGBA>& cloud )
{
	double x=0,y=0,z=0;
	int rcount=0;
	static double RED_VAL = FileIO::getConst("RED_VAL");
	static double RED_RATE = FileIO::getConst("RED_RATE");
	for(int count=0;count<cloud.points.size();count++){
		if( cloud.points[count].r > RED_VAL && 
			cloud.points[count].r > cloud.points[count].g*RED_RATE &&
			cloud.points[count].r > cloud.points[count].b*RED_RATE ){
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
	//cout<<x<<" "<<y<<" "<<z<<endl;
}

void mario::filterA( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst)
{
	static double OUT_FILTER_LOWER = FileIO::getConst("OUT_FILTER_LOWER");
	static double OUT_FILTER_UPPER = FileIO::getConst("OUT_FILTER_UPPER");
	static double DOWN_FILTER_LEAF = FileIO::getConst("DOWN_FILTER_LEAF"); // �傫���قǏ�������
	static int const filterNum = 3;
	static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloudPtrs[filterNum+1];
	// �t�B���^�����O
	if( cloud ){
		static pcl::PassThrough<pcl::PointXYZRGBA> pass; // �O��l�����t�B���^
		static pcl::VoxelGrid< pcl::PointXYZRGBA > sor; // �_�E���T���v�����O�t�B���^
		static bool isInitDone = false;
		if( isInitDone == false ){
			// �O��l�����t�B���^�̐ݒ�
			pass.setFilterFieldName ("z");
			pass.setFilterLimits (OUT_FILTER_LOWER, OUT_FILTER_UPPER);
			// �_�E���T���v�����O�t�B���^�̐ݒ�
			if( DOWN_FILTER_LEAF > 0 ){
				sor.setLeafSize (DOWN_FILTER_LEAF,DOWN_FILTER_LEAF, DOWN_FILTER_LEAF);
			}
			for( int i=0; i<filterNum; i++ ){
				cloudPtrs[i] = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
			}
			isInitDone = true;
		}

		int filterCount = 0;
		// �͂���l�����t�B���^
		if( OUT_FILTER_LOWER > 0 && OUT_FILTER_UPPER > 0 ){
			pass.setInputCloud ( cloud );
			///pass.setFilterLimitsNegative (true);
			filterCount++;
			pass.filter ( *cloudPtrs[filterCount] );
			
		}
		// �_�E���T���v�����O�t�B���^
		if( DOWN_FILTER_LEAF > 0 ){
			if( filterCount > 0 ){
				sor.setInputCloud ( cloudPtrs[filterCount] );
			}
			else{
				sor.setInputCloud ( cloud );
			}
			filterCount++;
			sor.filter ( *cloudPtrs[filterCount] );
		}
		// ���ʒ��o
		//auto inliers = mario::segmentate( cloud_down_filtered, 0.001 ); //�傫���قǃA�o�E�g�Ɍ��o
		//auto cloud_plane_filtered = filter( cloud_down_filtered, inliers, true );
		//inliers = mario::segmentate( cloud_plane_filtered, 0.001 ); //�傫���قǃA�o�E�g�Ɍ��o
		// �i�[����Ă��鏇�ԂɐԂ����F
		//mario::redIteration( *cloud_down_filtered );
		// �ԐF�����o���ėΐF�ɕϊ�
		mario:redDetection( *cloudPtrs[filterCount] );
		dst = cloudPtrs[filterCount]->makeShared();
	}
}


void mario::cvt2Mat( const boost::shared_ptr<openni_wrapper::Image>& input, boost::shared_ptr<openni_wrapper::Image>& output )
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