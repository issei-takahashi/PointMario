#pragma once

#include "Coordinate.h"

namespace mario{

	pcl::PointIndices::Ptr 
		segmentate(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr& cloud, double threshould);
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr
		filter( pcl::PointCloud<pcl::PointXYZRGBA>::Ptr& cloud, pcl::PointIndices::Ptr inliers, bool isNegatibe );
	void filterA( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	void filterB( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	void downSamplingFilter( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	void outlierFilter( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	void redIteration( pcl::PointCloud<pcl::PointXYZRGBA>& cloud );
	Coordinate<typeM> redDetection(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	Coordinate<typeM> redExtraction(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
	void removePlane( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst, double threshould );
	void cvt2Mat( const boost::shared_ptr<openni_wrapper::Image>& input, boost::shared_ptr<openni_wrapper::Image>& output );
	void clusterize( const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud,  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst );
}
