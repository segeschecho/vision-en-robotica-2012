#include <iostream>
#include <vector>
#include <fstream>
#include <opencv/cv.h>
using namespace std;
using namespace cv;

void showMatrix(Mat M){
  cout << endl << endl;
  for ( int row = 0; row < M.rows ; ++ row ){
    for ( int col = 0; col < M.cols; ++ col ){
      cout << M.at<double>(row,col) << " ";
    }
    cout << endl << endl;
  }

  return;
}


int main(int argc, char *argv[])
{
  FileStorage fs("parameters.xml", FileStorage::READ);
  
  cout << "se abre el archivo xml" << endl << endl;

  if (!fs.isOpened()){
    cout << "no se pude abrir el archivo" << endl;
  }
  
  //cout << " valores: \n" << (string)fs.getFirstTopLevelNode() << endl;
  
  Mat p1, p2, dist1, dist2, r, t;
  fs["P1"] >> p1;
  fs["P2"] >> p2;
  fs["dist1"] >> dist1;
  fs["dist2"] >> dist2;
  fs["R"] >> r;
  fs["T"] >> t;
  showMatrix(p1);
  showMatrix(p2);
  showMatrix(dist1);
  showMatrix(dist2);
  showMatrix(r);
  showMatrix(t);

  fs.release();
  return 0;
}

