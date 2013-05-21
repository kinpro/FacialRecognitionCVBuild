//
//  main.cpp
//  OpenCV Test
//
//  Created by Derek Quach on 5/20/13.
//  Copyright (c) 2013 Derek Quach. All rights reserved.
//

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main (int argc, const char * argv[])
{
    Mat img = imread("/Users/Derek/Desktop/apple.jpg"); //Change the image path here.
    if (img.data == 0) {
        cerr << "Image not found!" << endl;
        return -1;
    }
    namedWindow("image", CV_WINDOW_AUTOSIZE);
    imshow("image", img);
    waitKey();
}