#include "pcl_utils.h"
#include "FileIO.h"


pcl::PointCloud<pcl::PointXYZRGBA>::Ptr
	mario::filter( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr& cloud, pcl::PointIndices::Ptr inliers, bool isNegatibe )
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

mario::Coordinate<mario::typeM> mario::redExtraction( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst )
{
	long double x=0,y=0,z=0;
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
				x += cloud->points[count].x;
				y += cloud->points[count].y;
				z += cloud->points[count].z;
				dst->push_back( pt );
				rcount++;
		}
	}
	x/=rcount;
	y/=rcount;
	z/=rcount;
	cout<<x<<" "<<y<<" "<<z<<" :"<<rcount<<endl;
	return mario::Coordinate<mario::typeM>(x,y,z);
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

	//フィルタリング
	pcl::ExtractIndices<pcl::PointXYZRGBA> extract;
	extract.setInputCloud( cloud );
	extract.setIndices( inliers );

	// true にすると平面を除去、false にすると平面以外を除去
	extract.setNegative( true );
	extract.filter( *dst );
}

void mario::filterA( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst)
{
	static double OUT_FILTER_LOWER = FileIO::getConst("OUT_FILTER_LOWER");
	static double OUT_FILTER_UPPER = FileIO::getConst("OUT_FILTER_UPPER");
	static double DOWN_FILTER_LEAF = FileIO::getConst("DOWN_FILTER_LEAF"); // 大きいほど除去する
	static int const filterNum = 3;
	static pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloudPtrs[filterNum+1];
	// フィルタリング
	if( cloud ){
		static pcl::PassThrough<pcl::PointXYZRGBA> pass; // 外れ値除去フィルタ
		static pcl::VoxelGrid< pcl::PointXYZRGBA > sor; // ダウンサンプリングフィルタ
		static bool isInitDone = false;
		if( isInitDone == false ){
			// 外れ値除去フィルタの設定
			pass.setFilterFieldName ("z");
			pass.setFilterLimits (OUT_FILTER_LOWER, OUT_FILTER_UPPER);
			// ダウンサンプリングフィルタの設定
			if( DOWN_FILTER_LEAF > 0 ){
				sor.setLeafSize (DOWN_FILTER_LEAF,DOWN_FILTER_LEAF, DOWN_FILTER_LEAF);
			}
			for( int i=0; i<filterNum; i++ ){
				cloudPtrs[i] = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
			}
			isInitDone = true;
		}

		int filterCount = 0;
		// はずれ値除去フィルタ
		if( OUT_FILTER_LOWER > 0 && OUT_FILTER_UPPER > 0 ){
			pass.setInputCloud ( cloud );
			///pass.setFilterLimitsNegative (true);
			filterCount++;
			pass.filter ( *cloudPtrs[filterCount] );

		}
		// ダウンサンプリングフィルタ
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
		// 平面抽出
		//auto inliers = mario::segmentate( cloud_down_filtered, 0.001 ); //大きいほどアバウトに検出
		//auto cloud_plane_filtered = filter( cloud_down_filtered, inliers, true );
		//inliers = mario::segmentate( cloud_plane_filtered, 0.001 ); //大きいほどアバウトに検出
		// 格納されている順番に赤く着色
		//mario::redIteration( *cloud_down_filtered );
		// 赤色を検出して緑色に変換
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
	//mario::outlierFilter( downed, dst );
	static double SEGMENT_THRESHOULD = FileIO::getConst("SEGMENT_THRESHOULD"); // 大きいほど除去する
	mario::removePlane( downed, planed, SEGMENT_THRESHOULD );
	//mario::redExtraction( planed, reded );
	mario::clusterize( planed, clustered );
}

void mario::downSamplingFilter( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst)
{
	static double DOWN_FILTER_LEAF = FileIO::getConst("DOWN_FILTER_LEAF"); // 大きいほど除去する
	static pcl::VoxelGrid< pcl::PointXYZRGBA > sor; // ダウンサンプリングフィルタ
	static bool isInitDone = false;
	if( isInitDone == false ){
		// ダウンサンプリングフィルタの設定
		if( DOWN_FILTER_LEAF > 0 ){
			sor.setLeafSize (DOWN_FILTER_LEAF,DOWN_FILTER_LEAF, DOWN_FILTER_LEAF);
		}
		isInitDone = true;
	}

	// ダウンサンプリングフィルタ
	if( DOWN_FILTER_LEAF > 0 ){
		sor.setInputCloud ( cloud );
		sor.filter ( *dst );
	}
}

void mario::outlierFilter( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst)
{
	static double OUT_FILTER_LOWER = FileIO::getConst("OUT_FILTER_LOWER");
	static double OUT_FILTER_UPPER = FileIO::getConst("OUT_FILTER_UPPER");

	static pcl::PassThrough<pcl::PointXYZRGBA> pass; // 外れ値除去フィルタ
	static pcl::VoxelGrid< pcl::PointXYZRGBA > sor; // ダウンサンプリングフィルタ
	static bool isInitDone = false;
	if( isInitDone == false ){
		// 外れ値除去フィルタの設定
		pass.setFilterFieldName ("z");
		pass.setFilterLimits (OUT_FILTER_LOWER, OUT_FILTER_UPPER);
		isInitDone = true;
	}

	// はずれ値除去フィルタ
	if( OUT_FILTER_LOWER > 0 && OUT_FILTER_UPPER > 0 ){
		pass.setInputCloud ( cloud );
		///pass.setFilterLimitsNegative (true);
		pass.filter ( *dst );
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

void mario::clusterize( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst )
{
	// Read in the cloud data
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_f (new pcl::PointCloud<pcl::PointXYZRGBA>);

	// Create the filtering object: downsample the dataset using a leaf size of 1cm
	pcl::VoxelGrid<pcl::PointXYZRGBA> vg;
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_filtered (new pcl::PointCloud<pcl::PointXYZRGBA>);
	vg.setInputCloud (cloud);
	vg.setLeafSize (0.01f, 0.01f, 0.01f);
	vg.filter (*cloud_filtered);
	std::cout << "PointCloud after filtering has: " << cloud_filtered->points.size ()  << " data points." << std::endl; //*

	// Create the segmentation object for the planar model and set all the parameters
	pcl::SACSegmentation<pcl::PointXYZRGBA> seg;
	pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
	pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_plane (new pcl::PointCloud<pcl::PointXYZRGBA>());
	pcl::PCDWriter writer;
	seg.setOptimizeCoefficients (true);
	seg.setModelType (pcl::SACMODEL_PLANE);
	seg.setMethodType (pcl::SAC_RANSAC);
	seg.setMaxIterations (100);
	seg.setDistanceThreshold (0.02);

	int i=0, nr_points = (int) cloud_filtered->points.size ();
	while (cloud_filtered->points.size () > 0.3 * nr_points){
		// Segment the largest planar component from the remaining cloud
		seg.setInputCloud (cloud_filtered);
		seg.segment (*inliers, *coefficients);
		if (inliers->indices.size () == 0){
			std::cout << "Could not estimate a planar model for the given dataset." << std::endl;
			break;
		}

		// Extract the planar inliers from the input cloud
		pcl::ExtractIndices<pcl::PointXYZRGBA> extract;
		extract.setInputCloud (cloud_filtered);
		extract.setIndices (inliers);
		extract.setNegative (false);

		// Get the points associated with the planar surface
		extract.filter (*cloud_plane);
		std::cout << "PointCloud representing the planar component: " << cloud_plane->points.size () << " data points." << std::endl;

		// Remove the planar inliers, extract the rest
		extract.setNegative (true);
		extract.filter (*cloud_f);
		*cloud_filtered = *cloud_f;
	}

	// Creating the KdTree object for the search method of the extraction
	pcl::search::KdTree<pcl::PointXYZRGBA>::Ptr tree (new pcl::search::KdTree<pcl::PointXYZRGBA>);
	tree->setInputCloud (cloud_filtered);

	std::vector<pcl::PointIndices> cluster_indices;
	pcl::EuclideanClusterExtraction<pcl::PointXYZRGBA> ec;
	ec.setClusterTolerance (0.02); // 2cm
	ec.setMinClusterSize (100);
	ec.setMaxClusterSize (25000);
	ec.setSearchMethod (tree);
	ec.setInputCloud (cloud_filtered);
	ec.extract (cluster_indices);

	int j = 0;
	for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it){
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_cluster (new pcl::PointCloud<pcl::PointXYZRGBA>);
		for (std::vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); pit++){
			cloud_cluster->points.push_back (cloud_filtered->points[*pit]); //*
		}
		cloud_cluster->width = cloud_cluster->points.size ();
		cloud_cluster->height = 1;
		cloud_cluster->is_dense = true;

		std::cout << "PointCloud representing the Cluster: " << cloud_cluster->points.size () << " data points." << std::endl;
		std::stringstream ss;
		ss << "cloud_cluster_" << j << ".pcd";
		writer.write<pcl::PointXYZRGBA> (ss.str (), *cloud_cluster, false); //*
		j++;
	}
	dst = cloud_filtered->makeShared();
}