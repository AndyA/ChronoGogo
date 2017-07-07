#ifndef __CHRONOGOGO_HPP
#define __CHRONOGOGO_HPP

#include "opencv2/opencv.hpp"

#include "framestore.hpp"
#include "timebend.hpp"

class ChronoGogo {
private:
  FrameStore fs;
  TimeBend *bender;
  TimeBendMapped *mappedBender;
  TimeBendAdaptive *adaptiveBender;
  cv::Mat timeMap;
  bool flipped;

public:
  ChronoGogo(unsigned histSize);
  ~ChronoGogo();
  void setMode(int key);
  void setFlipped(bool f);
  bool getFlipped();
  void toggleFlipped();
  void process(cv::Mat &out);
  cv::Mat *next();
};

#endif
