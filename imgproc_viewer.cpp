////////////////////////////////////////////////////////////////////////
//
// imgproc_viewer.cpp
//
// Presents a series of images with the ability to scroll through and
// run an image processing algorithm on each one.
// 
// Compile by running
// g++ -o imgproc_viewer imgproc_viewer.cpp `pkg-config --libs opencv`
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
Mat** loaded_images;

Mat getProcessedImage(Mat img) {
  Mat processed;
  dilate(img, processed, Mat(), Point(-1, -1));
  return processed;
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
  Mat *img = new Mat();
  if (loaded_images[current_image_index] != 0) {
    img = loaded_images[current_image_index];
  } else {
    *img = imread(filenames[current_image_index].c_str());
    loaded_images[current_image_index] = img;
  }
  Mat processedImage = getProcessedImage(*img);
  imshow("Original Image", *img);
  imshow("Processed Image", processedImage);
}

int main(int argc, char *argv[]) {
  if (argc <= 1 || argc >= 3) {
    cout << "Usage: " << argv[0] << " <images_directory>" << endl;
    exit(0);
  }

  filenames = getFileNamesInDirectory(argv[1]);
  
  loaded_images = new Mat *[filenames.size()];
  memset(loaded_images, 0, filenames.size()*sizeof(Mat*));

  namedWindow("Original Image", CV_WINDOW_AUTOSIZE); 
  moveWindow("Original Image", 0, 0);
  namedWindow("Processed Image", CV_WINDOW_AUTOSIZE);
  moveWindow("Processed Image", 200, 200);

  createTrackbar("Image Number:", "Original Image",
    &current_image_index, filenames.size() - 1, loadImage);
  loadImage(0, 0); 
  waitKey(0);
  return 0;
}
