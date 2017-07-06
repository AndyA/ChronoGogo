#include <math.h>

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

#include "framestore.hpp"

#define HISTORY 30

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
  m.create(sz, CV_8U);
  int cx = sz.width / 2;
  int cy = sz.height / 2;
  double maxDist = dist(cx, cy);
  for (int y = 0; y < sz.height; y++) {
    uchar *row = m.ptr<uchar>(y);
    for (int x = 0; x < sz.width; x++) {
      double dotDist = dist(x - cx, y - cy);
      row[x] = (unsigned char) HISTORY * dotDist / maxDist;
    }
  }
}

static void timeBend(Mat &out, FrameStore &fs, const Mat &timeMap) {
  Mat *fr = fs.history(0);
  if (!fr) return;

  int channels = fr->channels();

  CV_Assert(channels == 3);

  Size size = fr->size();
  out.create(size, fr->type());

  if (out.isContinuous() && fr->isContinuous() && timeMap.isContinuous()) {
    size.width *= size.height;
    size.height = 1;
  }

  for (int y = 0; y < size.height; y++) {
    uchar *hrow[HISTORY];
    for (int i = 0; i < HISTORY; i++) {
      Mat *m = fs.history(i);
      if (m)
        hrow[i] = m->ptr<uchar>(y);
      else
        hrow[i] = 0;
    }
    uchar *orow = out.ptr<uchar>(y);
    const uchar *trow = timeMap.ptr<uchar>(y);

    for (int x = 0, xx = 0; x < size.width; x++, xx += channels) {
      int delay = trow[x];

      if (delay < HISTORY) {
        uchar *src = hrow[delay];
        if (src) {
          orow[xx + 0] = src[xx + 0];
          orow[xx + 1] = src[xx + 1];
          orow[xx + 2] = src[xx + 2];
          continue;
        }
      }

      orow[xx + 0] = 0;
      orow[xx + 1] = 0;
      orow[xx + 2] = 0;
    }
  }
}

int main(int, char **) {
  FrameStore fs(HISTORY);
  VideoCapture cap(0); // open the default camera

  if (!cap.isOpened()) // check if we succeeded
    return -1;

  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

  namedWindow("chrono", 1);

  Mat chrono, timeMap;
  Mat *fr = fs.next();
  cap >> *fr;

  timeCone(timeMap, fr->size());

  for (;;) {
    flip(*fr, *fr, 1);

    if (1) {
      timeBend(chrono, fs, timeMap);
      imshow("chrono", chrono);
    }
    else {
      cvtColor(*fr, chrono, COLOR_BGR2GRAY);
      flip(chrono, chrono, 1);
      GaussianBlur(chrono, chrono, Size(7, 7), 1.5, 1.5);
      Canny(chrono, chrono, 0, 30, 3);
      invert(chrono);
      imshow("chrono", chrono);
    }

    waitKey(10);

    fr = fs.next();
    cap >> *fr;
  }
  // the camera will be deinitialized automatically in VideoCapture destructor
  return 0;
}
