#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    //Load the image in greyscale (Sobel works best on single channel)
    Mat src = imread("frame_100.jpeg", IMREAD_GRAYSCALE);
    if(src.empty())
    {
        cerr << "Error: Unable to load image!" << endl;
        return -1;
    }

    //Resize the image to 600x300
    resize(src, src, Size(600, 300));

    //Declare matrices to store the gradients
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y, grad;

    //Parameters for the sobel operator
    int ddepth = CV_16S; //Use a 16-bit signed output to caputre negative gradients
    int scale = 1;
    int delta = 0;

    //Applying Sobel Operator in X direction
    Sobel(src, grad_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT);
    //Applying Sobel operator in Y direction
    Sobel(src, grad_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT);

    //Convert the gradients to absolute values and then to 8-bit(CV_8U)
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);

    //Combining the gradients 
    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

    //Resize the ouput the image to 600x1200
    resize(grad, grad, Size(600, 300));

    //save the images
    if(!imwrite("original_Image.jpg", src))
    {   
        cout<< "Error: faild to save original image"<< endl;
    }
    if(!imwrite("sobe_image.jpg", grad))
    {
        cout << "Error: failed to save sobel image" << endl;
    }

    //Displaying the resulting image
    imshow("Original Image", src);
    imshow("Sobel Edge Detection", grad);

    waitKey(0); //Wait for a key press
    return 0;
}