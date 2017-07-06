#ifndef __FRAMESTORE_HPP
#define __FRAMESTORE_HPP

#include "opencv2/opencv.hpp"

class FrameStore {
private:
  unsigned frameCount;
  unsigned used;
  unsigned nextPos;
  cv::Mat *frames;

public:
  FrameStore(unsigned count);
  ~FrameStore();
  unsigned position();
  unsigned length();
  cv::Mat *next();
  cv::Mat *history(int index);
};

#endif
