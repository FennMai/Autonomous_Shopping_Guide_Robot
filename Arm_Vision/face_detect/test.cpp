/*
Geo OpenCV FaceDetect 1.2
10 Apr 00:10
*/
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    // 创建级联分类器对象
    CascadeClassifier face_cascade;

    // 加载级联文件，确保路径是正确的
    if (!face_cascade.load("/usr/local/share/opencv4/haarcascades/haarcascade_frontalface_default.xml")) {
        cerr << "Error loading face cascade file. Check the path!" << endl;
        return -1;
    }

    // 打开视频捕捉设备，默认的摄像头通常是索引号0
    VideoCapture capture(0);
    if (!capture.isOpened()) {
        cerr << "Error opening video capture." << endl;
        return -1;
    }

    Mat frame, gray;
    while (capture.read(frame)) {
        if (frame.empty()) {
            cerr << "No captured frame -- Break!" << endl;
            break;
        }

        // 转换为灰度图，这对检测效率更高
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        // 使用直方图均衡化改善灰度图像的对比度
        equalizeHist(gray, gray);

        // 检测人脸
        vector<Rect> faces;
        face_cascade.detectMultiScale(gray, faces);

        // 在检测到的每个人脸周围画一个红色的矩形
        for (const auto& face : faces) {
            rectangle(frame, face, Scalar(0, 0, 255), 2);
        }

        // 显示结果
        imshow("Face Detection", frame);

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
