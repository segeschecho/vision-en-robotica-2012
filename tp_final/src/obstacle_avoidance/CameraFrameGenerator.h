//
//  CameraFrameGenerator.h
//

#ifndef _CameraFrameGenerator_h
#define _CameraFrameGenerator_h

#include "IFrameGenerator.h"

class Camera;

class CameraFrameGenerator : public IFrameGenerator {
public:
    CameraFrameGenerator(const char *deviceName, int width=640, int height=480, int fps=15, int exposure=400);
    virtual ~CameraFrameGenerator();
    
    virtual bool init();
    virtual void release();
    virtual bool getNextFrame(cv::Mat& frame);

    virtual int getFrameWidth();
    virtual int getFrameHeight();

private:
    
    Camera*     _camera;

    char        _deviceName[64];
    int         _width;
    int         _height;
    int         _fps;
    int         _exposure;
    IplImage*   _frame;
};

#endif