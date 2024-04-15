/*
Geo OpenCV ColorDetect 1.0
10 Apr 00:44
*/

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Error opening video capture." << endl;
        return -1;
    }

    Mat frame;
    while (capture.read(frame)) {
        if (frame.empty()) {
            cerr << "No captured frame -- Break!" << endl;
            break;
        }

        Mat hsv, redMask1, redMask2, redMask;
        // 转换到HSV色彩空间
        cvtColor(frame, hsv, COLOR_BGR2HSV);

        // 定义红色的两个HSV范围
        inRange(hsv, Scalar(0, 70, 50), Scalar(10, 255, 255), redMask1);
        inRange(hsv, Scalar(170, 70, 50), Scalar(180, 255, 255), redMask2);

        // 合并两个红色范围的掩模
        redMask = redMask1 | redMask2;

        // 使用形态学操作改善掩模结果
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
        morphologyEx(redMask, redMask, MORPH_CLOSE, kernel);

        // 寻找红色物体的轮廓
        vector<vector<Point>> contours;
        findContours(redMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
        if (!contours.empty()) {
            // 取最大的轮廓
            vector<Point> largestContour = contours[0];
            for (size_t i = 1; i < contours.size(); i++) {
                if (contourArea(contours[i]) > contourArea(largestContour)) {
                    largestContour = contours[i];
                }
            }
            objectBoundingRectangle = boundingRect(largestContour);
            rectangle(frame, objectBoundingRectangle, Scalar(0, 255, 0), 2);
        }

        int cols = frame.cols;
        int rows = frame.rows;
        int cellWidth = cols / 3;
        int cellHeight = rows / 3;

        // 判断物体中心的位置
        Point center = Point(objectBoundingRectangle.x + objectBoundingRectangle.width/2,
                             objectBoundingRectangle.y + objectBoundingRectangle.height/2);
        int cellCol = center.x / cellWidth;
        int cellRow = center.y / cellHeight;

        // 输出移动指示
        if (cellRow == 1 && cellCol == 1) {  // 中心区域
            putText(frame, "Object is centered", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0), 2);
        } else {
            if (cellCol < 1) {
                putText(frame, "Move Right", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 2);
            } else if (cellCol > 1) {
                putText(frame, "Move Left", Point(50, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
            }
        }

        // 显示结果
        imshow("Red Object Detection", frame);

        // 按 'ESC' 键退出
        if (waitKey(10) == 27) {
            break;
        }
    }

    // 释放资源
    capture.release();
    destroyAllWindows();
    return 0;
}
