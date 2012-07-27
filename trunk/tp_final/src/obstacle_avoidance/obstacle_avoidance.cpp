#include <iostream>
#include <fstream>
#include <math.h>
#include <signal.h>
#include <opencv/cv.h>
#include "../library/libelas/src/elas.h"
#include "../library/libexabot-remote/libexabot-remote.h"
#include "CameraCalibration.h"
#include "RectifyMaps.h"
#include "GlobalConfig.h"
#include "VideoFileFrameGenerator.h"
#include "CameraFrameGenerator.h"

#define PI 3.14159265f

#define HIGH_DISPARITY_THRESHOLD  75
#define PERCENTAGE_FIRST_ZONE     0.25f
#define PERCENTAGE_SECOND_ZONE    0.6f
#define NEGATIVE_DISPARITY_WEIGHT 0.3f   //

#define Y_DELTA                   0.09f

#define TEST
#define EXABOT_SEND_COMMANDS

struct Vector2D {
  float x;
  float y;
};

/// Global Variables
int trackbar_max;
int alpha_slider;
double alpha_track_bar;
double beta_track_bar;
bool end = false;

#ifdef TEST
cv::Mat_<uchar> pixels_high_disparity;
#endif

cv::Mat undistorted_left_im, undistorted_right_im, dst, undistorted_right_im_moved;

// Configuration
int g_old_alpha_disp_trackbar;
int g_disparity_rows_treshold = 120;
float g_exabot_speed = 0.0f;
float g_high_average_threshold = 0.3;

void onTrackbarAlphaDisparity(int trackbar_position, void*) {
  int newDisparityInfinite = GlobalConfig::getInstance().getDisparityInfinite() + g_old_alpha_disp_trackbar - trackbar_position;

  GlobalConfig::getInstance().setDisparityInfinite(newDisparityInfinite);
  g_old_alpha_disp_trackbar = trackbar_position;
}

void onTrackbarDisparityIgnoreRowsPercentage(int trackbar_position, void*) {
}

void onTrackbarExabotSpeed(int trackbar_position, void*) {
  g_exabot_speed = (float)trackbar_position / 10.0f;
}

void onTrackbarAverageThreshold(int trackbar_position, void*) {
  g_high_average_threshold = (float)trackbar_position / 100.0f;
}

// for libexabot
void interrupt_signal(int s) {
  end = true;
}

int sgn(double x){
  return x < 0 ? -1 : 1;
}

void translateVectorToExabotVelocity(const Vector2D& movementVector, Vector2D& result) {
  if (movementVector.x == 0.0f && movementVector.y == 0.0f) {
    result.x = 0.0f;
    result.y = 0.0f;

    return;
  }
  
  result.x = 1.0f;
  result.y = 1.0f;
  
  float tanAngle = movementVector.y / movementVector.x;
  double angle = atan(tanAngle);
  
  if (movementVector.x < 0)
  {
    result.x = (float)(angle / (PI/2));
  }
  else if(movementVector.x > 0)
  {
    result.y = (float)(angle / (PI/2));
  }
  
  if(fabs(movementVector.y) < Y_DELTA)
  {
    // Must turn staying in the same place
    if (movementVector.x < 0)
    {
      result.x = -result.y;
    }
    else if(movementVector.x > 0)
    {
      result.y = -result.x;
    }
  }
  else
  {
    if (movementVector.y < 0)
    {
      result.x = -fabs(result.x);
      result.y = -fabs(result.y);
    }
    else if (movementVector.y > 0)
    {
      result.x = fabs(result.x);
      result.y = fabs(result.y);
    }
  }
  
  float vectorLength = (float)sqrt(movementVector.x * movementVector.x + movementVector.y * movementVector.y);
  
  result.x *= vectorLength;
  result.y *= vectorLength;
}




float compute_average_disparity(cv::Mat_<float>& disparity_frame, int begin_zone_pixels, int end_zone_pixels) {
  int high_disparity_counter     = 0;
  int negative_disparity_counter = 0; //black zone in disparity map
  float average                  = 0;
  int image_rows                 = disparity_frame.rows - g_disparity_rows_treshold;
  int total_pixels               = image_rows * (end_zone_pixels - begin_zone_pixels);

  // counting high disparity pixels
  for (int row = 0; row < image_rows; ++row) {
    for (int col = begin_zone_pixels; col < end_zone_pixels; ++col) {
      int pixel_disparity = disparity_frame.at<float>(row, col);
      
      if(pixel_disparity < 0){
        negative_disparity_counter++;
#ifdef TEST
        pixels_high_disparity.at<uchar>(row, col) = 60;
#endif
      }
      else if (pixel_disparity >= HIGH_DISPARITY_THRESHOLD) {
        high_disparity_counter++;
#ifdef TEST
        pixels_high_disparity.at<uchar>(row, col) = 255;
#endif
      }
    }
  }
  
#ifdef TEST
  cv::imshow("Pixels detected as High Disparity", pixels_high_disparity);
#endif
  // calculate average
  average =  NEGATIVE_DISPARITY_WEIGHT * (float)negative_disparity_counter / (float)total_pixels;
  average += (float)high_disparity_counter / (float)total_pixels;
  return average;
}


float computeDirectionFromDisparity(cv::Mat_<float>& disparity_frame_left, cv::Mat_<float>& disparity_frame_right) {
  float direction = 0;
  float disparity_center_percentage = 0;
  float disparity_left_percentage = 0;
  float disparity_right_percentage = 0;
  
  // calculate percentage of high disparity in the center of the image
  int begin_center_zone = PERCENTAGE_FIRST_ZONE * disparity_frame_left.cols;
  int end_center_zone = begin_center_zone + PERCENTAGE_SECOND_ZONE * disparity_frame_left.cols;

#ifdef TEST
  pixels_high_disparity = cv::Mat::zeros(disparity_frame_left.rows, disparity_frame_left.cols, CV_8UC1);
#endif

  disparity_center_percentage = compute_average_disparity(disparity_frame_left, begin_center_zone, end_center_zone);
  
  std::cout << "disparity_center_percentage " << disparity_center_percentage << std::endl;
  
  if (disparity_center_percentage >= g_high_average_threshold) {
    // compute average disparity for frame's left and right sides
    disparity_left_percentage = compute_average_disparity(disparity_frame_left, 0, begin_center_zone);
//    disparity_right_percentage = compute_average_disparity(disparity_frame_right, end_center_zone, disparity_frame_right.cols);
    disparity_right_percentage = compute_average_disparity(disparity_frame_left, end_center_zone, disparity_frame_left.cols);
    
    if (disparity_left_percentage < disparity_right_percentage) {
      direction = PI/2;
    } else {
      direction = -PI/2;
    }
  }
  
  return direction;
}


void printHSV(cv::Mat_<float>& disparityData, const char* windowName) {
  cv::Mat_<cv::Vec3b> disparity_data_color(disparityData.size());
  for (unsigned int j = 0; j < (unsigned int)disparityData.cols; j++) {
    for (unsigned int i = 0; i < (unsigned int)disparityData.rows; i++) {    
      cv::Vec3b v;
      
      float val = std::min(disparityData.at<float>(i,j) * 0.01f, 1.0f);
      if (val < 0) {
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

void computeDisparity(cv::Mat& rectify_left_frame, cv::Mat& aligned_right_frame, cv::Mat_<float>& disparity_left_frame, cv::Mat_<float>& disparity_right_frame){

  Elas::parameters param;
  param.postprocess_only_left = false;
  param.ipol_gap_width = 30; //parametro para interpolar pixels y evitar las zonas negras
  Elas elas(param);

  // get image width and height
  int32_t width  = rectify_left_frame.size().width;
  int32_t height = rectify_left_frame.size().height;

  // allocate memory for disparity images
  const int32_t dims[3] = {width,height,width}; // bytes per line = width
  disparity_left_frame = cv::Mat_<float>(rectify_left_frame.size());
  disparity_right_frame = cv::Mat_<float>(aligned_right_frame.size());

  // process
  elas.process(rectify_left_frame.data, aligned_right_frame.data, (float*)disparity_left_frame.data, (float*)disparity_right_frame.data, dims); 
}

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



bool readStereoCalibration(const char* parameters_xml, CameraCalibration& leftCalib, CameraCalibration& rightCalib)
{  
  /*read the xml file */
  cv::FileStorage fs(parameters_xml, cv::FileStorage::READ);
  
  int alpha_disparity;
  
  std::cout << "Se abre el archivo xml." << std::endl << std::endl;
  
  if (!fs.isOpened()){
    return false;
  }
  
  fs["P1"] >> leftCalib.k;
  fs["P2"] >> rightCalib.k;
  fs["dist1"] >> leftCalib.dist;
  fs["dist2"] >> rightCalib.dist;
  fs["R"] >> rightCalib.r;
  fs["T"] >> rightCalib.t;
  fs["ALPHA_DISPARITY"] >> alpha_disparity;

  GlobalConfig::getInstance().setDisparityInfinite(alpha_disparity);
  leftCalib.r = cv::Mat::eye(rightCalib.r.rows, rightCalib.r.cols, rightCalib.r.type());
  leftCalib.t = cv::Mat::zeros(rightCalib.r.rows, rightCalib.r.cols, rightCalib.r.type());

  fs.release();

  return true;
}

void openConfigurationWindow(int frame_width, int frame_height) {
  cv::namedWindow("Configuration", 1);

  // Alpha disparity
  cv::createTrackbar("Alpha disparity", "Configuration", NULL, frame_width*2, onTrackbarAlphaDisparity);

  g_old_alpha_disp_trackbar = frame_width + GlobalConfig::getInstance().getDisparityInfinite();
  std::cout << g_old_alpha_disp_trackbar << std::endl;
  cv::setTrackbarPos("Alpha disparity", "Configuration", g_old_alpha_disp_trackbar);

  // Disparity Ignore Rows
  cv::createTrackbar("Disparity Rows Limit", "Configuration", &g_disparity_rows_treshold, frame_height, onTrackbarDisparityIgnoreRowsPercentage);

  cv::setTrackbarPos("Disparity Rows Limit", "Configuration", g_disparity_rows_treshold);

  // Exabot Speed
  cv::createTrackbar("Exabot Speed", "Configuration", NULL, 10, onTrackbarExabotSpeed);

  cv::setTrackbarPos("Exabot Speed", "Configuration", g_exabot_speed * 10);
  
  // Average of disparity pixels
  cv::createTrackbar("Average of disparity pixels", "Configuration", NULL, 100, onTrackbarAverageThreshold);

  cv::setTrackbarPos("Average of disparity pixels", "Configuration", (int)(g_high_average_threshold * 100.0f));
}

int main(int argc, char *argv[])
{
  if (argc < 4 || argc > 5)
  {
    std::cout << "Arguments must be passing in this way: " << argv[0] << " <Parameters File> <Left Frames Source> <Right Frames Source> [Source Type]" << std::endl;
    std::cout << std::endl;
    std::cout << "Parameters File: Path to parameters.xml file." << std::endl;
    std::cout << "Left Frames Source: Path to left video format file or left video device (eg. /dev/video1)" << std::endl;
    std::cout << "Right Frame Source: Path to right video format file or right video device (eg. /dev/video2)" << std::endl;
    std::cout << "Source Type [OPTIONAL]:" << std::endl;
    std::cout << "   vid: video source type. Left frames source and right source frame are loaded from video format file." << std::endl;
    std::cout << "   cam: video source type. Left frames source and right source frame are loaded from camera device." << std::endl;
    
    return -1;
  }

  const char* parameters_xml = argv[1];
  const char* left_frame_sourcename = argv[2];
  const char* right_frame_sourcename = argv[3];
  const char* frame_source_type = "";
  
  if(argc == 5) {
    frame_source_type = argv[4];
  }
  /*read the xml file */
  
  GlobalConfig& global_config = GlobalConfig::getInstance();
  
  CameraCalibration& left_calib = global_config.getCameraCalibrationLeft();
  CameraCalibration& right_calib = global_config.getCameraCalibrationRight();
  RectifyMaps& rectify_maps = global_config.getRectifyMaps();

  bool parametersParsed = readStereoCalibration(parameters_xml, left_calib, right_calib);
  
  if(!parametersParsed) {
    std::cerr << "ERROR: No se pudo abrir el archivo xml." << std::endl;
    return -1;
  }

  IFrameGenerator* frame_generator_left = NULL;
  IFrameGenerator* frame_generator_right = NULL;

  if (strcmp(frame_source_type, "cam") == 0) {
    frame_generator_left = new CameraFrameGenerator(left_frame_sourcename);
    frame_generator_right = new CameraFrameGenerator(right_frame_sourcename);
  } else {
    frame_generator_left = new VideoFileFrameGenerator(left_frame_sourcename);
    frame_generator_right = new VideoFileFrameGenerator(right_frame_sourcename);
  }
  
  if (!frame_generator_left->init()){  // check if we succeeded
    std::cerr << "No se pudo iniciar el generador de frames a partir de " << left_frame_sourcename << std::endl;
    return -1;
  }

  if (!frame_generator_right->init()){  // check if we succeeded
    std::cerr << "No se pudo iniciar el generador de frames a partir de " << right_frame_sourcename << std::endl;
    return -1;
  }
 
  cv::Size size_left = cv::Size(frame_generator_left->getFrameWidth(), frame_generator_left->getFrameHeight());  
  cv::Size size_right = cv::Size(frame_generator_right->getFrameWidth(), frame_generator_right->getFrameHeight());

  initRectifyMaps(left_calib, right_calib, size_left, size_right, rectify_maps);

  cv::namedWindow("Disparity Left Camera", 1);
//  cv::namedWindow("Disparity Right Camera", 1);
  cv::namedWindow("Original Left Camera", 1);
//  cv::namedWindow("Original Right Camera", 1);

#ifdef TEST
  cv::namedWindow("Pixels detected as High Disparity", 1);
#endif

  openConfigurationWindow(frame_generator_left->getFrameWidth(), frame_generator_left->getFrameHeight());

  cv::Mat frame_left, frame_right, rectify_left_frame, rectify_right_frame, aligned_right_frame;
  cv::Mat_<float> disparity_left_frame, disparity_right_frame;
  cv::Mat_<uchar> frame_left_gray, frame_right_gray;

//  video_left >> frame_left; // get a new frame from camera
//  video_right >> frame_right; // get a new frame from camera
  
#ifdef EXABOT_SEND_COMMANDS
  // initializate libexabot
  exa_remote_initialize("192.168.1.2");
#endif

  signal(SIGINT, &interrupt_signal);
  
//  while(!frame_left.empty() && !frame_right.empty() && !end) {

  bool has_next_frame_left = frame_generator_left->getNextFrame(frame_left);     // get a new frame from left camera
  bool has_next_frame_right = frame_generator_right->getNextFrame(frame_right);  // get a new frame from right camera

  while(has_next_frame_left && has_next_frame_right && !end) {
    
    cv::cvtColor(frame_left, frame_left_gray, CV_RGB2GRAY);
    cv::cvtColor(frame_right, frame_right_gray, CV_RGB2GRAY);
    
    applyMaps(frame_left_gray, frame_right_gray, rectify_maps, rectify_left_frame, rectify_right_frame);

    translateFrame(global_config.getDisparityInfinite(), rectify_right_frame, aligned_right_frame);
   
    computeDisparity(rectify_left_frame, aligned_right_frame, disparity_left_frame, disparity_right_frame);
    
    printHSV(disparity_left_frame, "Disparity Left Camera");
//    printHSV(disparity_right_frame, "Disparity Right Camera");

    cv::line(frame_left, cv::Point(0, frame_left.rows - g_disparity_rows_treshold), cv::Point(frame_left.cols, frame_left.rows - g_disparity_rows_treshold), CV_RGB(0, 255, 0));
    cv::imshow("Original Left Camera", frame_left);
//    cv::imshow("Original Right Camera", frame_right);
    
    cv::waitKey(30);
    
        
    // obstacle avoidance strategy
    float angle = computeDirectionFromDisparity(disparity_left_frame, disparity_right_frame);
    Vector2D direction_from_disparity;
    Vector2D exabot_engine_speed;
    
    direction_from_disparity.y = sin(angle + PI/2) * g_exabot_speed;
    direction_from_disparity.x = cos(angle + PI/2) * g_exabot_speed;
    
    translateVectorToExabotVelocity(direction_from_disparity, exabot_engine_speed);
    
    std::cout << "DISPARITY_INFINITE: " << global_config.getDisparityInfinite() << " LEFT:  " << exabot_engine_speed.x << " RIGHT: " << exabot_engine_speed.y << std::endl;
    
#ifdef EXABOT_SEND_COMMANDS
//    if(sgn(exabot_engine_speed.x) != sgn(exabot_engine_speed.y)){
//      exa_remote_set_motors(sgn(exabot_engine_speed.x) * 0.2, sgn(exabot_engine_speed.y) * 0.2);
//    }
//    else{
      exa_remote_set_motors(exabot_engine_speed.x, exabot_engine_speed.y); 
//    }
#endif
    has_next_frame_left = frame_generator_left->getNextFrame(frame_left);     // get a new frame from left camera
    has_next_frame_right = frame_generator_right->getNextFrame(frame_right);  // get a new frame from right camera
  }

  frame_generator_left->release();
  frame_generator_right->release();
  
  delete frame_generator_left;
  delete frame_generator_right;
  
#ifdef EXABOT_SEND_COMMANDS
  // close exabot connection
  exa_remote_set_motors(0, 0);
  exa_remote_deinitialize();
#endif

  cv::destroyAllWindows();
    
  return 0;
}
