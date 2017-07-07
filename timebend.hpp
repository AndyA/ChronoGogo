#ifndef __TIMEBEND_HPP
#define __TIMEBEND_HPP

#include "opencv2/opencv.hpp"

#include "framestore.hpp"

class TimeBend {
protected:
  FrameStore &fs;
public:
  TimeBend(FrameStore &frameStore) : fs(frameStore) {}
  virtual void process(cv::Mat &out) = 0;
};

class TimeBendMapped : public TimeBend {
private:
  cv::Mat &timeMap;
public:
  TimeBendMapped(FrameStore &frameStore, cv::Mat &map) : TimeBend(frameStore), timeMap(map) {}
  virtual ~TimeBendMapped() {};
  virtual void process(cv::Mat &out);
};

class TimeBendAdaptive : public TimeBend {
public:
  TimeBendAdaptive(FrameStore &frameStore) : TimeBend(frameStore) {}
  virtual ~TimeBendAdaptive() {};
  virtual void process(cv::Mat &out);
};

#endif
