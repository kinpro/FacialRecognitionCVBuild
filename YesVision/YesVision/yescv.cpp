/**
 *  yescv.cpp
 *  OpenCV Test
 *
 *  Created by Derek Quach on 5/21/13.
 *  Copyright (c) 2013 Derek Quach. All rights reserved.
 *
 *  OpenCV Test is a familiarization with the OpenCV libraries and a rudimentary
 *  implementation of facial recognition in video.
 */
#include "yescv.h"

// Static string declarations
const char * YesVision::windowName = "YesVision";
const char * YesVision::pathToXML = "/Users/Derek/Desktop/InternStuff/facedetection/2013OpenCV/opencv/data/haarcascades/haarcascade_frontalface_default.xml";


YesVision::YesVision() {
    errCode = visionInit();
    if (errCode == INIT_FAIL) exit(-1);
}

YesVision::~YesVision() {
    cvReleaseCapture(&captureSrc);
    cvDestroyWindow(windowName);
    if (cascade) cvReleaseHaarClassifierCascade(&cascade);
    if (pStorage) cvReleaseMemStorage(&pStorage);
}

/**
 * Initializes the capture source. This will be the webcam for now.
 * Also loads the desire xml file for the HaarClassifier and sets aside memory for CV.
 * @return error code or success
 */
int YesVision::visionInit() {
    int retval = CAP_SRC_OK;
    
    // Initialize the camera capture.
    captureSrc = cvCaptureFromCAM(CV_CAP_ANY); // CV_CAP_ANY means any camera.

    // Load the classifier xml and allocate memory.
    cascade = (CvHaarClassifierCascade *)cvLoad(pathToXML, 0, 0, 0);    
    pStorage = cvCreateMemStorage(0);

    // Check if classifier was successfully loaded.
    if (cascade == NULL) { 
        fprintf(stderr, "ERROR: Casacade is NULL !!\n");
        retval = INIT_FAIL;
    }
   
    // Check if memory allocation worked.
    if (pStorage == NULL) { 
        fprintf(stderr, "ERROR: Storage is NULL !!\n");
        retval = INIT_FAIL;
    }

    // Check if the capture is null (no camera on board).
    if (captureSrc == NULL) {
        fprintf(stderr, "ERROR: Capture is NULL !!\n");
        retval = INIT_FAIL;
    }
    return retval;

}

/**
 * Creates a window and displays input from the camera source.
 */
void YesVision::detectFaces() {
    cvNamedWindow(windowName, CV_WINDOW_AUTOSIZE);
    while (1) {
        // Grab a frame
        IplImage * frame = cvQueryFrame(captureSrc);
        // Check if the frame is NULL.
        if (frame == NULL) {
            fprintf(stderr, "ERROR: NULL frame found !!\n");
            break;
        }

        // Detect faces in the frame.
        faceRects = cvHaarDetectObjects(frame, cascade, pStorage,
                                        SEARCH_SCALE_INC_RATE,
                                        MIN_NEIGHBORS,
                                        CV_HAAR_DO_CANNY_PRUNING,
                                        cvSize(0,0));

        // Add in the overlay to display detected faces.
        for (int i = 0; i < faceRects->total; i++) {
            CvRect * rect = (CvRect *)cvGetSeqElem(faceRects, i );
            CvPoint p1 = {rect->x, rect->y};
            CvPoint p2 = {rect->x + rect->width, rect->y + rect->height};
            cvRectangle(frame, p1, p2, CV_RGB(0, 255, 0), 3, 4, 0);
        }

        // Display the image onto the frame.
        cvShowImage(windowName, frame);
        
        // ESC Key Code should be 0x10001B.
        // Break out of the loop if the ESC key is hit.
        if ((cvWaitKey(10) & 255) == ESC_KEY_CODE) break;
    }
}




int main() {
    YesVision * vision = new YesVision();
    vision->detectFaces();
    delete vision;
    return 0;
}
