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
const char * YesVision::WINDOW_NAME = "YesVision";
const char * YesVision::PATH_TO_XML = "/Users/Derek/Desktop/InternStuff/facedetection/2013OpenCV/opencv/data/haarcascades/haarcascade_frontalface_default.xml";
const char * YesVision::PATH_TO_AVI = "/Users/Derek/Desktop/InternStuff/facedetection/2013OpenCV/test.avi";
const char * YesVision::SLIDER_TEXT = "Position";

CvCapture * cap = NULL;

YesVision::YesVision() {
    errCode = visionInit();
    if (errCode == INIT_FAIL) exit(-1);
}

YesVision::~YesVision() {
    cvReleaseCapture(&captureSrc);
    cvDestroyWindow(WINDOW_NAME);
    if (cascade) cvReleaseHaarClassifierCascade(&cascade);
    if (pStorage) cvReleaseMemStorage(&pStorage);
}


/**
  * This defines what will happen when the trackbar is moved. Used for seeking.
  */
void YesVision::onTrackbarSlide(int pos) {
    cvSetCaptureProperty(cap, CV_CAP_PROP_POS_FRAMES, pos);
}

/**
 * Initializes the capture source. This will be a .avi file for now.
 * Also loads the desire xml file for the HaarClassifier and sets aside memory for CV.
 * @return error code or success
 */
int YesVision::visionInit() {
    int retval = CAP_SRC_OK;
   

    /*
    // Initialize the camera capture.
    captureSrc = cvCaptureFromCAM(CV_CAP_ANY); // CV_CAP_ANY means any camera.
    */
    

    captureSrc = cvCaptureFromFile(PATH_TO_AVI);

    // Load the classifier xml and allocate memory.
    cascade = (CvHaarClassifierCascade *)cvLoad(PATH_TO_XML, 0, 0, 0);    
    pStorage = cvCreateMemStorage(0);

    // Check if classifier was successfully loaded.
    if (cascade == NULL) { 
        fprintf(stderr, "ERROR: Cascade is NULL !!\n");
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

    // Set the initial of the slider to 0.
    sliderPos = 0;

    cap = captureSrc;

    return retval;

}

/**
 * Creates a window and displays input from the camera source.
 */
void YesVision::detectFaces() {
    cvNamedWindow(WINDOW_NAME, CV_WINDOW_AUTOSIZE);
    
    // Add the trackbar to this window.
    int numFrames = (int)cvGetCaptureProperty(captureSrc, CV_CAP_PROP_FRAME_COUNT);
    if (numFrames != 0) {
        cvCreateTrackbar(SLIDER_TEXT, WINDOW_NAME, &sliderPos, numFrames, onTrackbarSlide);
    }
    
    while (1) {
        // Grab a frame
        IplImage * origFrame = cvQueryFrame(captureSrc);
        // Check if the frame is NULL.
        if (origFrame == NULL) {
            fprintf(stderr, "ERROR: NULL frame !! Possible EOF.\n");
            break;
        }

        // Create a copy of the grabbed image frame.
        IplImage * frame = cvCloneImage(origFrame);

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
        cvShowImage(WINDOW_NAME, frame);
        cvReleaseImage(&frame);

        // Update where the slider is.
        cvSetTrackbarPos(SLIDER_TEXT, WINDOW_NAME, ++sliderPos);

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
