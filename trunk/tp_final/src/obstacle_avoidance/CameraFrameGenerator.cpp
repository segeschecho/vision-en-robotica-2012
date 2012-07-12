#include "CameraFrameGenerator.h"
#include "stdio.h"
#include "../library/libcam/libcam.h"

CameraFrameGenerator::CameraFrameGenerator(const char *deviceName, int width, int height, int fps, int exposure) {
    strcpy(_deviceName, deviceName);
    _width = width;
    _height = height;
    _fps = fps;
    _exposure = exposure;
    _camera = NULL;
    _frame = NULL;
}

CameraFrameGenerator::~CameraFrameGenerator() {
    
}

bool CameraFrameGenerator::init() {
    _camera = new Camera(_deviceName, _width, _height, _fps);
    _camera->setExposure(_exposure);
    _camera->Update();
    
    _frame = cvCreateImage(cvSize(_width, _height), IPL_DEPTH_8U, 3);
    
    return true;
}

void CameraFrameGenerator::release() {
    delete _camera;
    _camera = NULL;
    cvReleaseImage(&_frame);
}

bool CameraFrameGenerator::getNextFrame(cv::Mat& frame) {
    _camera->Update((unsigned int)1);
    
    _camera->toIplImage(_frame);
    frame = cv::Mat(_frame);
    
    return true;
}

int CameraFrameGenerator::getFrameWidth() {
    if(_camera == NULL) {
        return -1;
    }
    
    return _camera->width;
}

int CameraFrameGenerator::getFrameHeight() {
    if(_camera == NULL) {
        return -1;
    }
    
    return _camera->height;
}
