#include <iostream>
#include <vector>
#include <fstream>
#include <opencv/cv.h>
using namespace std;
using namespace cv;
int main(int argc, char *argv[])
{
    FileStorage fs("parameters640x480.xml", FileStorage::READ);
    
    Mat cameraMatrix2, distCoeffs2;
    //FileNode P1 = fs["P1"] ;
    //FileNode P1 = fs["P2"] ;
    fs["P1"] >> cameraMatrix2;
    //fs[""] >> distCoeffs2;

    cout << "camera matrix: " << fs["P1"]->type() << endl;
    
    fs.release();
    return 0;
}