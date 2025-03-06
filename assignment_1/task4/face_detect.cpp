#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

// Function to detect faces and eyes
void detectAndDisplay(Mat frame, CascadeClassifier face_cascade, CascadeClassifier eyes_cascade) {
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    // Detect faces
    vector<Rect> faces;
    face_cascade.detectMultiScale(frame_gray, faces, 1.1, 3, 0, Size(30, 30));
    for (size_t i = 0; i < faces.size(); i++) {
        Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
        ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4);

        Mat faceROI = frame_gray(faces[i]);
        vector<Rect> eyes;

        // Detect eyes within the face region
        eyes_cascade.detectMultiScale(faceROI, eyes);
        for (size_t j = 0; j < eyes.size(); j++) {
            Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
            int radius = cvRound((eyes[j].width + eyes[j].height) * 0.25);
            circle(frame, eye_center, radius, Scalar(255, 0, 0), 4);
        }
    }

    // Display the result
    imshow("Capture - Face detection", frame);
    waitKey(0);
}

int main() {
    // Load Haar Cascade Classifiers
    CascadeClassifier face_cascade, eyes_cascade;
    if (!face_cascade.load("haarcascade_frontalface_default.xml")) {
        cerr << "Error loading face cascade!" << endl;
        return -1;
    }
    if (!eyes_cascade.load("haarcascade_eye.xml")) {
        cerr << "Error loading eyes cascade!" << endl;
        return -1;
    }

    // Load the professor's headshot
    Mat frame = imread("Sam-Headshot.png");
    if (frame.empty()) {
        cerr << "Error loading image!" << endl;
        return -1;
    }

    // Detect and display
    detectAndDisplay(frame, face_cascade, eyes_cascade);

    return 0;
}
