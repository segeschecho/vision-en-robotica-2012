#include "VideoFileFrameGenerator.h"

VideoFileFrameGenerator::VideoFileFrameGenerator(const std::string& fileName) : _fileName(fileName), _videoCapture() {
}

VideoFileFrameGenerator::~VideoFileFrameGenerator() {
}
    
bool VideoFileFrameGenerator::init() {
    _videoCapture.open(_fileName);
    
    if (!_videoCapture.isOpened()) {
        return false;
    }
    
    return true;
}

void VideoFileFrameGenerator::release() {
    _videoCapture.release();
}

bool VideoFileFrameGenerator::getNextFrame(cv::Mat& frame) {
    return _videoCapture.read(frame);
}

int VideoFileFrameGenerator::getFrameWidth() {
    if (!_videoCapture.isOpened()) {
        return -1;
    }
    
    return _videoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
}

int VideoFileFrameGenerator::getFrameHeight() {
    if (!_videoCapture.isOpened()) {
        return -1;
    }
    
    return _videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);    
}
