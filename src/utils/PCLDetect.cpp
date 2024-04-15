#include "PCLDetect.h"

// Constructor
PCLDetect::PCLDetect() {
    // Initialize a KD-Tree for efficient spatial searching
    tree.reset(new pcl::search::KdTree<pcl::PointXYZ>);
}

// Destructor
PCLDetect::~PCLDetect() {
    // Cleanup is handled by smart pointers automatically
}

// Method to cluster lidar data
void PCLDetect::clusterLidarData(const std::vector<LidarPoint>& inputPoints, std::vector<std::vector<LidarPoint>>& clusteredPoints) {
    // Create a new point cloud object
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
    cloud->width = inputPoints.size();  // Set the width of the point cloud to the number of input points
    cloud->height = 1;                  // Point cloud is unorganized
    cloud->points.resize(cloud->width); // Resize the point array to accommodate all points

    // Populate the point cloud data
    for (size_t i = 0; i < inputPoints.size(); ++i) {
        // Calculate x and y using polar coordinates (r and phi)
        cloud->points[i].x = inputPoints[i].x; // Directly use the precomputed X coordinate
        cloud->points[i].y = inputPoints[i].y; // Directly use the precomputed Y coordinate
    cloud->points[i].z = 0; // Z coordinate is 0 for 2D data
    }

    // Set the input cloud for the KD-Tree to enable fast spatial searches
    tree->setInputCloud(cloud);

    // Prepare a container for cluster indices
    std::vector<pcl::PointIndices> cluster_indices;
    // Create an object for Euclidean cluster extraction
    pcl::EuclideanClusterExtraction<pcl::PointXYZ> ec;
    ec.setClusterTolerance(0.5);        // Spatial cluster tolerance set to 20 cm
    ec.setMinClusterSize(10);            // Minimum number of points for a cluster
    ec.setMaxClusterSize(250);          // Maximum number of points for a cluster
    ec.setSearchMethod(tree);           // Set the search method as KD-Tree
    ec.setInputCloud(cloud);            // Set the input point cloud for clustering
    ec.extract(cluster_indices);        // Perform the clustering operation

    // Iterate through each cluster
    for (auto it = cluster_indices.begin(); it != cluster_indices.end(); ++it) {
        std::vector<LidarPoint> cluster; // Store points for the current cluster
        for (const auto& idx : it->indices) { // Iterate through all points in the current cluster
            LidarPoint pt;
            pt.x = cloud->points[idx].x; // Extract the X coordinate
            pt.y = cloud->points[idx].y; // Extract the Y coordinate
            pt.r = sqrt(pt.x * pt.x + pt.y * pt.y); // Calculate r from x and y
            pt.phi = atan2(pt.y, pt.x); // Calculate phi from y and x
            cluster.push_back(pt); // Add the point to the current cluster
        }
        clusteredPoints.push_back(cluster); // Add the current cluster to the list of clusters
    }
}
