#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>

using namespace cv;
using namespace std;

#define ESCAPE_KEY (27)
#define FRAME_LIMIT 1800 // 60 seconds at 30 FPS

int main() {
    VideoCapture cam0(0);
    if (!cam0.isOpened()) {
        cerr << "Error: Could not open camera!" << endl;
        return -1;
    }

    cam0.set(CAP_PROP_FRAME_WIDTH, 640);
    cam0.set(CAP_PROP_FRAME_HEIGHT, 480);

    namedWindow("video_display", WINDOW_AUTOSIZE);

    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    
    int frame_count = 0;
    char winInput;

    while (frame_count < FRAME_LIMIT) {
        Mat frame;
        cam0.read(frame);
        imshow("video_display", frame);
        frame_count++;

        if ((winInput = waitKey(1)) == ESCAPE_KEY) {
            break;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    double fps = frame_count / elapsed_time;

    cout << "\n[INFO] OpenCV Capture Completed." << endl;
    cout << "Elapsed Time: " << elapsed_time << " seconds" << endl;
    cout << "Average FPS: " << fps << endl;

    destroyWindow("video_display");
    return 0;
}
