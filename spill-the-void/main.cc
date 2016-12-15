#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int, char**) {
    VideoCapture capture(0);

    if(!capture.isOpened()){
      return -1;
    }

    // get a frame from camera to know frame size and type
    Mat frame;
    capture >> frame;

    if (frame.empty()) {
      cerr << "Frame returned nothing" << endl;
      return -1;
    }

    bool isColor = (frame.type() == CV_8UC3);

    // Initialize Video Writer
    VideoWriter writer;
    int codec = CV_FOURCC('M', 'J', 'P', 'G');
    double fps = 25.0;
    string filename = "./live.avi";
    writer.open(filename, codec, fps, frame.size(), isColor);

    if (!writer.isOpened()) {
      cerr << "Writer failed to open" << endl;
      return -1;
    }

    // start writing video file
    cout << "Writing videofile: " << filename << endl;
    cout << "Press any key to terminate" << endl;

    for (;;) {
      if (!capture.read(frame)) {
        cerr << "ERROR! blank frame grabbed\n";
        break;
      }

      writer.write(frame);
    }

    return 0;
}
