#include <math.h>

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

#include "framestore.hpp"

using namespace cv;
using namespace std;

static void invert(Mat &m) {
  CV_Assert(m.depth() == CV_8U);
  CV_Assert(m.dims == 2);

  Size size = m.size();

  if (m.isContinuous()) {
    size.width *= size.height;
    size.height = 1;
  }

  int width = size.width * m.channels();

  for (int y = 0; y < size.height; y++) {
    uchar *row = m.ptr<uchar>(y);
    for (int x = 0; x < width; x++) {
      row[x] = ~row[x];
    }
  }

}

static double dist(int dx, int dy) {
  return  sqrt(dx * dx + dy * dy);
}

static void timeCone(Mat &m, Size sz) {
  m.create(sz.height, sz.width, CV_8U);
  int cx = sz.width / 2;
  int cy = sz.height / 2;
  double maxDist = dist(cx, cy);
  for (int y = 0; y < sz.height; y++) {
    uchar *row = m.ptr<uchar>(y);
    for (int x = 0; x < sz.width; x++) {
      double dotDist = dist(x - cx, y - cy);
      row[x] = (unsigned char) 255 * dotDist / maxDist;
    }
  }
}

int main(int, char **) {
  FrameStore fs(256);
  VideoCapture cap(0); // open the default camera

  if (!cap.isOpened()) // check if we succeeded
    return -1;

  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

  namedWindow("chrono", 1);

  Mat chrono, frame, timeMap;

  cap >> frame;

  timeCone(timeMap, frame.size());

  for (;;) {
    Mat *fr = fs.next();
    cap >> *fr; // get a new *fr from camera
    if (0) {
      imshow("chrono", *fr);
    }
    else {
      cvtColor(*fr, chrono, COLOR_BGR2GRAY);
      flip(chrono, chrono, 1);
      GaussianBlur(chrono, chrono, Size(7, 7), 1.5, 1.5);
      Canny(chrono, chrono, 0, 30, 3);
      if (1) {
        char buf[100];
        sprintf(buf, "pos: %3d", fs.position());
        putText(chrono, buf, Point2f(100, 150), FONT_HERSHEY_DUPLEX, 3,  Scalar(255, 255, 255));
      }
      invert(chrono);
      imshow("chrono", chrono);
    }
    if (waitKey(10) >= 0) break;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
