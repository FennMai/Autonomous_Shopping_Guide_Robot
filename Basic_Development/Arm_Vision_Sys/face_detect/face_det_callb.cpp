/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2020, Ideas on Board Oy.
 * Copyright (C) 2024, Bernd Porr
 * Copyright (C) 2021, kbarni https://github.com/kbarni/
 * edit by Geo
 */
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include "libcam2opencv.h"

using namespace cv;
using namespace std;

class FrameProcessor : public Libcam2OpenCV::Callback {
public:
    CascadeClassifier face_cascade;

    FrameProcessor() {
        if (!face_cascade.load("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")) {
            cerr << "Error loading face cascade file. Check the path!" << endl;
            exit(-1);
        }
    }

    void hasFrame(const cv::Mat &frame, const libcamera::ControlList &metadata) override {
        Mat gray;
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        equalizeHist(gray, gray);

        vector<Rect> faces;
        face_cascade.detectMultiScale(gray, faces);

        for (const auto& face : faces) {
            rectangle(frame, face, Scalar(0, 0, 255), 2);
        }

        imshow("Face Detection", frame);
        if (waitKey(10) == 27) {
            cout << "ESC pressed - exiting..." << endl;
            exit(0);
        }
    }
};

int main() {
    Libcam2OpenCV cameraSystem;
    FrameProcessor processor;

    Libcam2OpenCVSettings settings;
    settings.width = 640;  // Set desired width
    settings.height = 480; // Set desired height
    settings.framerate = 30; // Set desired framerate

    cameraSystem.registerCallback(&processor);
    cameraSystem.start(settings);

    cout << "Press ESC on OpenCV window to terminate" << endl;
    while (true) {
        this_thread::sleep_for(chrono::milliseconds(10));
    }

    cameraSystem.stop();
    return 0;
}
