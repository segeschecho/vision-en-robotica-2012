#include <iostream>
#include <vector>
#include <fstream>
#include <opencv/cv.h>
#include <opencv/highgui.h>



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

  cv::Mat undistorted_left_im, undistorted_rigth_im ;
  cv::remap(mat_left_im, undistorted_left_im, left_map1, left_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
  cv::remap(mat_rigth_im, undistorted_rigth_im, rigth_map1, rigth_map2, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);


  /* Show images*/

  if(! mat_left_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the left image" << std::endl ;
    return -1;
  }
  
  if(! mat_rigth_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the rigth image" << std::endl ;
    return -1;
  }

  cv::namedWindow( "Original Left Image", CV_WINDOW_AUTOSIZE ); // Create a window for display.
  cv::imshow( "Original Left Image", mat_left_im);                   // Show our image inside it.
  
  cv::namedWindow( "Original Rigth Image", CV_WINDOW_AUTOSIZE ); // Create a window for display.
  cv::imshow( "Original Rigth Image", mat_rigth_im);                   // Show our image inside it.

  if(! undistorted_left_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the image" << std::endl ;
    return -1;
  }

  if(! undistorted_rigth_im.data ){                              // Check for invalid input
    std::cerr <<  "Could not open or find the image" << std::endl ;
    return -1;
  }

  cv::namedWindow( "Undistorted Left Image", CV_WINDOW_AUTOSIZE );// Create a window for display.
  cv::imshow( "Undistorted Left Image", undistorted_left_im);                   // Show our image inside it.

  cv::namedWindow( "Undistorted Rigth Image", CV_WINDOW_AUTOSIZE );// Create a window for display.
  cv::imshow( "Undistorted Rigth Image", undistorted_rigth_im);                   // Show our image inside it.


  cv::waitKey(0);                                          // Wait for a keystroke in the window
  
  return 0;
}

