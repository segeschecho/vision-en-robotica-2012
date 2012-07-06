#include <iostream>
#include <vector>
#include <fstream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "../library/libelas/src/elas.h"

/// Global Variables
int trackbar_max;
int alpha_slider;
double alpha_track_bar;
double beta_track_bar;

cv::Mat undistorted_left_im, undistorted_right_im, dst, undistorted_right_im_moved;

void printHSV(cv::Mat_<float>& disparityData, const char* windowName) {
  cv::Mat_<cv::Vec3b> disparity_data_color(disparityData.size());
  for (uint j = 0; j < (uint)disparityData.cols; j++) {
    for (uint i = 0; i < (uint)disparityData.rows; i++) {    
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

void showDisparity() {
  Elas::parameters param;
  param.postprocess_only_left = false;
  param.ipol_gap_width = 30; //parametro para interpolar pixels y evitar las zonas negras
  Elas elas(param);

  cv::Mat_<uchar> im1_out_gray, im2_out_gray;
  
  undistorted_left_im.copyTo(im2_out_gray);
  undistorted_right_im_moved.copyTo(im1_out_gray);
  
  //cv::cvtColor(undistorted_left_im, im1_out_gray, CV_BGR2GRAY);
  //cv::cvtColor(undistorted_right_im, im2_out_gray, CV_BGR2GRAY);

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
std::cout << alpha_slider << std::endl;
  showDisparity();
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

int main(int argc, char *argv[])
{
  if (argc !=4){
    std::cerr << "Arguments must be passing in this way: ./read_values /path/sequence/to/parameters_file.xml /path/sequence/to/left_image /path/sequence/to/right_image" << std::endl;
    return -1;
  }
  std::string parameters_xml = argv[1];
  std::string left_image = argv[2];
  std::string right_image = argv[3];

  /*read the xml file */
  cv::FileStorage fs(parameters_xml, cv::FileStorage::READ);
  
  std::cout << "se abre el archivo xml" << std::endl << std::endl;

  if (!fs.isOpened()){
    std::cout << "no se pude abrir el archivo" << std::endl;
  }
  
  cv::Mat left_k, right_k, left_dist, right_dist, r, t;
  fs["P1"] >> left_k;
  fs["P2"] >> right_k;
  fs["dist1"] >> left_dist;
  fs["dist2"] >> right_dist;
  fs["R"] >> r;
  fs["T"] >> t;

  // std::cout << left_k << std::endl << std::endl;
  // std::cout << right_k << std::endl << std::endl;
  // std::cout << left_dist << std::endl << std::endl;
  // std::cout << right_dist << std::endl << std::endl;
  // std::cout << r << std::endl << std::endl;
  // std::cout << t << std::endl << std::endl;

  fs.release();
  
  
  /* Read the image to undistorted */

  cv::Mat mat_left_im, mat_right_im; 
  mat_left_im  = cv::imread(left_image, 0);
  mat_right_im  = cv::imread(right_image, 0);

  /* computes the rotation matrices for each camera that (virtually) make both camera image planes the same plane */
  cv::Mat left_r, right_r, left_k_prima, right_k_prima, q;
  double alpha = 0;
  cv::Size new_left_image_size = mat_left_im.size();
  cv::Size new_right_image_size = mat_right_im.size();
  cv::Rect left_roi, right_roi;

  cv::stereoRectify(left_k, left_dist, right_k, right_dist, mat_left_im.size(), r, t, left_r, right_r, left_k_prima, right_k_prima, q, alpha, cv::CALIB_ZERO_DISPARITY, new_left_image_size , &left_roi , &right_roi );


  /* Computes the undistortion and rectification transformation map for each camera*/

  cv::Mat left_map1, left_map2, right_map1, right_map2;
  cv::initUndistortRectifyMap(left_k, left_dist, left_r, left_k_prima, new_left_image_size, CV_32FC1, left_map1, left_map2);
  cv::initUndistortRectifyMap(right_k, right_dist, right_r, right_k_prima, new_right_image_size, CV_32FC1, right_map1, right_map2);
  
  /* Applies a generic geometrical transformation to an image */

  //cv::Mat undistorted_left_im, undistorted_right_im ;
  cv::remap(mat_left_im, undistorted_left_im, left_map1, left_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
  cv::remap(mat_right_im, undistorted_right_im, right_map1, right_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);


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

  // Initialize values
  alpha_slider = -(undistorted_left_im.size().width-1);
  trackbar_max = 2 * ( undistorted_left_im.size().width-1);
  undistorted_right_im_moved = cv::Mat(undistorted_right_im.size(),CV_8UC1);
  
  // Create Windows
  cv::namedWindow("Linear Blend", 1);
 
  // Create Trackbars
  char trackbarName[50];
  sprintf(trackbarName, "dst [0,%d]", trackbar_max);
  
  cv::createTrackbar(trackbarName, "Linear Blend", &alpha_slider, trackbar_max, on_trackbar);

  cv::setTrackbarPos(trackbarName, "Linear Blend", 632);

  cv::Mat dst;
  // Show some stuff
  on_trackbar(alpha_slider, 0);

  listenToKeys();

  // Save mapping
  cv::FileStorage fs_out("parameters_out.xml", cv::FileStorage::WRITE);
  
  fs_out << "P1" << left_k;
  fs_out << "P2" << right_k;
  fs_out << "dist1" << left_dist;
  fs_out << "dist2" << right_dist;
  fs_out << "R" << r;
  fs_out << "T" << t;
  //fs_out << "LEFT_MAP1" << left_map1;
  //fs_out << "LEFT_MAP2" << left_map2;
  //fs_out << "RIGHT_MAP1" << right_map1;
  //fs_out << "RIGHT_MAP2" << right_map2;
  std::cout <<  alpha_slider << std::endl;
  fs_out << "ALPHA_DISPARITY" << alpha_slider;
  
  fs_out.release();
  
  return 0;
}
