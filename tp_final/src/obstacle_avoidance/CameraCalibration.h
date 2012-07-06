//
//  CameraCalibration.h
//

#ifndef TP3_VisRob_CameraCalibration_h
#define TP3_VisRob_CameraCalibration_h

#include <opencv/cv.h>

class CameraCalibration {
  
public:
  CameraCalibration() : k(), dist(), r(), t() {};
  ~CameraCalibration(){};
  
  //private:
  cv::Mat k;
  cv::Mat dist;
  cv::Mat r;
  cv::Mat t;
};

#endif
