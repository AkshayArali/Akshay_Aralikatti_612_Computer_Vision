// #include <opencv2/opencv.hpp>
// #include <iostream>

// using namespace std;
// using namespace cv;

// int main()
// {
//     //Load the image in grayscale 
//     Mat src = imread("200th_frame.png", IMREAD_GRAYSCALE);
    
//     if(src.empty())
//     {
//         cerr << "Error: unable to load the image!" << endl;
//         return -1;
//     }

//     // Resize the image to 600x300
//     resize(src, src, Size(600, 300));

//     //Declare matrices to store the gradients
//     Mat grad_x, grad_y;
//     Mat abs_grad_x, abs_grad_y, grad;

//     //Parameters for the Canny operator


// }

#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

#define ESCAPE_KEY (27)

Mat src, src_gray;
Mat dst, detected_edges;
int lowThreshold = 0;
const int max_lowThreshold = 100;
const int canny_ratio = 3;
const int kernel_size = 3;
const char* window_name = "Edge Map";

static void CannyThreshold(int, void*)
{
    blur(src_gray, detected_edges, Size(3, 3));
    Canny(detected_edges, detected_edges, lowThreshold, lowThreshold * canny_ratio, kernel_size);
    dst = Scalar::all(0);
    src.copyTo(dst, detected_edges);
    imshow(window_name, dst);
}

int main()
{
    char winInput;

    // Load image directly from the same folder
    src = imread("200th_frame.png", IMREAD_COLOR); // Load image in color
    if (src.empty())
    {
        cout << "Could not open or find the image!" << endl;
        return -1;
    }

    // Prepare images
    dst.create(src.size(), src.type());
    cvtColor(src, src_gray, COLOR_BGR2GRAY);  // Convert to grayscale

    namedWindow(window_name, WINDOW_AUTOSIZE);
    createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

    while (true)
    {
        CannyThreshold(0, 0);

        winInput = waitKey(10);
        if (winInput == ESCAPE_KEY)
        {
            break;  // Exit on ESC key
        }
    }

    // Save the output edge-detected image
    imwrite("canny_edges_200th_frame.png", dst);
    cout << "Edge-detected image saved as 'canny_edges_200th_frame.png'" << endl;

    return 0;
}

