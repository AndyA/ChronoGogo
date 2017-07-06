#include "opencv2/opencv.hpp"

class FrameStore {
private:
  unsigned frameCount;
  unsigned used;
  unsigned nextPos;
  cv::Mat *frames;

public:
  FrameStore(unsigned count);
  unsigned position();
  cv::Mat *next();
};
