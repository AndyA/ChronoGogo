#include <algorithm>
#include <iostream>
#include <math.h>

#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"

#include "chronogogo.hpp"

#define HISTORY 50

using namespace cv;
using namespace std;

int main(int argc, char *argv[]) {
  int histLen = HISTORY;

  if (argc > 2) {
    cerr << "Usage: " << argv[0] << " [<history>]\n";
    return 1;
  }

  if (argc > 1) {
    histLen = atoi(argv[1]);
  }

  cout << "History: " << histLen << " steps\n";

  ChronoGogo gogo(histLen);
  VideoCapture cap(0);

  if (!cap.isOpened())
    return -1;

  cap.set(CV_CAP_PROP_FRAME_WIDTH, 1920);
  cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1080);

  namedWindow("chrono", 1);

  // Grab a frame before building delay map so we know what
  // size it needs to be.
  Mat *fr = gogo.next();
  cap >> *fr;

  // Default to mode '1'
  gogo.setMode('1');

  Mat chrono;

  for (;;) {
    gogo.process(chrono);
    imshow("chrono", chrono);

    int k = waitKey(10);

    if (k == 'Q' || k == 'q' || k == 27)
      break;

    gogo.setMode(k);

    fr = gogo.next();
    cap >> *fr;
  }

  return 0;
}
