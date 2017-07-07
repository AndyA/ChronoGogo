#include <math.h>
#include <algorithm>

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

#include "chronogogo.hpp"

#define HISTORY 50

using namespace cv;
using namespace std;

int main(int, char **) {
  ChronoGogo gogo(HISTORY);
  VideoCapture cap(0);

  if (!cap.isOpened())
    return -1;

  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

  namedWindow("chrono", 1);

  Mat *fr = gogo.next();
  Mat chrono;

  cap >> *fr;

  gogo.setMode('1');

  for (;;) {
    flip(*fr, *fr, 1);

    gogo.process(chrono);
    imshow("chrono", chrono);

    int k = waitKey(10);

    if (k == 'Q' || k == 'q' || k == 27)
      break;

    if (k >= '0' && k <= '9')
      gogo.setMode(k);

    fr = gogo.next();
    cap >> *fr;
  }

  return 0;
}
