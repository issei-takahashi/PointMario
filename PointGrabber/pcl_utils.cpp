#include "pcl_utils.h"

void issei::segmentate(pcl::PointCloud<pcl::PointXYZRGBA>& cloud, double threshould)
{
	static pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
	static pcl::PointIndices::Ptr inliers (new pcl::PointIndices);

	// Create the segmentation object
	pcl::SACSegmentation<pcl::PointXYZRGBA> seg;
	// Optional
	seg.setOptimizeCoefficients (true);
	// Mandatory
	seg.setModelType (pcl::SACMODEL_PLANE);
	seg.setMethodType (pcl::SAC_RANSAC);
	seg.setDistanceThreshold (threshould);

	seg.setInputCloud (cloud.makeShared ());
	seg.segment (*inliers, *coefficients);

	// ax+by+cz+d=0
	if( coefficients->values.size() >= 4 ) {
		cout << "(" << coefficients->values[0] << ")x + (" << coefficients->values[1] << ")y + (" << coefficients->values[2] << ")z + (" << coefficients->values[3] << ") = 0" << endl;
	}
	float xmax=-FLT_MIN, ymax=-FLT_MIN;
	int iZero = -1;
	for (size_t i = 0; i < inliers->indices.size (); ++i) {
		cloud.points[inliers->indices[i]].r = 255;
		cloud.points[inliers->indices[i]].g = 0;
		cloud.points[inliers->indices[i]].b = 0;
		if( cloud.points[inliers->indices[i]].x > xmax && cloud.points[inliers->indices[i]].y > ymax ){
			xmax = cloud.points[inliers->indices[i]].x;
			ymax = cloud.points[inliers->indices[i]].y;
			iZero = i;
		}
	}
	if( iZero >= 0 ) {
		cloud.points[inliers->indices[iZero]].r = 0;
		cloud.points[inliers->indices[iZero]].g = 255;
		cloud.points[inliers->indices[iZero]].b = 0;
	}
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
			pass.setFilterLimits (0.0, 0.8);
			// ダウンサンプリングフィルタの設定
			static double leaf = 0.01;
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
		issei::segmentate( *cloud_down_filtered, 0.005 );
		dst = cloud_down_filtered->makeShared();
	}
}