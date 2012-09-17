////////////////////////////////////////////////////////////////////////
//
// imgproc_viewer.cpp
//
// Presents a series of images with the ability to scroll through and
// run an image processing algorithm on each one.
// 
// Compile by running
// g++ imgproc_viewer -o imgproc_viewer -lopencv_core -lopencv_highgui -lopencv_imgproc
//
////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <dirent.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

using namespace cv;
using namespace std;

int current_image_index = 0;
vector<string> filenames;

IplImage* getProcessedImage(IplImage* img) {
  IplImage* imgHSV = cvCreateImage(cvGetSize(img), 8, 3);
  cvCvtColor(img, imgHSV, CV_BGR2HSV);
  IplImage* imgThreshed = cvCreateImage(cvGetSize(img), 8, 1);
  cvInRangeS(imgHSV, cvScalar(60, 100, 100), cvScalar(120, 255, 255), imgThreshed);
  cvReleaseImage(&imgHSV);
  return imgThreshed;
}

// modified from http://stackoverflow.com/
//   questions/612097/how-can-i-get-a-list-of-files-in-a-directory-using-c-or-c
vector<string> getFileNamesInDirectory(char* directory) {
  vector<string> filenames;
  DIR *dir;
  struct dirent *ent;
  dir = opendir (directory);
  if (dir != NULL) {
    while ((ent = readdir (dir)) != NULL) {
      if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")) {
        stringstream image_location;
        image_location << directory << "/" << ent->d_name;
        filenames.push_back(image_location.str());
      }
    }
    closedir(dir);
  } else {
    perror("");
  }
  sort(filenames.begin(), filenames.end());
  return filenames;
}

void loadImage(int, void*) {
  IplImage* img = 0; 
  img = cvLoadImage(filenames[current_image_index].c_str());
  IplImage* processedImage = getProcessedImage(img);
  cvShowImage("Original Image", img);
  cvShowImage("Processed Image", processedImage);
}

int main(int argc, char *argv[]) {
  if (argc <= 1 || argc >= 3) {
    cout << "Usage: " << argv[0] << " <images_directory>" << endl;
    exit(0);
  }

  filenames = getFileNamesInDirectory(argv[1]);

  cvNamedWindow("Original Image", CV_WINDOW_AUTOSIZE); 
  cvMoveWindow("Original Image", 0, 0);
  namedWindow("Processed Image", CV_WINDOW_AUTOSIZE);
  cvMoveWindow("Processed Image", 200, 200);

  createTrackbar("Image Number:", "Original Image",
    &current_image_index, filenames.size() - 1, loadImage);
  loadImage(0, 0); 
  waitKey(0);
  return 0;
}
