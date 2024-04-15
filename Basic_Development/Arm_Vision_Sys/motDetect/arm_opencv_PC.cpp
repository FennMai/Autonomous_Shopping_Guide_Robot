/*
this code is run in the PC to detect the object and control the arm to move to the object

*/
#include <opencv2/opencv.hpp>
#include <iostream>
#include "arm_sys.h" 

using namespace cv;
using namespace std;

int main() {
    auto onTargetReached = []() {
        std::cout << "Target angle reached." << std::endl;
    };

    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Error opening video capture." << endl;
        return -1;
    }

    Mat frame;
    mg90s servo0(21), servo1(20), servo2(16), servo3(26); 

    while (capture.read(frame)) {
        if (frame.empty()) {
            cerr << "No captured frame -- Break!" << endl;
            break;
        }

        Mat hsv, redMask1, redMask2, redMask;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        inRange(hsv, Scalar(0, 70, 50), Scalar(10, 255, 255), redMask1);
        inRange(hsv, Scalar(170, 70, 50), Scalar(180, 255, 255), redMask2);
        redMask = redMask1 | redMask2;

        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
        morphologyEx(redMask, redMask, MORPH_CLOSE, kernel);

        vector<vector<Point>> contours;
        findContours(redMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        Rect objectBoundingRectangle = Rect(0, 0, 0, 0);
        if (!contours.empty()) {
            vector<Point> largestContour = contours[0];
            for (size_t i = 1; i < contours.size(); i++) {
                if (contourArea(contours[i]) > contourArea(largestContour)) {
                    largestContour = contours[i];
                }
            }
            objectBoundingRectangle = boundingRect(largestContour);
            rectangle(frame, objectBoundingRectangle, Scalar(0, 255, 0), 2);

            int frameCenterX = frame.cols / 2;
            int frameCenterY = frame.rows / 2;
            int objCenterX = objectBoundingRectangle.x + objectBoundingRectangle.width / 2;
            int objCenterY = objectBoundingRectangle.y + objectBoundingRectangle.height / 2;
            double areaRatio = (double)objectBoundingRectangle.area() / (frame.rows * frame.cols);

            // Adjust horizontal position
            if (objCenterX < frameCenterX) {
                servo0.setTargetAngleAsync(-10,onTargetReached); // Move right
            } else if (objCenterX > frameCenterX) {
                servo0.setTargetAngleAsync(10,onTargetReached); // Move left
            }

            // Adjust vertical position and forward movement
            if (objCenterY < frameCenterY) {
                servo1.setTargetAngleAsync(5,onTargetReached);
                servo2.setTargetAngleAsync(5,onTargetReached); // Move up and forward
            } else if (objCenterY > frameCenterY) {
                servo1.setTargetAngleAsync(-5,onTargetReached);
                servo2.setTargetAngleAsync(-5,onTargetReached); // Move down and forward
            }

            // Check if object is well-centered and at the desired scale
            if (abs(objCenterX - frameCenterX) < 20 && abs(objCenterY - frameCenterY) < 20 && areaRatio > 0.05) {
                servo3.setTargetAngleAsync(90,onTargetReached); // Activate the gripping mechanism
            }
        }

        imshow("Detection", frame);

        if (waitKey(10) == 27) {
            break;
        }
    }

    capture.release();
    destroyAllWindows();
    return 0;
}
