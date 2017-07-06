#include "framestore.hpp"

using namespace cv;

FrameStore::FrameStore(unsigned count) {
  frameCount = count;
  nextPos = used = 0;
  frames = new Mat[count];
}

FrameStore::~FrameStore() {
  delete [] frames;
}

unsigned FrameStore::position() {
  return nextPos;
}

unsigned FrameStore::length() {
  return frameCount;
}

Mat *FrameStore::next() {
  unsigned pos = nextPos++;
  if (used < nextPos)
    used = nextPos;
  if (nextPos == frameCount)
    nextPos = 0;
  return &frames[pos];
}

Mat *FrameStore::history(int index) {
  if (index < 0)
    return 0;

  // 0 == current frame, 1 == frame before etc
  int pos = (int) nextPos - 1 - index;

  if (pos < 0) {
    if (used < frameCount)
      return 0;
    pos += frameCount;
  }

  return &frames[pos];
}
