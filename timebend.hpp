#ifndef __TIMEBEND_HPP
#define __TIMEBEND_HPP

#include "opencv2/opencv.hpp"

#include "framestore.hpp"

class TimeBend {
protected:
  FrameStore &fs;
public:
  TimeBend(FrameStore &frameStore);
  virtual void process(cv::Mat &out) = 0;
};

class TimeBendMapped : public TimeBend {
private:
  cv::Mat &timeMap;
public:
  TimeBendMapped(FrameStore &frameStore, cv::Mat &map);
  virtual ~TimeBendMapped() {};
  virtual void process(cv::Mat &out);
};

#endif
