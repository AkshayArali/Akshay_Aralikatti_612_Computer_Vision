#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

#define HRES 640
#define VRES 480

int main(int argc, char** argv)
{
    namedWindow("Capture Example", WINDOW_NORMAL);

    int dev = 0;
    if (argc > 1) {
        sscanf(argv[1], "%d", &dev);
        printf("Using camera %s\n", argv[1]);
    } else {
        printf("Using default camera\n");
    }

    // Use modern VideoCapture instead of deprecated CvCapture
    VideoCapture capture(dev);
    if (!capture.isOpened()) {
        cout << "Error: Cannot open camera!" << endl;
        return -1;
    }

    // Set resolution
    capture.set(CAP_PROP_FRAME_WIDTH, HRES);
    capture.set(CAP_PROP_FRAME_HEIGHT, VRES);

    Mat frame, gray, canny_frame, cdst;
    vector<Vec4i> lines;

    while (true) {
        if (!capture.read(frame)) {
            cout << "No frame captured" << endl;
            break;
        }

        // Convert to grayscale and detect edges
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        Canny(gray, canny_frame, 50, 200, 3);

        // Convert edges to color image
        cvtColor(canny_frame, cdst, COLOR_GRAY2BGR);

        // Apply Hough Line Transform
        HoughLinesP(canny_frame, lines, 1, CV_PI / 180, 50, 50, 10);

        // Draw detected lines
        for (size_t i = 0; i < lines.size(); i++) {
            Vec4i l = lines[i];
            line(frame, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
        }

        // Display the result
        imshow("Capture Example", frame);

        // Press 'q' to exit
        if (waitKey(10) == 'q') break;
    }

    // Release resources
    capture.release();
    destroyAllWindows();

    return 0;
}
