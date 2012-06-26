#include <vector>
#include <fstream>
#include <opencv/cv.h>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    FileStorage fs("parameters.xml", FileStorage::READ);

    cout << "se abre el archivo xml" << endl << endl;

    if (!fs.isOpened()){
      cout << "no se pude abrir el archivo" << endl;
    }

    //cout << " valores: \n" << (string)fs.getFirstTopLevelNode() << endl;

    Mat cameraMatrix2, distCoeffs2;

    //cout << " valores: \n" << (string)fs["P1"] << endl;
    
    //
    //FileNode P1 = fs["P2"] ;
    fs["P1"] >> cameraMatrix2;
    //fs[""] >> distCoeffs2;

    cout << "camera matrix: " << cameraMatrix2 << endl;
    
    fs.release();
    return 0;
}
