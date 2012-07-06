#include <iostream>
#include <fstream>
#include <opencv/highgui.h>
#include "libelas/src/elas.h"
#include "CameraCalibration.h"
#include "RectifyMaps.h"
#include "GlobalConfig.h"

/// Global Variables
int trackbar_max;
int alpha_slider;
double alpha_track_bar;
double beta_track_bar;

cv::Mat undistorted_left_im, undistorted_right_im, dst, undistorted_right_im_moved;

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
  
  cv::imshow(windowName, disparity_data_color);
}

void computeDisparity(cv::Mat& aligned_left_frame, cv::Mat& aligned_right_frame, cv::Mat_<float>& disparity_left_frame, cv::Mat_<float>& disparity_right_frame){

  Elas::parameters param;
  param.postprocess_only_left = false;
  param.ipol_gap_width = 30; //parametro para interpolar pixels y evitar las zonas negras
  Elas elas(param);

  cv::Mat_<uchar> im1_out_gray, im2_out_gray;

  //aligned_left_frame.copyTo(im1_out_gray);
  //aligned_right_frame.copyTo(im2_out_gray);
  
  cv::cvtColor(aligned_left_frame, im1_out_gray, CV_BGR2GRAY);
  cv::cvtColor(aligned_right_frame, im2_out_gray, CV_BGR2GRAY);

  // get image width and height
  int32_t width  = im1_out_gray.size().width;
  int32_t height = im1_out_gray.size().height;

  // allocate memory for disparity images
  const int32_t dims[3] = {width,height,width}; // bytes per line = width
  disparity_left_frame = cv::Mat_<float>(im1_out_gray.size());
  disparity_right_frame = cv::Mat_<float>(im2_out_gray.size());

  // process
  elas.process(im1_out_gray.data, im2_out_gray.data, (float*)disparity_left_frame.data, (float*)disparity_right_frame.data, dims);
 
}

/**
 * @function on_trackbar
 * @brief Callback for trackbar
 */
void translateFrame(int alpha_disparity, cv::Mat& frame, cv::Mat& translated_frame){
  
  int width = frame.size().width;
  int height = frame.size().height;

  int x_prima, d_prima;
  
  translated_frame = cv::Mat(frame.size(), frame.type());
 
  for(int y = 0; y<height; ++y){
    for(int x = 0; x<width; ++x){
      if (alpha_disparity < 0) {
        if (x < width + alpha_disparity) {
          d_prima = (width + alpha_disparity - x);
          x_prima = width - d_prima;
          translated_frame.at<uchar>(y,x) = frame.at<uchar>(y,x_prima);
        }
        else {
          translated_frame.at<uchar>(y,x) = 0;
        }
      }
      else {
        if (x + alpha_disparity < width){
          x_prima = alpha_disparity +x;
          translated_frame.at<uchar>(y,x_prima) = frame.at<uchar>(y,x);
        }
        if (x < alpha_disparity){
          translated_frame.at<uchar>(y,x) = 0;
        }        
      }
    }
  }  
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


void initRectifyMaps(CameraCalibration& left_calib, CameraCalibration& right_calib, cv::Size& left_size, cv::Size& right_size, RectifyMaps& out_rectify_maps)
{
  /* computes the rotation matrices for each camera that (virtually) make both camera image planes the same plane */
  cv::Mat left_r_prima, right_r_prima, left_k_prima, right_k_prima, q;
  double alpha = 0;
  cv::Size new_left_image_size = left_size;
  cv::Size new_right_image_size = right_size;
  cv::Rect left_roi, right_roi;
  
  cv::stereoRectify(left_calib.k, left_calib.dist, right_calib.k, right_calib.dist, left_size, right_calib.r, right_calib.t, left_r_prima, right_r_prima, left_k_prima, right_k_prima, q, alpha, cv::CALIB_ZERO_DISPARITY, new_left_image_size, &left_roi , &right_roi );
  

  /* Computes the undistortion and rectification transformation map for each camera*/
  cv::initUndistortRectifyMap(left_calib.k, left_calib.dist, left_r_prima, left_k_prima, new_left_image_size, CV_32FC1, out_rectify_maps.left_map1, out_rectify_maps.left_map2);
  cv::initUndistortRectifyMap(right_calib.k, right_calib.dist, right_r_prima, right_k_prima, new_right_image_size, CV_32FC1, out_rectify_maps.right_map1, out_rectify_maps.right_map2);
}

void applyMaps(cv::Mat& left_image, cv::Mat& right_image, RectifyMaps& rectify_maps, cv::Mat& out_left_image, cv::Mat& out_right_image)
{
  cv::remap(left_image, out_left_image, rectify_maps.left_map1, rectify_maps.left_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
  cv::remap(right_image, out_right_image, rectify_maps.right_map1, rectify_maps.right_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
}



void readStereoCalibration(const char* parameters_xml, CameraCalibration& leftCalib, CameraCalibration& rightCalib, int& alpha_disparity)
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
  fs["ALPHA_DISPARITY"] >> alpha_disparity;

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
  
  GlobalConfig& global_config = GlobalConfig::getInstance();
  
  CameraCalibration& left_calib = global_config.getCameraCalibrationLeft();
  CameraCalibration& right_calib = global_config.getCameraCalibrationRight();
  RectifyMaps& rectify_maps = global_config.getRectifyMaps();
  int alpha_disparity;

  readStereoCalibration(parameters_xml, left_calib, right_calib, alpha_disparity);
  

  global_config.setDisparityInfinite(alpha_disparity);

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
 
  cv::Size size_left = cv::Size(video_left.get(CV_CAP_PROP_FRAME_WIDTH), video_left.get(CV_CAP_PROP_FRAME_HEIGHT));  
  cv::Size size_right = cv::Size(video_right.get(CV_CAP_PROP_FRAME_WIDTH), video_right.get(CV_CAP_PROP_FRAME_HEIGHT));

 
  initRectifyMaps(left_calib, right_calib, size_left, size_right, rectify_maps);

  cv::namedWindow("Disparity Left Camera", 1);
  cv::namedWindow("Disparity Right Camera", 1);
  cv::namedWindow("Original Left Camera", 1);
  cv::namedWindow("Original Right Camera", 1);

  cv::Mat frame_left, frame_right, rectify_left_frame, rectify_right_frame, aligned_left_frame, aligned_right_frame;
  cv::Mat_<float> disparity_left_frame, disparity_right_frame;

  video_left >> frame_left; // get a new frame from camera
  video_right >> frame_right; // get a new frame from camera

  while(!frame_left.empty() && !frame_right.empty()) {

    
    
    applyMaps(frame_left, frame_right, rectify_maps, rectify_left_frame, rectify_right_frame);

    translateFrame(alpha_disparity, rectify_left_frame, aligned_left_frame);
    translateFrame(alpha_disparity, rectify_right_frame, aligned_right_frame);

   
    computeDisparity(aligned_left_frame, aligned_right_frame, disparity_left_frame, disparity_right_frame);
    
    printHSV(disparity_left_frame, "Disparity Right Camera");
    printHSV(disparity_right_frame, "Disparity Left Camera");

    cv::imshow("Original Left Camera", frame_left);
    cv::imshow("Original Right Camera", frame_right);

    cv::waitKey(30);

    video_left >> frame_left; // get a new frame from camera
    video_right >> frame_right; // get a new frame from camera
  }

  return 0;
}
