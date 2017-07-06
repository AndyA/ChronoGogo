#include "framestore.hpp"

using namespace cv;

FrameStore::FrameStore(unsigned count) {
  frameCount = count;
  nextPos = used = 0;
  frames = new Mat[count];
}

unsigned FrameStore::position() {
  return nextPos;
}

Mat *FrameStore::next() {
  unsigned pos = nextPos++;
  if (used < nextPos)
    used = nextPos;
  if (nextPos == frameCount)
    nextPos = 0;
  return &frames[pos];
}
