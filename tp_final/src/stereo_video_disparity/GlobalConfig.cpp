//
//  GlobalConfig.cpp
//

#include "GlobalConfig.h"

GlobalConfig GlobalConfig::_instance = GlobalConfig();

///////////////////////////////////////////////////////
// Constructors and destructors
///////////////////////////////////////////////////////

GlobalConfig::GlobalConfig() : _rectifyMaps(), _cameraCalibrationLeft(), _cameraCalibrationRight(), _disparityInfinite() {
}

//-------------------------------------------------------

GlobalConfig::~GlobalConfig() {
  
}

//-------------------------------------------------------

GlobalConfig& GlobalConfig::getInstance() {
  return _instance;
}

//-------------------------------------------------------

RectifyMaps& GlobalConfig::getRectifyMaps() {
  return _rectifyMaps;
}

//-------------------------------------------------------

CameraCalibration& GlobalConfig::getCameraCalibrationLeft() {
  return _cameraCalibrationLeft;  
}

//-------------------------------------------------------

CameraCalibration& GlobalConfig::getCameraCalibrationRight() {
  return _cameraCalibrationRight;  
}

//-------------------------------------------------------

int GlobalConfig::getDisparityInfinite() {
  return _disparityInfinite;
}

//-------------------------------------------------------

void GlobalConfig::setDisparityInfinite(int disparityInfinite) {
  _disparityInfinite = disparityInfinite;
}

//-------------------------------------------------------
