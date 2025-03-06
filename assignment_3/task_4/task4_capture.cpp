#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

char difftext[20];

int main( int argc, char** argv )
{
    Mat mat_frame, mat_gray, mat_diff, mat_gray_prev;
    VideoCapture vcap("with_clutter.mpeg");
    cv::namedWindow("Original_Video", cv::WINDOW_NORMAL);
    cv::namedWindow("Gray_Diff", cv::WINDOW_NORMAL);


    unsigned int diffsum, maxdiff;
    double percent_diff;


    //open the video stream and make sure it's opened
    // "0" is the default video device which is normally the built-in webcam
    // if(!vcap.open(0)) 
    // {
    //     std::cout << "Error opening video stream or file" << std::endl;
    //     return -1;
    // }
    // else
    // {
	//    std::cout << "Opened default camera interface" << std::endl;
    // }

    while(!vcap.read(mat_frame)) {
	std::cout << "No frame" << std::endl;
	cv::waitKey(33);
    }
	
    cv::cvtColor(mat_frame, mat_gray, cv::COLOR_BGR2RGB);

    mat_diff = mat_gray.clone();
    mat_gray_prev = mat_gray.clone();

    maxdiff = (mat_diff.cols)*(mat_diff.rows)*255;

    while(1)
    {
	if(!vcap.read(mat_frame)) {
		std::cout << "No frame" << std::endl;
		cv::waitKey();
	}
	
	cv::cvtColor(mat_frame, mat_gray, cv::COLOR_BGR2RGB);

	absdiff(mat_gray_prev, mat_gray, mat_diff);

	// worst case sum is resolution * 255
	diffsum = (unsigned int)cv::sum(mat_diff)[0]; // single channel sum

	percent_diff = ((double)diffsum / (double)maxdiff)*100.0;

        printf("percent diff=%lf\n", percent_diff);
        sprintf(difftext, "%8d",  diffsum);

        // tested in ERAU Jetson lab
	if(percent_diff > 0.5) cv::putText(mat_diff, difftext, cv::Point(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200,200,250), 1, cv::LINE_AA);

	cv::imshow("Original_Video", mat_frame);
	// cv::imshow("Gray Previous", mat_gray_prev);
	cv::imshow("Gray_Diff", mat_diff);

    cv::resizeWindow("Original_Video", 800, 600);
    cv::resizeWindow("Gray_Diff", 800, 600);


        char c = cv::waitKey(33); // take this out or reduce
        if( c == 'q' ) break;

	mat_gray_prev = mat_gray.clone();
    }

};