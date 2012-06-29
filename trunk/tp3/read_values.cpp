#include <iostream>
#include <vector>
#include <fstream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "libelas/src/elas.h"

/// Global Variables
int alpha_slider_max;
int alpha_slider;
double alpha_track_bar;
double beta_track_bar;

cv::Mat undistorted_left_im, undistorted_rigth_im, dst;

/**
 * @function on_trackbar
 * @brief Callback for trackbar
 */
void on_trackbar(int, void*){
  int height = undistorted_rigth_im.size().height;
  int width = undistorted_rigth_im.size().width;
  double left_transparency = 0.5;
  double rigth_transparency = 0.5;
  cv::Mat undistorted_rigth_im_moved;
  undistorted_rigth_im.copyTo(undistorted_rigth_im_moved);
  
  for(int y = 0; y<height; ++y){
    for(int x = 0; x<width; ++x){
      undistorted_rigth_im_moved.at<uchar>(y,x) = undistorted_rigth_im.at<uchar>(y,x + alpha_slider);
    }
  }
  
  cv::addWeighted(undistorted_left_im, left_transparency, undistorted_rigth_im_moved, rigth_transparency, 0.0, dst);

  cv::imshow( "Linear Blend", dst);
}


int main(int argc, char *argv[])
{
  if (argc !=4){
    std::cerr << "Arguments must be passing in this way: ./read_values /path/sequence/to/parameters_file.xml /path/sequence/to/left_image /path/sequence/to/rigth_image" << std::endl;
    return -1;
  }
  std::string parameters_xml = argv[1];
  std::string left_image = argv[2];
  std::string rigth_image = argv[3];

  /*read the xml file */
  cv::FileStorage fs(parameters_xml, cv::FileStorage::READ);
  
  std::cout << "se abre el archivo xml" << std::endl << std::endl;

  if (!fs.isOpened()){
    std::cout << "no se pude abrir el archivo" << std::endl;
  }
  
  cv::Mat left_k, rigth_k, left_dist, rigth_dist, r, t;
  fs["P1"] >> left_k;
  fs["P2"] >> rigth_k;
  fs["dist1"] >> left_dist;
  fs["dist2"] >> rigth_dist;
  fs["R"] >> r;
  fs["T"] >> t;

  std::cout << left_k << std::endl << std::endl;
  std::cout << rigth_k << std::endl << std::endl;
  std::cout << left_dist << std::endl << std::endl;
  std::cout << rigth_dist << std::endl << std::endl;
  std::cout << r << std::endl << std::endl;
  std::cout << t << std::endl << std::endl;

  fs.release();
  
  
  /* Read the image to undistorted */

  cv::Mat mat_left_im, mat_rigth_im; 
  mat_left_im  = cv::imread(left_image, 0);
  mat_rigth_im  = cv::imread(rigth_image, 0);

  /* computes the rotation matrices for each camera that (virtually) make both camera image planes the same plane */
  cv::Mat left_r, rigth_r, left_k_prima, rigth_k_prima, q;
  double alpha = 0;
  cv::Size new_left_image_size = mat_left_im.size();
  cv::Size new_rigth_image_size = mat_rigth_im.size();
  cv::Rect left_roi, rigth_roi;

  cv::stereoRectify(left_k, left_dist, rigth_k, rigth_dist, mat_left_im.size(), r, t, left_r, rigth_r, left_k_prima, rigth_k_prima, q, alpha, cv::CALIB_ZERO_DISPARITY, new_left_image_size , &left_roi , &rigth_roi );


  /* Computes the undistortion and rectification transformation map for each camera*/

  cv::Mat left_map1, left_map2, rigth_map1, rigth_map2;
  cv::initUndistortRectifyMap(left_k, left_dist, left_r, left_k_prima, new_left_image_size, CV_32FC1, left_map1, left_map2);
  cv::initUndistortRectifyMap(rigth_k, rigth_dist, rigth_r, rigth_k_prima, new_rigth_image_size, CV_32FC1, rigth_map1, rigth_map2);
  
  /* Applies a generic geometrical transformation to an image */

  //cv::Mat undistorted_left_im, undistorted_rigth_im ;
  cv::remap(mat_left_im, undistorted_left_im, left_map1, left_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
  cv::remap(mat_rigth_im, undistorted_rigth_im, rigth_map1, rigth_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);


  if(! mat_left_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the left image" << std::endl ;
    return -1;
  }
  
  if(! mat_rigth_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the rigth image" << std::endl ;
    return -1;
  }

  if(! undistorted_left_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the image" << std::endl ;
    return -1;
  }

  if(! undistorted_rigth_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the image" << std::endl ;
    return -1;
  }

  // Initialize values
  alpha_slider = 0;
  alpha_slider_max = undistorted_left_im.size().width-1;
  
  // Create Windows
  cv::namedWindow("Linear Blend", 1);
  
  // Create Trackbars
  char TrackbarName[50];
  sprintf(TrackbarName, "dst [0,%d]", alpha_slider_max);
  
  cv::createTrackbar(TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar);
  
  cv::Mat dst;
  // Show some stuff
  on_trackbar(alpha_slider, 0);

  ////////////////////////////////////////////////////////////////////////////////////////////////

  Elas::parameters param;
  param.postprocess_only_left = true;
  Elas elas(param);  

  cv::Mat_<uchar> im1_out_gray, im2_out_gray;
  cv::cvtColor(im1_out, im1_out_gray, CV_BGR2GRAY);
  cv::cvtColor(im2_out, im2_out_gray, CV_BGR2GRAY);

  // get image width and height
  int32_t width  = im1_out_gray.size().width;
  int32_t height = im1_out_gray.size().height;

  // allocate memory for disparity images
  const int32_t dims[3] = {width,height,width}; // bytes per line = width
  cv::Mat_<float> D1_data(im1_out_gray.size());
  cv::Mat_<float> D2_data(im1_out_gray.size());

  // process
  elas.process(im2_out_gray.data,im1_out_gray.data,(float*)D1_data.data,(float*)D2_data.data,dims);
  
  cv::Mat_<cv::Vec3b> D1_data_color(D1_data.size());
  for (uint j = 0; j < D1_data.cols; j++) {
    for (uint i = 0; i < D1_data.rows; i++) {    
      cv::Vec3b v;
      
      float val = std::min(D1_data.at<float>(i,j) * 0.01f, 1.0f);
      if (val <= 0) v[0] = v[1] = v[2] = 0;
      else {
        float h2 = 6.0f * (1.0f - val);
        unsigned char x  = (unsigned char)((1.0f - fabs(fmod(h2, 2.0f) - 1.0f))*255);
        if (0 <= h2&&h2<1) { v[0] = 255; v[1] = x; v[2] = 0; }
        else if (1 <= h2 && h2 < 2)  { v[0] = x; v[1] = 255; v[2] = 0; }
        else if (2 <= h2 && h2 < 3)  { v[0] = 0; v[1] = 255; v[2] = x; }
        else if (3 <= h2 && h2 < 4)  { v[0] = 0; v[1] = x; v[2] = 255; }
        else if (4 <= h2 && h2 < 5)  { v[0] = x; v[1] = 0; v[2] = 255; }
        else if (5 <= h2 && h2 <= 6) { v[0] = 255; v[1] = 0; v[2] = x; }
      }
      
      D1_data_color.at<cv::Vec3b>(i, j) = v;
    }
    // cout << endl;
  }
    
  
  cv::waitKey(0);                                          // Wait for a keystroke in the window
  
  return 0;
}

