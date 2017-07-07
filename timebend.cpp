#include "timebend.hpp"

using namespace cv;

void TimeBendMapped::process(Mat &out) {
  Mat *fr = fs.history(0);
  if (!fr)
    return;

  int channels = fr->channels();

  CV_Assert(channels == 3);

  Size size = fr->size();
  out.create(size, fr->type());

  if (out.isContinuous() && fr->isContinuous() && timeMap.isContinuous()) {
    size.width *= size.height;
    size.height = 1;
  }

  int width = size.width * channels;
  int histLen = fs.length();

  for (int y = 0; y < size.height; y++) {
    uchar *hrow[histLen];

    for (int i = 0; i < histLen; i++) {
      Mat *m = fs.history(i);
      hrow[i] = m ? m->ptr<uchar>(y) : 0;
    }

    uchar *orow = out.ptr<uchar>(y);
    const uchar *trow = timeMap.ptr<uchar>(y);

    for (int x = 0; x < width; x++) {
      int delay = trow[x];

      if (delay < histLen) {
        uchar *src = hrow[delay];
        if (src) {
          orow[x] = src[x];
          continue;
        }
      }

      orow[x] = 0;
    }
  }
}

void TimeBendAdaptive::process(Mat &out) {
  Mat *fr = fs.history(0);
  if (!fr)
    return;

  int channels = fr->channels();

  CV_Assert(channels == 3);

  Size size = fr->size();
  out.create(size, fr->type());

  if (out.isContinuous() && fr->isContinuous()) {
    size.width *= size.height;
    size.height = 1;
  }

  int width = size.width * channels;
  int histLen = fs.length();

  for (int y = 0; y < size.height; y++) {
    uchar *hrow[histLen];

    for (int i = 0; i < histLen; i++) {
      Mat *m = fs.history(i);
      hrow[i] = m ? m->ptr<uchar>(y) : 0;
    }

    uchar *orow = out.ptr<uchar>(y);

    for (int x = 0; x < width; x++) {
      int delay = (255 - hrow[0][x]) * (histLen - 1) / 255;

      if (delay < histLen) {
        uchar *src = hrow[delay];
        if (src) {
          orow[x] = src[x];
          continue;
        }
      }

      orow[x] = 0;
    }
  }
}
