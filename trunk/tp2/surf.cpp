#include <iostream>
#include <vector>
#include <fstream>
#include <opencv/cv.h>
#include <highgui.h>
using namespace std;

int main(int argc, char *argv[])
{	
   
  if(argc != 4){
    cout << "[ERROR] Usage: " << argv[0] << " img des_file ktps_file" << endl;
    return -1;
  }
  
  cv::Mat img = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);
  if (img.empty()) {
    cout << "[ERROR] Image " << argv[1] << " can't be read" << endl;
    return -1;
  }

  ofstream des_file(argv[2], ios_base::out | ios_base::trunc), kpts_file(argv[3], ios_base::out | ios_base::trunc);
  
  cout << "Extracting surf features..." << endl;
  vector<cv::KeyPoint> keypoints;
  vector<float> descriptors;
  cv::SURF surf(1000, 3, 4);
  surf(img, cv::Mat(), keypoints, descriptors);
  cout << "Complete: " << keypoints.size() << " keypoints extracted" << endl;
  for (unsigned int i = 0; i < keypoints.size(); i++) {
    kpts_file << keypoints[i].pt.x << " " << keypoints[i].pt.y << endl;
    for (unsigned int k = 0; k < (unsigned int)surf.descriptorSize(); k++) {
      des_file << descriptors[i * surf.descriptorSize() + k] << " ";
    }
    des_file << endl;
  }
  return 0;
}
