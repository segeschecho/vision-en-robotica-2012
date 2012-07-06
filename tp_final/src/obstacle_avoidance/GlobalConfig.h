//
//  GlobalConfig.h
//

#ifndef TP3_VisRob_GlobalConfig_h
#define TP3_VisRob_GlobalConfig_h

#include "RectifyMaps.h"
#include "CameraCalibration.h"

class GlobalConfig {
public:
  static GlobalConfig& getInstance();

  ~GlobalConfig();
  
  RectifyMaps& getRectifyMaps();
  
  CameraCalibration& getCameraCalibrationLeft();
  
  CameraCalibration& getCameraCalibrationRight();

  int getDisparityInfinite();
  void setDisparityInfinite(int disparityInfinite);
  
private:
  GlobalConfig();

  // Singleton instance
  static GlobalConfig _instance;
  
  RectifyMaps         _rectifyMaps;
  CameraCalibration   _cameraCalibrationLeft;
  CameraCalibration   _cameraCalibrationRight;  
  int                 _disparityInfinite;
};

#endif
