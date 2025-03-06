// #include <stdio.h>
// #include <stdlib.h>
// #include <iostream>
// #include <opencv2/core/core.hpp>
// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc.hpp>
// #include <time.h>  // For clock_gettime

// using namespace cv;
// using namespace std;

// #define ESCAPE_KEY (27)
// #define SYSTEM_ERROR (-1)

// enum Mode { NONE, CANNY, SOBEL };
// Mode currentMode = NONE;

// int main()
// {
//     VideoCapture cam0(0);
//     namedWindow("video_display");
//     char winInput;

//     if (!cam0.isOpened())
//     {
//         exit(SYSTEM_ERROR);
//     }

//     cam0.set(CAP_PROP_FRAME_WIDTH, 640);
//     cam0.set(CAP_PROP_FRAME_HEIGHT, 480);

//     int frameCount = 0;
//     struct timespec start_time, end_time;
//     clock_gettime(CLOCK_MONOTONIC, &start_time);

//     while (1)
//     {
//         Mat frame, gray, output;
//         cam0.read(frame);

//         cvtColor(frame, gray, COLOR_BGR2GRAY);

//         switch (currentMode)
//         {
//             case CANNY:
//                 Canny(gray, output, 100, 200);
//                 break;
//             case SOBEL:
//             {
//                 Mat grad_x, grad_y;
//                 Mat abs_grad_x, abs_grad_y;
//                 Sobel(gray, grad_x, CV_16S, 1, 0, 3);
//                 Sobel(gray, grad_y, CV_16S, 0, 1, 3);
//                 convertScaleAbs(grad_x, abs_grad_x);
//                 convertScaleAbs(grad_y, abs_grad_y);
//                 addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);
//                 break;
//             }
//             case NONE:
//                 output = frame.clone();
//                 break;
//         }

//         imshow("video_display", output);
//         frameCount++;

//         winInput = waitKey(1);
//         if (winInput == ESCAPE_KEY)
//         {
//             break;
//         }
//         else if (winInput == 'c')
//         {
//             currentMode = CANNY;
//             cout << "Canny Edge Detection Enabled" << endl;
//         }
//         else if (winInput == 's')
//         {
//             currentMode = SOBEL;
//             cout << "Sobel Edge Detection Enabled" << endl;
//         }
//         else if (winInput == 'n')
//         {
//             currentMode = NONE;
//             cout << "No Filter (Original Feed)" << endl;
//         }
//     }

//     clock_gettime(CLOCK_MONOTONIC, &end_time);
//     double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 
//                           (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
//     double fps = frameCount / elapsed_time;

//     cout << "Elapsed Time: " << elapsed_time << " seconds" << endl;
//     cout << "Average FPS: " << fps << endl;

//     destroyWindow("video_display");
//     return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <time.h>  // For clock_gettime

using namespace cv;
using namespace std;

#define ESCAPE_KEY (27)
#define SYSTEM_ERROR (-1)

enum Mode { NONE, CANNY, SOBEL };
Mode currentMode = NONE;

int cannyThreshold1 = 100, cannyThreshold2 = 200;  // Default Canny thresholds
int frameCount = 0;
struct timespec start_time, end_time;

// Function to update Canny thresholds using a trackbar
void on_trackbar(int, void *)
{
    cout << "Updated Canny Thresholds: " << cannyThreshold1 << ", " << cannyThreshold2 << endl;
}

int main()
{
    VideoCapture cam0(0);
    if (!cam0.isOpened())
    {
        cerr << "Error: Could not open webcam!" << endl;
        exit(SYSTEM_ERROR);
    }

    namedWindow("video_display");
    namedWindow("Controls", WINDOW_AUTOSIZE);
    createTrackbar("Canny Threshold 1", "Controls", &cannyThreshold1, 255, on_trackbar);
    createTrackbar("Canny Threshold 2", "Controls", &cannyThreshold2, 255, on_trackbar);

    cam0.set(CAP_PROP_FRAME_WIDTH, 640);
    cam0.set(CAP_PROP_FRAME_HEIGHT, 480);

    clock_gettime(CLOCK_MONOTONIC, &start_time);

    while (1)
    {
        Mat frame, gray, output;
        cam0.read(frame);
        if (frame.empty())
        {
            cerr << "Error: Empty frame captured!" << endl;
            break;
        }

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        struct timespec frame_start, frame_end;
        clock_gettime(CLOCK_MONOTONIC, &frame_start);

        switch (currentMode)
        {
            case CANNY:
                Canny(gray, output, cannyThreshold1, cannyThreshold2);
                break;
            case SOBEL:
            {
                Mat grad_x, grad_y, abs_grad_x, abs_grad_y;
                Sobel(gray, grad_x, CV_16S, 1, 0, 3);
                Sobel(gray, grad_y, CV_16S, 0, 1, 3);
                convertScaleAbs(grad_x, abs_grad_x);
                convertScaleAbs(grad_y, abs_grad_y);
                addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);
                break;
            }
            case NONE:
                output = frame.clone();
                break;
        }

        clock_gettime(CLOCK_MONOTONIC, &frame_end);
        double frame_time = (frame_end.tv_sec - frame_start.tv_sec) + 
                            (frame_end.tv_nsec - frame_start.tv_nsec) / 1e9;
        double fps_current = 1.0 / frame_time;

        imshow("video_display", output);
        frameCount++;

        char winInput = waitKey(1);
        if (winInput == ESCAPE_KEY)
            break;
        else if (winInput == 'c')
        {
            currentMode = CANNY;
            cout << "[Mode] Canny Edge Detection Enabled" << endl;
        }
        else if (winInput == 's')
        {
            currentMode = SOBEL;
            cout << "[Mode] Sobel Edge Detection Enabled" << endl;
        }
        else if (winInput == 'n')
        {
            currentMode = NONE;
            cout << "[Mode] No Filter (Original Feed)" << endl;
        }

        cout << "Current FPS: " << fps_current << "\r" << flush;
    }

    clock_gettime(CLOCK_MONOTONIC, &end_time);
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) + 
                          (end_time.tv_nsec - start_time.tv_nsec) / 1e9;
    double average_fps = frameCount / elapsed_time;

    cout << "\nElapsed Time: " << elapsed_time << " seconds" << endl;
    cout << "Average FPS: " << average_fps << endl;

    destroyAllWindows();
    return 0;
}
