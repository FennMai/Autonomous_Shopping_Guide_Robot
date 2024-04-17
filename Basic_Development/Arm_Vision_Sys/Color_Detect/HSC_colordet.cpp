/*
Author: Geo date: 15 Apr 2024 time:12:37
Version: 1.0
it is using for color detection, because we need to know the color HSV value of the object.
*/
#include "libcam2opencv.cpp"
#include "ColorDetectionCallback.cpp"

int main() {
    try {
        // 创建摄像头接口和设置对象
        Libcam2OpenCV cameraInterface;
        ColorDetectionCallback frameProcessor;
        cameraInterface.registerCallback(&frameProcessor);

        Libcam2OpenCVSettings settings;
        settings.width = 640;  // 设定摄像头宽度
        settings.height = 480;  // 设定摄像头高度
        settings.framerate = 30;  // 设定帧率

        // 启动摄像头
        cameraInterface.start(settings);
        std::cout << "Press enter to exit..." << std::endl;
        std::cin.get();  // 等待用户输入以退出
        cameraInterface.stop();
    } catch (const std::exception& e) {
        std::cerr << "Exception caught during camera operation: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
