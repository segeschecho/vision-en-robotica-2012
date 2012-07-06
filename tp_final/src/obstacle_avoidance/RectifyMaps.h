//
//  RectifyMaps.h
//

#ifndef TP3_VisRob_RectifyMaps_h
#define TP3_VisRob_RectifyMaps_h

#include <opencv/cv.h>

class RectifyMaps {
  
public:
  RectifyMaps() : left_map1(), left_map2(), right_map1(), right_map2() {};
  ~RectifyMaps(){};
  
  //private:
  cv::Mat left_map1;
  cv::Mat left_map2;
  cv::Mat right_map1;
  cv::Mat right_map2;
  
};

#endif
