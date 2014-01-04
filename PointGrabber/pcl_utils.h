#pragma once

namespace issei{

	pcl::PointIndices::Ptr 
		segmentate(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr& cloud, double threshould);
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr
		filter( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr& cloud, pcl::PointIndices::Ptr inliers, bool isNegatibe );
	void filterA( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	void redIteration( pcl::PointCloud<pcl::PointXYZRGBA>& cloud );
	void redDetection( pcl::PointCloud<pcl::PointXYZRGBA>& cloud );
	void cvt2Mat( const boost::shared_ptr<openni_wrapper::Image>& input, boost::shared_ptr<openni_wrapper::Image>& output );
}
