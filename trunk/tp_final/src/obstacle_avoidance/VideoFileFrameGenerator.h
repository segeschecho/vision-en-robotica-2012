//
//  VideoFileFrameGenerator.h
//

#ifndef _VideoFileFrameGenerator_h
#define _VideoFileFrameGenerator_h

#include "IFrameGenerator.h"

#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

class VideoFileFrameGenerator : public IFrameGenerator {
public:
    VideoFileFrameGenerator(const std::string& fileName);
    virtual ~VideoFileFrameGenerator();
    
    virtual bool init();
    virtual void release();
    virtual bool getNextFrame(cv::Mat& frame);

    virtual int getFrameWidth();
    virtual int getFrameHeight();

private:
    const std::string   _fileName;    
    cv::VideoCapture    _videoCapture;
};

#endif