//
//  IFrameGenerator.h
//

#ifndef _IFrameGenerator_h
#define _IFrameGenerator_h
#include <opencv/cv.h>

class IFrameGenerator {
public:
    virtual ~IFrameGenerator(){};
    
    virtual bool init() = 0;
    virtual void release() = 0;
    virtual bool getNextFrame(cv::Mat& frame) = 0;
    
    virtual int getFrameWidth() = 0;
    virtual int getFrameHeight() = 0;
};

#endif