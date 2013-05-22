/**
  *  yescv.h
  *  OpenCV Test
  *
  *  Created by Derek Quach on 5/21/13.
  *  Copyright (c) 2013 Derek Quach. All rights reserved.
  *
  *  OpenCV Test is a familiarization with the OpenCV libraries and a rudimentary
  *  implementation of facial recognition in video.
  */


#ifndef _YESCV_H_
#define _YESCV_H_


#include "cv.h"
#include "highgui.h"
#include <stdio.h>


#define CAP_SRC_OK            1
#define INIT_FAIL             0
#define ESC_KEY_CODE          27
#define MIN_NEIGHBORS         6
#define SEARCH_SCALE_INC_RATE 1.1

using namespace std;
using namespace cv;

class YesVision {
    public:
        YesVision();
        ~YesVision();

        int  visionInit();
        void detectFaces();

    private:
        CvCapture *               captureSrc;
        CvHaarClassifierCascade * cascade;
        CvMemStorage *            pStorage;
        CvSeq *                   faceRects;

        static const char *       windowName;
        static const char *       pathToXML;
        int                       errCode;        
};
#endif //_YESCV_H_







