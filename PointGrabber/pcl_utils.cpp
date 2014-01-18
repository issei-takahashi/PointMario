#include "pcl_utils.h"
#include "FileIO.h"



mario::Coordinate<mario::typeM> mario::redDetection( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst )
{
	dst = cloud->makeShared();
	long double x=0,y=0,z=0;
	int rcount=0;
	static double RED_VAL = FileIO::getConst("RED_VAL");
	static double RED_RATE = FileIO::getConst("RED_RATE");
	for(int count=0;count<dst->points.size();count++){
		if( dst->points[count].r > RED_VAL && 
			dst->points[count].r > dst->points[count].g*RED_RATE &&
			dst->points[count].r > dst->points[count].b*RED_RATE ){
				dst->points[count].r = 0;
				dst->points[count].g = 255;
				dst->points[count].b = 0;

				x += dst->points[count].x;
				y += dst->points[count].y;
				z += dst->points[count].z;
				rcount++;
		}
	}
	x/=rcount;
	y/=rcount;
	z/=rcount;
	cout<<x<<" "<<y<<" "<<z<<" :"<<rcount<<endl;
	return mario::Coordinate<mario::typeM>(x,y,z);
}

void mario::redExtraction( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst )
{
	int rcount=0;
	static double RED_VAL = FileIO::getConst("RED_VAL");
	static double RED_RATE = FileIO::getConst("RED_RATE");
	for(int count=0;count<cloud->points.size();count++){
		if( cloud->points[count].r > RED_VAL && 
			cloud->points[count].r > cloud->points[count].g*RED_RATE &&
			cloud->points[count].r > cloud->points[count].b*RED_RATE ){
				pcl::PointXYZRGBA pt;
				pt.r = 255;
				pt.g = 0;
				pt.b = 0;
				pt.a = 255;
				pt.x = cloud->points[count].x;
				pt.y = cloud->points[count].y;
				pt.z = cloud->points[count].z;
				dst->push_back( pt );
				rcount++;
		}
	}
}

void mario::removePlane( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst, double threshould )
{
	dst = cloud->makeShared();

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

	//for (size_t i = 0; i < inliers->indices.size (); ++i) {
	//	cloud->points[inliers->indices[i]].r = 255;
	//	cloud->points[inliers->indices[i]].g = 0;
	//	cloud->points[inliers->indices[i]].b = 0;
	//}

	//�t�B���^�����O
	pcl::ExtractIndices<pcl::PointXYZRGBA> extract;
	extract.setInputCloud( cloud );
	extract.setIndices( inliers );

	// true �ɂ���ƕ��ʂ������Afalse �ɂ���ƕ��ʈȊO������
	extract.setNegative( true );
	extract.filter( *dst );
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
		//mario::redDetection( *cloudPtrs[filterCount] );
		dst = cloudPtrs[filterCount]->makeShared();
	}
}

void mario::filterB( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst)
{
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr downed, filtered, planed, reded, clustered;

	dst = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	downed = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	filtered = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	planed = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	reded = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	clustered = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);

	mario::downSamplingFilter( cloud, downed );
	mario::outlierFilter( downed, filtered );
	static double SEGMENT_THRESHOULD = FileIO::getConst("SEGMENT_THRESHOULD"); // �傫���قǏ�������
	mario::removePlane( filtered, planed, SEGMENT_THRESHOULD );
	mario::redExtraction( planed, reded );
	vector< pcl::PointCloud<pcl::PointXYZRGBA>::Ptr > v_clusters;
	mario::clusterize( reded, dst, v_clusters );
	cout << "cluster size == " << v_clusters.size() << endl;
	foreach(it,v_clusters){
		mario::Coordinate<mario::typeM> ave = mario::getAverage(*it);
		cout << "\t" << ave.x << "," << ave.y << "," << ave.z << endl;
	}
}

void mario::downSamplingFilter( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst)
{
	static double DOWN_FILTER_LEAF = FileIO::getConst("DOWN_FILTER_LEAF"); // �傫���قǏ�������
	static pcl::VoxelGrid< pcl::PointXYZRGBA > sor; // �_�E���T���v�����O�t�B���^
	static bool isInitDone = false;
	if( isInitDone == false ){
		// �_�E���T���v�����O�t�B���^�̐ݒ�
		if( DOWN_FILTER_LEAF > 0 ){
			sor.setLeafSize (DOWN_FILTER_LEAF,DOWN_FILTER_LEAF, DOWN_FILTER_LEAF);
		}
		isInitDone = true;
	}

	// �_�E���T���v�����O�t�B���^
	if( DOWN_FILTER_LEAF > 0 ){
		sor.setInputCloud ( cloud );
		sor.filter ( *dst );
	}
}

void mario::outlierFilter( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst)
{
	static double OUT_FILTER_LOWER = FileIO::getConst("OUT_FILTER_LOWER");
	static double OUT_FILTER_UPPER = FileIO::getConst("OUT_FILTER_UPPER");

	static pcl::PassThrough<pcl::PointXYZRGBA> pass; // �O��l�����t�B���^
	static bool isInitDone = false;
	if( isInitDone == false ){
		// �O��l�����t�B���^�̐ݒ�
		pass.setFilterFieldName ("z");
		pass.setFilterLimits (OUT_FILTER_LOWER, OUT_FILTER_UPPER);
		isInitDone = true;
	}

	// �͂���l�����t�B���^
	if( 0 <= OUT_FILTER_LOWER  && 0 <= OUT_FILTER_UPPER ){
		pass.setInputCloud ( cloud );
		///pass.setFilterLimitsNegative (true);
		pass.filter ( *dst );
	}
	else{
		dst = cloud->makeShared();
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

void mario::clusterize( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst, vector<  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr >& v_dst )
{
	dst = cloud->makeShared();

	// Creating the KdTree object for the search method of the extraction
	pcl::search::KdTree<pcl::PointXYZRGBA>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZRGBA>);
	//tree->setInputCloud (cloud_filtered);
	tree->setInputCloud (cloud);
	std::vector<pcl::PointIndices> cluster_indices;
	pcl::EuclideanClusterExtraction<pcl::PointXYZRGBA> ec;
	static double CLUSTER_TOLERANCE = FileIO::getConst("CLUSTER_TOLERANCE");
	static double MIN_CLUSTER_SIZE = FileIO::getConst("MIN_CLUSTER_SIZE");
	static double MAX_CLUSTER_SIZE = FileIO::getConst("MAX_CLUSTER_SIZE");
	ec.setClusterTolerance (CLUSTER_TOLERANCE); 
	ec.setMinClusterSize (MIN_CLUSTER_SIZE);
	ec.setMaxClusterSize (MAX_CLUSTER_SIZE);
	ec.setSearchMethod (tree);
	//ec.setInputCloud (cloud_filtered);
	ec.setInputCloud (cloud);
	ec.extract (cluster_indices);

	int j = 0;
	float colors[6][3] ={{255, 0, 0}, {0,255,0}, {0,0,255}, {255,255,0}, {0,255,255}, {255,0,255}};  
	for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it){
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_cluster (new pcl::PointCloud<pcl::PointXYZRGBA>);
		for (std::vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); pit++){
			cloud_cluster->points.push_back (dst->points[*pit]); //*
			dst->points[*pit].r = colors[j%6][0];
			dst->points[*pit].g = colors[j%6][1];
			dst->points[*pit].b = colors[j%6][2];
		}
		cloud_cluster->width = cloud_cluster->points.size ();
		cloud_cluster->height = 1;
		cloud_cluster->is_dense = true;
		v_dst.push_back( cloud_cluster );
		///std::cout << "PointCloud representing the Cluster: " << cloud_cluster->points.size () << " data points." << std::endl;
		///std::stringstream ss;
		///ss << "cloud_cluster_" << j << ".pcd";
		///writer.write<pcl::PointXYZRGBA> (ss.str (), *cloud_cluster, false); //*
		j++;
	}
}

mario::Coordinate<mario::typeM> mario::getAverage( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud )
{
	mario::Coordinate<mario::typeM> ret(0,0,0);
	int count = 0;
	for( count=0; count<cloud->points.size(); count++ ){
		ret.x += cloud->points[count].x;
		ret.y += cloud->points[count].y;
		ret.z += cloud->points[count].z;
	}
	ret /= count;
	return ret;
}