#include <math.h>
#include <algorithm>

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

#include "framestore.hpp"
#include "timebend.hpp"

#define HISTORY 50

using namespace cv;
using namespace std;

static double dist(int dx, int dy) {
  return sqrt(dx * dx + dy * dy);
}

static void timeCone(Mat &m, Size sz, double cxp, double cyp, int channel) {
  m.create(sz, CV_8UC3);

  int channels = m.channels();
  CV_Assert(channel >= 0 && channel < channels);

  int cx = (int)(sz.width * cxp);
  int cy = (int)(sz.height * cyp);

  int sx = max(cx, sz.width - 1 - cx);
  int sy = max(cy, sz.height - 1 - cy);

  double maxDist = dist(sx, sy);

  for (int y = 0; y < sz.height; y++) {
    uchar *row = m.ptr<uchar>(y);
    for (int x = 0; x < sz.width; x++) {
      double dotDist = dist(x - cx, y - cy);
      row[x * channels + channel] = (unsigned char) HISTORY * dotDist / maxDist;
    }
  }
}

static void timeRand(Mat &m, Size sz, int channel) {
  m.create(sz, CV_8UC3);

  int channels = m.channels();
  CV_Assert(channel >= 0 && channel < channels);

  for (int y = 0; y < sz.height; y++) {
    uchar *row = m.ptr<uchar>(y);
    for (int x = 0; x < sz.width; x++) {
      double r = (double) rand() / RAND_MAX;
      row[x * channels + channel] = (unsigned char) HISTORY * r;
    }
  }
}

static void timeSlice(Mat &m, Size sz, int down, int channel) {
  m.create(sz, CV_8UC3);

  int channels = m.channels();
  CV_Assert(channel >= 0 && channel < channels);

  for (int y = 0; y < sz.height; y++) {
    uchar *row = m.ptr<uchar>(y);
    int delay = y * HISTORY / sz.height;

    if (down < 0)
      delay = HISTORY - 1 - delay;
    else if (down == 0)
      delay = 0;

    for (int x = 0; x < sz.width; x++) {
      row[x * channels + channel] = delay;
    }
  }
}

static void timeSpin(Mat &timeMap, const Mat &frame, double radius) {
  for (int c = 0; c < 3; c++) {
    double cx = sin(M_PI * 2 / 3 * c) * radius + 0.5;
    double cy = cos(M_PI * 2 / 3 * c) * radius + 0.5;
    timeCone(timeMap, frame.size(), cx, cy, c);
  }
}

static void setMap(Mat &timeMap, const Mat &frame, int which) {
  switch (which) {
  default:
  case '1':
    for (int c = 0; c < 3; c++)
      timeCone(timeMap, frame.size(), 0.5, 0.5, c);
    break;

  case '2':
    timeSpin(timeMap, frame, 0.05);
    break;

  case '3':
    timeSpin(timeMap, frame, 0.3);
    break;

  case '4':
    for (int c = 0; c < 3; c++)
      timeSlice(timeMap, frame.size(), -1, c);
    break;

  case '5':
    for (int c = 0; c < 3; c++)
      timeSlice(timeMap, frame.size(), 1, c);
    break;

  case '6':
    for (int c = 0; c < 3; c++)
      timeSlice(timeMap, frame.size(), c - 1, c);
    break;

  case '9':
    for (int c = 0; c < 3; c++)
      timeRand(timeMap, frame.size(), c);
    break;
  }
}

int main(int, char **) {
  FrameStore fs(HISTORY);
  VideoCapture cap(0);

  if (!cap.isOpened())
    return -1;

  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

  namedWindow("chrono", 1);

  Mat chrono, timeMap;
  Mat *fr = fs.next();

  cap >> *fr;

  setMap(timeMap, *fr, '1');

  TimeBendMapped bender(&fs, &timeMap);

  for (;;) {
    flip(*fr, *fr, 1);

    bender.process(chrono);
    imshow("chrono", chrono);

    int k = waitKey(10);

    if (k == 'Q' || k == 'q')
      break;

    if (k >= '0' && k <= '9')
      setMap(timeMap, *fr, k);

    fr = fs.next();
    cap >> *fr;
  }

  return 0;
}
