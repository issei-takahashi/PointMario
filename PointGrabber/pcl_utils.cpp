#include "pcl_utils.h"
#include "FileIO.h"

using namespace mario;

Coordinate<typeM> mario::redDetection( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst )
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
	cout<<"RedDetection:"<<x<<" "<<y<<" "<<z<<" :"<<rcount<<endl;
	return Coordinate<typeM>(x,y,z);
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
			}else{
				sor.setInputCloud ( cloud );
			}
			filterCount++;
			sor.filter ( *cloudPtrs[filterCount] );
		}
		// 平面抽出
		//auto inliers = segmentate( cloud_down_filtered, 0.001 ); //大きいほどアバウトに検出
		//auto cloud_plane_filtered = filter( cloud_down_filtered, inliers, true );
		//inliers = segmentate( cloud_plane_filtered, 0.001 ); //大きいほどアバウトに検出
		// 格納されている順番に赤く着色
		//redIteration( *cloud_down_filtered );
		// 赤色を検出して緑色に変換
		//redDetection( *cloudPtrs[filterCount] );
		dst = cloudPtrs[filterCount]->makeShared();
	}
}

bool mario::filterB( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst, list< pcl::PointCloud<pcl::PointXYZRGBA>::Ptr >& l_dst, Coordinate<typeM> & redCenter )
{
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr downed, filtered, planed, reded, clustered;

	dst = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	downed = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	filtered = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	planed = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	reded = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);
	clustered = ( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr )(new pcl::PointCloud<pcl::PointXYZRGBA>);

	downSamplingFilter( cloud, downed );
	outlierFilter( downed, filtered );
	static double SEGMENT_THRESHOULD = FileIO::getConst("SEGMENT_THRESHOULD"); // 大きいほど除去する
	//removePlane( filtered, planed, SEGMENT_THRESHOULD );
	redExtraction( filtered, reded );

	clusterize( reded, dst, l_dst, 4 );
	//cout << "cluster size == " << l_dst.size() << endl;
	if( l_dst.size() == 4 ){
		Coordinate<typeM> ave;
		foreach(it,l_dst){
			ave += getAverage(*it);
			//cout << "\t" << ave.x << "," << ave.y << "," << ave.z << endl;
		}
		ave /= l_dst.size();
		redCenter = ave;
		return true;
	}else{
		return false;
	}
}

void mario::downSamplingFilter( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst)
{
	if( cloud->empty() ){
		dst = pcl::PointCloud<pcl::PointXYZRGBA>::Ptr( new pcl::PointCloud<pcl::PointXYZRGBA>() );
		return;
	}
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
	if( cloud->empty() ){
		dst = pcl::PointCloud<pcl::PointXYZRGBA>::Ptr( new pcl::PointCloud<pcl::PointXYZRGBA>() );
		return;
	}
	static double OUT_FILTER_LOWER = FileIO::getConst("OUT_FILTER_LOWER");
	static double OUT_FILTER_UPPER = FileIO::getConst("OUT_FILTER_UPPER");

	static pcl::PassThrough<pcl::PointXYZRGBA> pass; // 外れ値除去フィルタ
	static bool isInitDone = false;
	if( isInitDone == false ){
		// 外れ値除去フィルタの設定
		pass.setFilterFieldName ("z");
		pass.setFilterLimits (OUT_FILTER_LOWER, OUT_FILTER_UPPER);
		isInitDone = true;
	}

	// はずれ値除去フィルタ
	if( 0 <= OUT_FILTER_LOWER  && 0 <= OUT_FILTER_UPPER ){
		pass.setInputCloud ( cloud );
		///pass.setFilterLimitsNegative (true);
		pass.filter ( *dst );
	}else{
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

void mario::clusterize( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst, list< pcl::PointCloud<pcl::PointXYZRGBA>::Ptr >& l_dst, int maxNum )
{
	if( cloud->empty() ){
		dst = pcl::PointCloud<pcl::PointXYZRGBA>::Ptr( new pcl::PointCloud<pcl::PointXYZRGBA>() );
		return;
	}

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
	ec.extract(cluster_indices);

	// クラスタをサイズ順にソート
	{
		// ソート用ファンクタ
		struct cluster_compare_by_size
		{
			typedef pcl::PointIndices PI;
			bool operator()(const PI& lhs, const PI& rhs) const {
				return lhs.indices.size() < rhs.indices.size();
			}
		};
		// サイズ昇順ソート
		std::sort( cluster_indices.begin(), cluster_indices.end(), cluster_compare_by_size() ); // 1 2 3...
		int const clusterNum = cluster_indices.size();
		int count = 0;
		// 大きい方からmaxNum個だけ残す
		if( clusterNum > maxNum ){
			for( auto it = cluster_indices.begin(); it != cluster_indices.end(); ){
				it = cluster_indices.erase(it);
				count++;
				if( count == clusterNum - maxNum ){
					break;
				}
			}
		}
	}

	//// クラスタを重心座標順にソート
	//{
	//	// ソート用ファンクタ
	//	struct cluster_compare_by_xyz
	//	{
	//		typedef std::vector<pcl::PointIndices> PI;
	//		bool operator()(const PI& lhs, const PI& rhs) const {
	//			Coordinate<typeM> avel = getAverage(lhs);
	//			Coordinate<typeM> aver = getAverage(rhs);
	//			static Coordinate<typeM> zero;
	//			return avel.distance(zero) < aver.distance(zero);
	//		}
	//	};
	//	// 重心座標昇順ソート
	//	std::sort( cluster_indices.begin(), cluster_indices.end(), cluster_compare_by_xyz() ); // 1 2 3...
	//}

	dst = pcl::PointCloud<pcl::PointXYZRGBA>::Ptr( new pcl::PointCloud<pcl::PointXYZRGBA>() );
	int j = 0;
	static float const colors[6][3] ={{255, 0, 0}, {0,255,0}, {0,0,255}, {255,255,0}, {0,255,255}, {255,0,255}};  
	for (std::vector<pcl::PointIndices>::const_iterator it = cluster_indices.begin (); it != cluster_indices.end (); ++it){
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_cluster (new pcl::PointCloud<pcl::PointXYZRGBA>);
		// dstの着色
		// 各点をクラスタにプッシュ
		for (std::vector<int>::const_iterator pit = it->indices.begin (); pit != it->indices.end (); pit++){
			cloud_cluster->points.push_back (cloud->points[*pit]); //*
			dst->push_back(cloud->points[*pit]);
			dst->back().r = colors[j%6][0];
			dst->back().g = colors[j%6][1];
			dst->back().b = colors[j%6][2];
		}
		// クラスタをリストにプッシュ
		cloud_cluster->width = cloud_cluster->points.size ();
		cloud_cluster->height = 1;
		cloud_cluster->is_dense = true;
		l_dst.push_back( cloud_cluster );
		j++;
	}
}

void mario::searchNeighbors_voxel( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _inputCloud,
	pcl::PointXYZRGBA const & _searchPoint, float _resolution,
	indices_t & _ind )
{
	if( _inputCloud ){
		pcl::octree::OctreePointCloudSearch<pcl::PointXYZRGBA> octree (_resolution);
		octree.setInputCloud (_inputCloud);
		octree.addPointsFromInputCloud ();

		// Neighbors within voxel search

		indices_t pointIdxVec = (indices_t)(new vector<int>());

		if (octree.voxelSearch (_searchPoint, *pointIdxVec)){
			std::cout << "Neighbors within voxel search at (" << _searchPoint.x 
				<< " " << _searchPoint.y 
				<< " " << _searchPoint.z << ")" 
				<< std::endl;
			/*
			for (size_t i = 0; i < pointIdxVec->size (); ++i){
				std::cout << "    " << _inputCloud->points[(*pointIdxVec)[i]].x 
					<< " " << _inputCloud->points[(*pointIdxVec)[i]].y 
					<< " " << _inputCloud->points[(*pointIdxVec)[i]].z << std::endl;
			}
			*/
		}
		_ind = pointIdxVec;
	}
}


void mario::searchNeighbors_Knearest( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _inputCloud,
	pcl::PointXYZRGBA const & _searchPoint, float _resolution, int _K,
	indices_t & _ind, distances_t & _dist )
{
	if( _inputCloud ){
		pcl::octree::OctreePointCloudSearch<pcl::PointXYZRGBA> octree (_resolution);
		octree.setInputCloud (_inputCloud);
		octree.addPointsFromInputCloud ();

		// K nearest neighbor search

		indices_t pointIdxNKNSearch = (indices_t)(new vector<int>());
		distances_t pointNKNSquaredDistance = (distances_t)(new vector<float>());

		std::cout << "K nearest neighbor search at (" << _searchPoint.x 
			<< " " << _searchPoint.y 
			<< " " << _searchPoint.z
			<< ") with K=" << _K << std::endl;

		if (octree.nearestKSearch (_searchPoint, _K, *pointIdxNKNSearch, *pointNKNSquaredDistance) > 0){
			/*
			for (size_t i = 0; i < pointIdxNKNSearch->size (); ++i){
				std::cout << "    "  <<   _inputCloud->points[ (*pointIdxNKNSearch)[i] ].x 
					<< " " << _inputCloud->points[ (*pointIdxNKNSearch)[i] ].y 
					<< " " << _inputCloud->points[ (*pointIdxNKNSearch)[i] ].z 
					<< " (squared distance: " << (*pointNKNSquaredDistance)[i] << ")" << std::endl;
			}
			*/
		}
		_ind = pointIdxNKNSearch;
		_dist = pointNKNSquaredDistance;
	}
}

void mario::searchNeighbors_radius( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _inputCloud,
	pcl::PointXYZRGBA const & _searchPoint, float _resolution, float _radius,
	indices_t & _ind, distances_t & _dist )
{
	if( _inputCloud ){
		pcl::octree::OctreePointCloudSearch<pcl::PointXYZRGBA> octree (_resolution);
		octree.setInputCloud (_inputCloud);
		octree.addPointsFromInputCloud ();

		// Neighbors within radius search

		indices_t pointIdxRadiusSearch = (indices_t)(new vector<int>());
		distances_t pointRadiusSquaredDistance = (distances_t)(new vector<float>());

		std::cout << "Neighbors within radius search at (" << _searchPoint.x 
			<< " " << _searchPoint.y 
			<< " " << _searchPoint.z
			<< ") with radius=" << _radius << std::endl;


		if (octree.radiusSearch (_searchPoint, _radius, *pointIdxRadiusSearch, *pointRadiusSquaredDistance) > 0){
			/*
			for (size_t i = 0; i < pointIdxRadiusSearch->size (); ++i){
				std::cout << "    "  <<   _inputCloud->points[ (*pointIdxRadiusSearch)[i] ].x 
					<< " " << _inputCloud->points[ (*pointIdxRadiusSearch)[i] ].y 
					<< " " << _inputCloud->points[ (*pointIdxRadiusSearch)[i] ].z 
					<< " (squared distance: " << (*pointRadiusSquaredDistance)[i] << ")" << std::endl;
			}
			*/
		}
		_ind = pointIdxRadiusSearch;
		_dist = pointRadiusSquaredDistance;
	}
}


void mario::newPointSearch( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _cloudA,
	const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & _cloudB, float _resolution,
	indices_t & _ind )
{
	if( _cloudA ){
		// Octree resolution - side length of octree voxels

		// Instantiate octree-based point cloud change detection class
		pcl::octree::OctreePointCloudChangeDetector<pcl::PointXYZRGBA> octree (_resolution);

		// Add points from cloudA to octree
		octree.setInputCloud (_cloudA);
		octree.addPointsFromInputCloud ();

		// Switch octree buffers: This resets octree but keeps previous tree structure in memory.
		octree.switchBuffers ();

		// Add points from cloudB to octree
		octree.setInputCloud (_cloudB);
		octree.addPointsFromInputCloud ();

		indices_t newPointIdxVector = (indices_t)(new vector<int>());

		// Get vector of point indices from octree voxels which did not exist in previous buffer
		octree.getPointIndicesFromNewVoxels (*newPointIdxVector);

		// Output points
		std::cout << "Output from getPointIndicesFromNewVoxels:" << std::endl;
		for (size_t i = 0; i < newPointIdxVector->size (); ++i){
			/*
			std::cout << i << "# Index:" << (*newPointIdxVector)[i]
			<< "  Point:" << _cloudB->points[(*newPointIdxVector)[i]].x << " "
				<< _cloudB->points[(*newPointIdxVector)[i]].y << " "
				<< _cloudB->points[(*newPointIdxVector)[i]].z << std::endl;
				*/
		}
		_ind = newPointIdxVector;
	}
}

Coordinate<typeM> mario::getAverage( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud )
{
	Coordinate<typeM> ret(0,0,0);
	int count = 0;
	for( count=0; count<cloud->points.size(); count++ ){
		ret.x += cloud->points[count].x;
		ret.y += cloud->points[count].y;
		ret.z += cloud->points[count].z;
	}
	ret /= count;
	return ret;
}