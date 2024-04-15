#ifndef __COLOR_DETECTION_H
#define __COLOR_DETECTION_H

#include "libcam2opencv.h"
#include <opencv2/opencv.hpp>

class ColorDetectionCallback : public Libcam2OpenCV::Callback {
public:
    // 这个方法会在每个摄像头帧准备好时被调用
    virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &metadata) override {
        cv::Mat hsvFrame;
        cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV); // 将图像从BGR转换到HSV色彩空间

        // 定义中心区域的矩形
        int rectSize = 100; // 中心矩形的大小
        cv::Point center(frame.cols / 2, frame.rows / 2);
        cv::Rect centerRect(center.x - rectSize / 2, center.y - rectSize / 2, rectSize, rectSize);

        // 提取中心区域并计算其HSV平均值
        cv::Mat centerROI = hsvFrame(centerRect);
        cv::Scalar averageHSV = cv::mean(centerROI); // 计算平均HSV值
        std::cout << "Average HSV in center: " << averageHSV << std::endl;
    }
};

#endif // __COLOR_DETECTION_H
