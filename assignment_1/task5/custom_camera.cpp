#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
    // Open the default camera (camera index 0)
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open the camera." << std::endl;
        return -1;
    }

    Mat frame;
    while (true) {
        // Capture a new frame
        cap >> frame;

        // Draw a green border (4-pixel width)
        rectangle(frame, Point(0, 0), Point(frame.cols - 1, frame.rows - 1), Scalar(0, 255, 0), 4);

        // Draw yellow crosshairs (1-pixel width)
        line(frame, Point(frame.cols / 2, 0), Point(frame.cols / 2, frame.rows), Scalar(0, 255, 255), 1);
        line(frame, Point(0, frame.rows / 2), Point(frame.cols, frame.rows / 2), Scalar(0, 255, 255), 1);

        // Display the frame with annotations
        imshow("Camera Stream", frame);

        // Break the loop if the user presses any key
        if (waitKey(30) >= 0) break;
    }

    return 0;
}
