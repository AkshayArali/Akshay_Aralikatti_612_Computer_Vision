
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core.hpp>

#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace cv;
using namespace std;

#define HRES 640
#define VRES 480

int main(int argc, char** argv)
{

    namedWindow("Detected_Circles", WINDOW_AUTOSIZE);
    VideoCapture capture;
    Mat frame;
    int dev=0;
    Mat gray;

    vector<Vec3f> circles;


    if(argc > 1)
    {
        sscanf(argv[1], "%d", &dev);
        printf("using %s\n", argv[1]);
    }
    else if(argc == 1)
        printf("using default\n");

    else
    {
        printf("usage: capture [dev]\n");
        exit(-1);
    }

    capture.open(dev);
    capture.set(CAP_PROP_FRAME_WIDTH, HRES);
    capture.set(CAP_PROP_FRAME_HEIGHT, VRES);

    if(!capture.isOpened())
    {
        printf("could not open camera.\n");
        return -1;
    }
    while(1)
    {
        capture >> frame;
        if(frame.empty()) break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        // Canny(gray, gray, 50, 200, 3);

        medianBlur(gray, gray, 5);
        vector<Vec3f> circles;
        HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
                    gray.rows/16,  // change this value to detect circles with different distances to each other
                    100, 30, 1, 30 // change the last two parameters
                // (min_radius & max_radius) to detect larger circles
        );
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Vec3i c = circles[i];
            Point center = Point(c[0], c[1]);
            // circle center
            circle( frame, center, 1, Scalar(0,100,100), 3, LINE_AA);
            // circle outline
            int radius = c[2];
            circle( frame, center, radius, Scalar(255,0,255), 3, LINE_AA);
        }
        imshow("Detected_Circles", frame);

        char c = (char)waitKey(10);
        if(c=='q') break;
   
    } 
    capture.release();
    destroyWindow("Detected_Circles");
    return 0;

}