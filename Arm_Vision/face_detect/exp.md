OpenCV 默认使用 Video4Linux (V4L2) 接口来接入摄像头设备，这是 Linux 系统上广泛支持的标准摄像头接口。然而，libcamera 是一个较新的库，专为提供一个更现代的、灵活的接口来管理复杂摄像头硬件而设计。它提供了更丰富的控制特性和更好的支持新硬件的能力，但不是基于 V4L2 构建的。因此，标准的 OpenCV 摄像头接口不能直接通过自身的方法来调用 libcamera，因为这两者背后的驱动模型和接口完全不同。

如何通过提供的代码解决这个问题
你提供的 Libcam2OpenCV 类通过以下方式解决了 OpenCV 不能直接使用 libcamera 的问题：

桥接模式：

该类充当了 OpenCV 和 libcamera 之间的桥梁。通过编写一个包装类（wrapper），它初始化和控制 libcamera 来捕获视频流，然后将这些视频帧转换为 OpenCV 可以直接处理的格式（例如 BGR888）。
直接内存访问：

通过 mmap 函数映射 libcamera 的帧缓冲区，该类可以直接访问图像数据，并将其转换为 OpenCV 的 cv::Mat 对象。这使得可以在 OpenCV 中应用各种图像处理算法，如人脸检测等。
回调机制：

该类利用 libcamera 的请求和回调系统来处理摄像头帧。每当一个请求（包含一个或多个帧的数据）完成时，都会触发一个回调函数（如 requestComplete），在这个函数中可以对捕获的帧进行处理。
配置和控制：

通过 libcamera 的 API，这个类能够详细配置摄像头的各种参数，包括分辨率、帧率和图像格式，这些都是直接通过 libcamera 进行设置的，而非依赖 V4L2。

face_det_callb.cpp is the last version. 