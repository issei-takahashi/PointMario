#pragma once

namespace issei{

	void segmentate(pcl::PointCloud<pcl::PointXYZRGBA>& cloud, double threshould);
	void filterA(const pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr & cloud, pcl::PointCloud<pcl::PointXYZRGBA>::Ptr & dst);
}
