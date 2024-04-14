/*
version 1.1  Author: Geo
Using libcamera to connect OpenCV. Fail

version 1.2  
Using Prof libcam2opencv.h rewrite cpp fail

version 1.3
add callback function
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include "libcam2opencv.h"
#include "arm_sys.h"

using namespace cv;
using namespace std;

class MyCallback : public Libcam2OpenCV::Callback {
public:
    mg90s servo0, servo1, servo2, servo3;

    MyCallback() : servo0(21), servo1(16), servo2(20), servo3(26) {}

    virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &metadata) override {
        processFrame(frame);
    }

    void processFrame(const cv::Mat &frame) {
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

            handleServo(frame, objectBoundingRectangle);
        }

        imshow("Detection", frame);
        if (waitKey(10) == 27) {
            cout << "Exiting..." << endl;
            exit(0);
        }
    }

    void handleServo(const Mat& frame, const Rect& rect) {
        int frameCenterX = frame.cols / 2;
        int frameCenterY = frame.rows / 2;
        int objCenterX = rect.x + rect.width / 2;
        int objCenterY = rect.y + rect.height / 2;
        double areaRatio = (double)rect.area() / (frame.rows * frame.cols);

        auto onTargetReached = []() {
            std::cout << "Target angle reached." << std::endl;
        };

        // Adjust horizontal position
        if (objCenterX < frameCenterX) {
            servo0.setTargetAngleAsync(-10, onTargetReached); // Move right
        } else if (objCenterX > frameCenterX) {
            servo0.setTargetAngleAsync(10, onTargetReached); // Move left
        }

        // Adjust vertical position and forward movement
        if (objCenterY < frameCenterY) {
            servo1.setTargetAngleAsync(5, onTargetReached);
            servo2.setTargetAngleAsync(5, onTargetReached); // Move up and forward
        } else if (objCenterY > frameCenterY) {
            servo1.setTargetAngleAsync(-5, onTargetReached);
            servo2.setTargetAngleAsync(-5, onTargetReached); // Move down and forward
        }

        // Check if object is well-centered and at the desired scale
        if (abs(objCenterX - frameCenterX) < 20 && abs(objCenterY - frameCenterY) < 20 && areaRatio > 0.05) {
            servo3.setTargetAngleAsync(90, onTargetReached); // Activate the gripping mechanism
        }
    }
};

int main() {
    Libcam2OpenCV cameraInterface;
    MyCallback frameProcessor;

    cameraInterface.registerCallback(&frameProcessor);

    Libcam2OpenCVSettings settings;
    settings.width = 640;  // Set desired width
    settings.height = 480;  // Set desired height
    settings.framerate = 30;  // Set desired framerate

    try {
        cameraInterface.start(settings);
        std::this_thread::sleep_for(std::chrono::seconds(10));  // Run for a limited time or handle with a different way
        cameraInterface.stop();
    } catch (const std::exception& e) {
        cerr << "Exception caught during camera operation: " << e.what() << endl;
        return -1;
    }

    return 0;
}
