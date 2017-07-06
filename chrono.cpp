#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

using namespace cv;

int main(int, char **) {
  VideoCapture cap(0); // open the default camera
  if (!cap.isOpened()) // check if we succeeded
    return -1;

  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

  Mat edges;
  namedWindow("edges", 1);
  for (;;) {
    Mat frame;
    cap >> frame; // get a new frame from camera
    if (0) {
      imshow("edges", frame);
    }
    else {
      cvtColor(frame, edges, COLOR_BGR2GRAY);
      flip(edges, edges, 1);
      GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);
      Canny(edges, edges, 0, 30, 3);
      imshow("edges", edges);
    }
    if (waitKey(10) >= 0) break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
