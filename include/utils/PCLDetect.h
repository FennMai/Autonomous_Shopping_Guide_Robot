// PCLDetect.h
#ifndef PCLDETECT_H
#define PCLDETECT_H

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/segmentation/extract_clusters.h>
#include "common.h"

class PCLDetect {
public:
    PCLDetect();
    ~PCLDetect();
    void clusterLidarData(const std::vector<LidarPoint>& inputPoints, std::vector<std::vector<LidarPoint>>& clusteredPoints);

private:
    pcl::search::KdTree<pcl::PointXYZ>::Ptr tree;
};

#endif // PCLDETECT_H
