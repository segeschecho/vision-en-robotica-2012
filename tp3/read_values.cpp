#include <iostream>
#include <vector>
#include <fstream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
//using namespace std;
//using namespace cv;

/*
void showMatrix(Mat M){
  std::cout << std::endl << std::endl;
  for ( int row = 0; row < M.rows ; ++ row ){
    for ( int col = 0; col < M.cols; ++ col ){
      std::cout << M.at<double>(row,col) << " ";
    }
    std::cout << std::endl << std::endl;
  }

  return;
}
*/

int main(int argc, char *argv[])
{
  /*read the xml file */
  cv::FileStorage fs("parameters.xml", cv::FileStorage::READ);
  
  std::cout << "se abre el archivo xml" << std::endl << std::endl;

  if (!fs.isOpened()){
    std::cout << "no se pude abrir el archivo" << std::endl;
  }
  
  cv::Mat k1, k2, dist1, dist2, r, t;
  fs["P1"] >> k1;
  fs["P2"] >> k2;
  fs["dist1"] >> dist1;
  fs["dist2"] >> dist2;
  fs["R"] >> r;
  fs["T"] >> t;

  std::cout << k1 << std::endl << std::endl;
  std::cout << k2 << std::endl << std::endl;
  std::cout << dist1 << std::endl << std::endl;
  std::cout << dist2 << std::endl << std::endl;
  std::cout << r << std::endl << std::endl;
  std::cout << t << std::endl << std::endl;

  fs.release();
  
  
  /* Read the image to undistorted */

  cv::Mat image; 
  image = cv::imread("./izquierda/Image3.tif", 0);

  /* computes the rotation matrices for each camera that (virtually) make both camera image planes the same plane */

  cv::Mat r1, r2, k1_prima, k2_prima, q;
  cv::Rect validRoir2;
  double alpha = 0;
  cv::Size newImageSize = image.size();
  cv::Rect roi1, roi2;

  cv::stereoRectify(k1, dist1, k2, dist2, image.size(), r, t, r1, r2, k1_prima, k2_prima, q, alpha, cv::CALIB_ZERO_DISPARITY, newImageSize, &roi1, &roi2);


  /* Computes the undistortion and rectification transformation map for each camera*/

  cv::Mat map11, map12, map21, map22;
  cv::initUndistortRectifyMap(k1, dist1, r1, k1_prima, newImageSize, CV_32FC1, map11, map12);
  cv::initUndistortRectifyMap(k2, dist2, r2, k2_prima, newImageSize, CV_32FC1, map21, map22);
  
  /* Applies a generic geometrical transformation to an image */

  cv::Mat undistorted_image;
  cv::remap(image, undistorted_image, map11, map12, cv::INTER_LINEAR, cv::BORDER_CONSTANT, 0);
  //cv::remap(image, undistorted_image, map21, map22, INTER_LINEAR, BORDER_CONSTANT, 0);


  /* Show images*/

  if(! image.data ){                              // Check for invalid input
    std::cout <<  "Could not open or find the image" << std::endl ;
    return -1;
  }

  cv::namedWindow( "Original Image", CV_WINDOW_AUTOSIZE );// Create a window for display.
  cv::imshow( "Original Image", image);                   // Show our image inside it.
  
  if(! undistorted_image.data ){                              // Check for invalid input
    std::cout <<  "Could not open or find the image" << std::endl ;
    return -1;
  }

  cv::namedWindow( "Undistorted Image", CV_WINDOW_AUTOSIZE );// Create a window for display.
  cv::imshow( "Undistorted Image", undistorted_image);                   // Show our image inside it.


  cv::waitKey(0);                                          // Wait for a keystroke in the window


  
  return 0;
}

