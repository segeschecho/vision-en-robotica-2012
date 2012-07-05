#include <iostream>
#include <vector>
#include <fstream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "libelas/src/elas.h"

/// Global Variables
int trackbar_max;
int alpha_slider;
double alpha_track_bar;
double beta_track_bar;

cv::Mat undistorted_left_im, undistorted_right_im, dst, undistorted_right_im_moved;

class CameraCalibration{

public:
  CameraCalibration() : k(), dist(), r(), t() {};
  ~CameraCalibration(){};

  //private:
  cv::Mat k;
  cv::Mat dist;
  cv::Mat r;
  cv::Mat t;

};

class RectifyMaps{

public:
  RectifyMaps() : left_map1(), left_map2(), right_map1(), right_map2() {};
  ~RectifyMaps(){};

  //private:
  cv::Mat left_map1;
  cv::Mat left_map2;
  cv::Mat right_map1;
  cv::Mat right_map2;

};


void printHSV(cv::Mat_<float>& disparityData, const char* windowName) {
  cv::Mat_<cv::Vec3b> disparity_data_color(disparityData.size());
  for (unsigned int j = 0; j < (unsigned int)disparityData.cols; j++) {
    for (unsigned int i = 0; i < (unsigned int)disparityData.rows; i++) {    
      cv::Vec3b v;
      
      float val = std::min(disparityData.at<float>(i,j) * 0.01f, 1.0f);
      if (val <= 0) {
        v[0] = v[1] = v[2] = 0;
      } else {
        float h2 = 6.0f * (1.0f - val);
        unsigned char x  = (unsigned char)((1.0f - fabs(fmod(h2, 2.0f) - 1.0f))*255);
        if (0 <= h2&&h2<1) { v[0] = 255; v[1] = x; v[2] = 0; }
        else if (1 <= h2 && h2 < 2)  { v[0] = x; v[1] = 255; v[2] = 0; }
        else if (2 <= h2 && h2 < 3)  { v[0] = 0; v[1] = 255; v[2] = x; }
        else if (3 <= h2 && h2 < 4)  { v[0] = 0; v[1] = x; v[2] = 255; }
        else if (4 <= h2 && h2 < 5)  { v[0] = x; v[1] = 0; v[2] = 255; }
        else if (5 <= h2 && h2 <= 6) { v[0] = 255; v[1] = 0; v[2] = x; }
      }
      
      disparity_data_color.at<cv::Vec3b>(i, j) = v;
    }
  }
  
  // Create Windows
  cv::namedWindow(windowName, 1);
  cv::imshow(windowName, disparity_data_color);
}

void showDisparity()
{
  Elas::parameters param;
  param.postprocess_only_left = false;
  param.ipol_gap_width = 30; //parametro para interpolar pixels y evitar las zonas negras
  Elas elas(param);

  cv::Mat_<uchar> im1_out_gray, im2_out_gray;
  
  cv::cvtColor(undistorted_left_im, im1_out_gray, CV_BGR2GRAY);
  cv::cvtColor(undistorted_right_im, im2_out_gray, CV_BGR2GRAY);

  // get image width and height
  int32_t width  = im1_out_gray.size().width;
  int32_t height = im1_out_gray.size().height;

  // allocate memory for disparity images
  const int32_t dims[3] = {width,height,width}; // bytes per line = width
  cv::Mat_<float> D1_data(im1_out_gray.size());
  cv::Mat_<float> D2_data(im1_out_gray.size());

  // process
  elas.process(im2_out_gray.data, im1_out_gray.data, (float*)D1_data.data, (float*)D2_data.data, dims);

  printHSV(D1_data, "Disparity Right Camera");
  printHSV(D2_data, "Disparity Left Camera");
}

/**
 * @function on_trackbar
 * @brief Callback for trackbar
 */
void on_trackbar(int, void*){
  int height = undistorted_right_im.size().height;
  int width = undistorted_right_im.size().width;
  double left_transparency = 0.5;
  double right_transparency = 0.5;
  int x_prima, d_prima;
  
  undistorted_right_im.copyTo(undistorted_right_im_moved);
  alpha_slider -= width;
  
  for(int y = 0; y<height; ++y){
    for(int x = 0; x<width; ++x){
      if (alpha_slider < 0)
      {
        if (x < width + alpha_slider)
        {
          d_prima = (width + alpha_slider - x);
          x_prima = width - d_prima;
          undistorted_right_im_moved.at<uchar>(y,x) = undistorted_right_im.at<uchar>(y,x_prima);
        }
        else
        {
          undistorted_right_im_moved.at<uchar>(y,x) = 0;
        }
      }
      else 
      {
        if (x + alpha_slider < width)
        {
          x_prima = alpha_slider +x;
          undistorted_right_im_moved.at<uchar>(y,x_prima) = undistorted_right_im.at<uchar>(y,x);
        }
        if (x < alpha_slider){
          undistorted_right_im_moved.at<uchar>(y,x) = 0;
        }
          
        }
      }
    }
  
  cv::addWeighted(undistorted_left_im, left_transparency, undistorted_right_im_moved, right_transparency, 0.0, dst);

  cv::imshow( "Linear Blend", dst);

//  showDisparity();
}

void listenToKeys() {
  int keyPressed;
  bool exit = false;

  while(!exit) {
    keyPressed = cv::waitKey(0);

    switch(keyPressed) {
    case ' ': //space
    case 1048608: //space (with numlock on)
      exit = true;
      break;
    }
  }
}


void initRectifyMaps(CameraCalibration& left_calib, CameraCalibration& right_calib, cv::Mat& left_im, cv::Mat& right_im, RectifyMaps& out_rectify_maps)
{
  /* computes the rotation matrices for each camera that (virtually) make both camera image planes the same plane */
  cv::Mat left_r_prima, right_r_prima, left_k_prima, right_k_prima, q;
  double alpha = 0;
  cv::Size new_left_image_size = left_im.size();
  cv::Size new_right_image_size = right_im.size();
  cv::Rect left_roi, right_roi;
  
  cv::stereoRectify(left_calib.k, left_calib.dist, right_calib.k, right_calib.dist, left_im.size(), right_calib.r, right_calib.t, left_r_prima, right_r_prima, left_k_prima, right_k_prima, q, alpha, cv::CALIB_ZERO_DISPARITY, new_left_image_size , &left_roi , &right_roi );
  

  /* Computes the undistortion and rectification transformation map for each camera*/
  cv::initUndistortRectifyMap(left_calib.k, left_calib.dist, left_r_prima, left_k_prima, new_left_image_size, CV_32FC1, out_rectify_maps.left_map1, out_rectify_maps.left_map2);
  cv::initUndistortRectifyMap(right_calib.k, right_calib.dist, right_r_prima, right_k_prima, new_right_image_size, CV_32FC1, out_rectify_maps.right_map1, out_rectify_maps.right_map2);
}

void applyMaps(cv::Mat& left_image, cv::Mat& right_image, RectifyMaps& rectify_maps, cv::Mat& out_left_image, cv::Mat& out_right_image)
{
  cv::remap(left_image, out_left_image, rectify_maps.left_map1, rectify_maps.left_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
  cv::remap(right_image, out_right_image, rectify_maps.right_map1, rectify_maps.right_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
}

int disparityAlignImages(CameraCalibration& left_calib, CameraCalibration& right_calib, cv::Mat& frame_left, cv::Mat& frame_right, RectifyMaps& rectify_maps)
{
  // Rectify the images
  cv::Mat undistorted_left_im_color, undistorted_right_im_color;
  applyMaps(frame_left, frame_right, rectify_maps, undistorted_left_im_color, undistorted_right_im_color);

  cv::cvtColor(undistorted_left_im_color, undistorted_left_im, CV_BGR2GRAY);
  cv::cvtColor(undistorted_right_im_color, undistorted_right_im, CV_BGR2GRAY);

  // Initialize values
  alpha_slider = -(undistorted_left_im.size().width-1);
  trackbar_max = 2 * (undistorted_left_im.size().width-1);
  undistorted_right_im_moved = cv::Mat(undistorted_right_im.size(),CV_8UC1);
  
  // Create Windows
  cv::namedWindow("Linear Blend", 1);
  
  // Create Trackbars
  char trackbarName[50];
  sprintf(trackbarName, "dst [0,%d]", trackbar_max);
  
  cv::createTrackbar(trackbarName, "Linear Blend", &alpha_slider, trackbar_max, on_trackbar);
  
  cv::Mat dst;

  // Show some stuff
  on_trackbar(alpha_slider, 0);

  listenToKeys();
  
  return alpha_slider;
}

void readStereoCalibration(const char* parameters_xml, CameraCalibration& leftCalib, CameraCalibration& rightCalib)
{  
  /*read the xml file */
  cv::FileStorage fs(parameters_xml, cv::FileStorage::READ);
  
  std::cout << "se abre el archivo xml" << std::endl << std::endl;
  
  if (!fs.isOpened()){
    std::cout << "no se pude abrir el archivo" << std::endl;
  }
  
  fs["P1"] >> leftCalib.k;
  fs["P2"] >> rightCalib.k;
  fs["dist1"] >> leftCalib.dist;
  fs["dist2"] >> rightCalib.dist;
  fs["R"] >> rightCalib.r;
  fs["T"] >> rightCalib.t;

  leftCalib.r = cv::Mat::eye(rightCalib.r.rows, rightCalib.r.cols, rightCalib.r.type());
  leftCalib.t = cv::Mat::zeros(rightCalib.r.rows, rightCalib.r.cols, rightCalib.r.type());

  fs.release();
}

int main(int argc, char *argv[])
{
  if (argc != 4)
  {
    std::cerr << "Arguments must be passing in this way: ./read_values /path/sequence/to/parameters_file.xml /path/sequence/to/video_left /path/sequence/to/video_right" << std::endl;
    return -1;
  }

  const char* parameters_xml = argv[1];
  const char* video_left_filename = argv[2];
  const char* video_right_filename = argv[3];

  /*read the xml file */
  
  CameraCalibration left_calib;
  CameraCalibration right_calib;
  
  readStereoCalibration(parameters_xml, left_calib, right_calib);


  /* Open left and right videos*/

  cv::VideoCapture video_left = cv::VideoCapture(); // open the default camera
  cv::VideoCapture video_right = cv::VideoCapture(); // open the default camera

  video_left.open(video_left_filename);
  video_right.open(video_right_filename);

  if (!video_left.isOpened()){  // check if we succeeded
    std::cerr << "No se pudo leer el video " << video_left_filename << std::endl;
    return -1;
  }

  if (!video_right.isOpened()){  // check if we succeeded
    std::cerr << "No se pudo leer el video " << video_right_filename << std::endl;
    return -1;
  }
 
  
  cv::Mat frame_left, frame_right;
  video_left >> frame_left; // get a new frame from camera
  video_right >> frame_right; // get a new frame from camera

  RectifyMaps rectify_maps;
  initRectifyMaps(left_calib, right_calib, frame_left, frame_right, rectify_maps);
  alpha_slider = disparityAlignImages(left_calib, right_calib, frame_left, frame_right, rectify_maps);

  /* Applies a generic geometrical transformation to an image */
  /*
  if(! mat_left_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the left image" << std::endl ;
    return -1;
  }
  
  if(! mat_right_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the right image" << std::endl ;
    return -1;
  }

  if(! undistorted_left_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the image" << std::endl ;
    return -1;
  }
  
  if(! undistorted_right_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the image" << std::endl ;
    return -1;
  }
  */

  return 0;
}
