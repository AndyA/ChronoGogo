#include "chronogogo.hpp"

using namespace cv;

ChronoGogo::ChronoGogo(unsigned histSize) : fs(histSize) {
  mappedBender = new TimeBendMapped(fs, timeMap);
  adaptiveBender = new TimeBendAdaptive(fs);
  bender = mappedBender;
}

ChronoGogo::~ChronoGogo() {
  delete mappedBender;
  delete adaptiveBender;
}

static double dist(int dx, int dy) {
  return sqrt(dx * dx + dy * dy);
}

static void timeCone(Mat &m, Size sz, double cxp, double cyp, int histLen, int channel) {
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
      row[x * channels + channel] = (unsigned char) histLen * dotDist / maxDist;
    }
  }
}

static void timeRand(Mat &m, Size sz, int histLen, int channel) {
  m.create(sz, CV_8UC3);

  int channels = m.channels();
  CV_Assert(channel >= 0 && channel < channels);

  for (int y = 0; y < sz.height; y++) {
    uchar *row = m.ptr<uchar>(y);
    for (int x = 0; x < sz.width; x++) {
      double r = (double) rand() / RAND_MAX;
      row[x * channels + channel] = (unsigned char) histLen * r;
    }
  }
}

static void timeSlice(Mat &m, Size sz, int down, int histLen,  int channel) {
  m.create(sz, CV_8UC3);

  int channels = m.channels();
  CV_Assert(channel >= 0 && channel < channels);

  for (int y = 0; y < sz.height; y++) {
    uchar *row = m.ptr<uchar>(y);
    int delay = y * histLen / sz.height;

    if (down < 0)
      delay = histLen - 1 - delay;
    else if (down == 0)
      delay = 0;

    for (int x = 0; x < sz.width; x++) {
      row[x * channels + channel] = delay;
    }
  }
}

static void timeSpin(Mat &timeMap, const Mat &frame, double radius, int histLen) {
  for (int c = 0; c < 3; c++) {
    double cx = sin(M_PI * 2 / 3 * c) * radius + 0.5;
    double cy = cos(M_PI * 2 / 3 * c) * radius + 0.5;
    timeCone(timeMap, frame.size(), cx, cy, histLen, c);
  }
}

void ChronoGogo::setMode(int key) {

  unsigned histLen = fs.length();
  Mat *frame = fs.history(0);
  CV_Assert(frame);

  switch (key) {
  default:
  case '1':
    for (int c = 0; c < 3; c++)
      timeCone(timeMap, frame->size(), 0.5, 0.5, histLen, c);
    bender = mappedBender;
    break;

  case '2':
    timeSpin(timeMap, *frame, 0.05, histLen);
    bender = mappedBender;
    break;

  case '3':
    timeSpin(timeMap, *frame, 0.3, histLen);
    bender = mappedBender;
    break;

  case '4':
    for (int c = 0; c < 3; c++)
      timeSlice(timeMap, frame->size(), -1, histLen, c);
    bender = mappedBender;
    break;

  case '5':
    for (int c = 0; c < 3; c++)
      timeSlice(timeMap, frame->size(), 1, histLen, c);
    bender = mappedBender;
    break;

  case '6':
    for (int c = 0; c < 3; c++)
      timeSlice(timeMap, frame->size(), c - 1, histLen, c);
    bender = mappedBender;
    break;

  case '7':
    bender = adaptiveBender;
    break;

  case '9':
    for (int c = 0; c < 3; c++)
      timeRand(timeMap, frame->size(), histLen, c);
    bender = mappedBender;
    break;
  }
}

void ChronoGogo::process(Mat &out) {
  if (bender) bender->process(out);
}

Mat *ChronoGogo::next() {
  return fs.next();
}
