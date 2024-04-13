/*
version 1.1  Author: Geo
Using libcamera to connect OpenCV. Fail
version 1.2  
Using Prof libcam2opencv.h rewrite cpp fail
version 1.3
add callback function MK" servo2 control the up and down 
version 1.4 
searching prograss need a loop control and callback function
version 1.5
All function 
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
    bool redDetected = false;  // Flag to check if red object is detected

    MyCallback() : servo0(21), servo1(16), servo2(20), servo3(26) {
        initiateSLE(); // Start the SLE program
    }

    void initiateSLE() {
        // Start the SLE program: servo0 turns left 90, right 180, left 180 degrees
        if (!redDetected) {
            servo0.setTargetAngleAsync(-90, [this]() {
                servo0.setTargetAngleAsync(90, [this]() {
                    servo0.setTargetAngleAsync(-90, [this]() {
                        initiateSLE(); // Continue the SLE loop recursively
                    });
                });
            });
        }
    }

    virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &metadata) override {
        if (!redDetected) {
            processFrameForRedDetection(frame);
        } else {
            processFrame(frame);
        }
    }

    void processFrameForRedDetection(const cv::Mat &frame) {
        Mat hsv, redMask1, redMask2;
        cvtColor(frame, hsv, COLOR_BGR2HSV);
        inRange(hsv, Scalar(0, 70, 50), Scalar(10, 255, 255), redMask1);
        inRange(hsv, Scalar(170, 70, 50), Scalar(180, 255, 255), redMask2);

        Mat redMask = redMask1 | redMask2;
        if (countNonZero(redMask) > 0) {
            redDetected = true; // Red object detected, stop SLE and switch to ALE program
        }
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

        if (!contours.empty()) {
            auto largestContour = max_element(contours.begin(), contours.end(),
                                              [](const vector<Point>& a, const vector<Point>& b) {
                                                  return contourArea(a) < contourArea(b);
                                              });
            Rect objectBoundingRectangle = boundingRect(*largestContour);
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

        // ALE (Aligning and Leveling Entity): Ensure object is centered
        checkAndAdjustCentering(objCenterX, frameCenterX, objCenterY, frameCenterY);

        // BLE (Bounding and Leveling Entity): Adjust based on object size
        if (areaRatio < 0.75) {
            adjustForScale(objCenterX, frameCenterX, objCenterY, frameCenterY);
        } else {
            // CLE (Clamping and Leveling Entity): Final adjustments
            performFinalAdjustments();
        }
    }

    void checkAndAdjustCentering(int objCenterX, int frameCenterX, int objCenterY, int frameCenterY) {
        // Adjust servos to center the detected object within the frame
        if (abs(objCenterX - frameCenterX) > 20) {
            if (objCenterX < frameCenterX) {
                servo0.setTargetAngleAsync(-10, []() { cout << "Adjusting left." << endl; });
            } else {
                servo0.setTargetAngleAsync(10, []() { cout << "Adjusting right." << endl; });
            }
        }

        if (abs(objCenterY - frameCenterY) > 20) {
            if (objCenterY < frameCenterY) {
                servo1.setTargetAngleAsync(5, []() { cout << "Adjusting up." << endl; });
            } else {
                servo1.setTargetAngleAsync(-5, []() { cout << "Adjusting down." << endl; });
            }
        }
    }

    void adjustForScale(int objCenterX, int frameCenterX, int objCenterY, int frameCenterY) {
        // Move forward to adjust the scale of the object in the frame
        servo2.setTargetAngleAsync(10, []() { cout << "Moving forward." << endl; });
        checkAndAdjustCentering(objCenterX, frameCenterX, objCenterY, frameCenterY);
    }

    void performFinalAdjustments() {
        // Perform final gripping actions and reset the servo positions
        servo3.setTargetAngleAsync(90, []() { cout << "Gripping." << endl; });
        servo0.setTargetAngleAsync(90, []() { cout << "Centering." << endl; });  // Return to center position
        servo3.setTargetAngleAsync(-90, []() { cout << "Releasing grip." << endl; });
        exit(0); // Terminate the program after final action
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
        cout << "Press enter to exit..." << endl;
        cin.get();  // Wait for user input to exit
        cameraInterface.stop();
    } catch (const std::exception& e) {
        cerr << "Exception caught during camera operation: " << e.what() << endl;
        return -1;
    }

    return 0;
}
