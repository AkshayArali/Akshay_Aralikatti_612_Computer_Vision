#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>  // Include this for mkdir
#include <sys/types.h>

using namespace cv;
using namespace std;



Mat thresholdFrame(const Mat& frame)
{
    Mat gray, binary;
    cvtColor(frame, gray, COLOR_BGR2GRAY);  // Convert to grayscale
    threshold(gray, binary, 200, 255, THRESH_BINARY); // Apply threshold
    return binary;
}

pair<int, int> COM(const Mat& frame)
{
    int sumX = 0, sumY = 0, sumIntensity = 0;

    for (int y = 0; y < frame.rows; y++)
    {
        for (int x = 0; x < frame.cols; x++)
        {
            int intensity = frame.at<uchar>(y, x);
            if (intensity > 200)  // Threshold condition
            {
                sumX += x * intensity;
                sumY += y * intensity;
                sumIntensity += intensity;
            }
        }
    }

    if (sumIntensity == 0)  // Prevent division by zero
        return make_pair(frame.cols / 2, frame.rows / 2);

    int centerX = sumX / sumIntensity;
    int centerY = sumY / sumIntensity;

    cout << "Computed COM: (" << centerX << ", " << centerY << ")" << endl;

    return make_pair(centerX, centerY);
}

Mat drawCrosshair(const Mat& frame, const pair<int, int>& coor)
{
    Mat frameCopy = frame.clone();  // Clone to avoid modifying the original
    line(frameCopy, Point(coor.first, 0), Point(coor.first, frame.rows), Scalar(0, 255, 0), 1);
    line(frameCopy, Point(0, coor.second), Point(frame.cols, coor.second), Scalar(0, 255, 0), 1);
    return frameCopy;
}

int main(int argc, char** argv)
{
    Mat mat_frame, orig_frame;
    VideoCapture vcap("output.mp4");
    VideoCapture original("Dark-Room-Laser-Spot.mpeg");
    int frameCounter = 0;

    if (!vcap.isOpened())
    {
        cout << "Error: Cannot open video file" << endl;
        return -1;
    }
    if (!original.isOpened())
    {
        cout << "Error: Cannot open video file" << endl;
        return -1;
    }

    // Ensure the directory exists before saving images
    std::string directory = "annotated_folder/";
    struct stat info;
    if (stat(directory.c_str(), &info) != 0) {
        cout << "Directory does not exist, creating: " << directory << endl;
        mkdir(directory.c_str(), 0777); // Create directory
    }

    cv::namedWindow("Original_Video", cv::WINDOW_NORMAL);
    cv::namedWindow("Annotated_Frame", cv::WINDOW_NORMAL);

    while (vcap.read(mat_frame) && original.read(orig_frame))
    {
        if (mat_frame.empty()) {
            cout << "No frame" << endl;
            break;
        }

        Mat threshold_frame = thresholdFrame(mat_frame);
        auto framePair = COM(threshold_frame);
        Mat annotated_frame = drawCrosshair(orig_frame, framePair);

        std::string filename = "CH_frame_" + std::to_string(frameCounter) + ".png";
        std::string fullPath = directory + filename;

        // Ensure annotated frame is not empty before saving
        if (annotated_frame.empty()) {
            cout << "Error: Annotated frame is empty, skipping save." << endl;
        } else {
            bool success = cv::imwrite(fullPath, annotated_frame);
            if (!success) {
                cout << "Error: Failed to save image at " << fullPath << endl;
            } else {
                cout << "Image saved successfully: " << fullPath << endl;
            }
        }

        // Display frames
        cv::imshow("Original_Video", mat_frame);
        cv::imshow("Annotated_Frame", annotated_frame);
        cv::resizeWindow("Original_Video", 800, 600);
        cv::resizeWindow("Annotated_Frame", 800, 600);

        char c = cv::waitKey(33);
        if (c == 'q') break;

        frameCounter++;
    }

    vcap.release();
    destroyAllWindows();
    return 0;
}
