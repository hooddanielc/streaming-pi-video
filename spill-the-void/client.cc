#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <spill-the-void/udp-socket.h>

using namespace std;
using namespace cv;

const int FRAME_HEIGHT = 720;
const int FRAME_WIDTH = 1280;
const int FRAME_INTERVAL = (1000/30);
const int PACK_SIZE = 4096;
const int ENCODE_QUALITY = 80;

int main(int argc, char * argv[]) {
  if ((argc < 3) || (argc > 3)) {
    cerr << "Usage: " << argv[0] << " <Server> <Server Port>\n";
    exit(1);
  }

  string servAddress = argv[1];
  unsigned short servPort = Socket::resolveService(argv[2], "udp");

  try {
    UDPSocket sock;
    int jpegqual =  ENCODE_QUALITY;
    Mat frame, send;
    vector < uchar > encoded;
    VideoCapture cap(0);

    if (!cap.isOpened()) {
      cerr << "OpenCV Failed to open camera";
      exit(1);
    }

    clock_t last_cycle = clock();

    while (1) {
      cap >> frame;

      if (frame.size().width==0) {
        continue;
      }

      resize(frame, send, Size(FRAME_WIDTH, FRAME_HEIGHT), 0, 0, INTER_LINEAR);
      vector < int > compression_params;
      compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
      compression_params.push_back(jpegqual);
      imencode(".jpg", send, encoded, compression_params);
      int total_pack = 1 + (encoded.size() - 1) / PACK_SIZE;
      int ibuf[1];
      ibuf[0] = total_pack;
      sock.sendTo(ibuf, sizeof(int), servAddress, servPort);

      for (int i = 0; i < total_pack; i++){
        sock.sendTo( & encoded[i * PACK_SIZE], PACK_SIZE, servAddress, servPort);
      }

      clock_t next_cycle = clock();
      double duration = (next_cycle - last_cycle) / (double) CLOCKS_PER_SEC;
      cout << "\teffective FPS:" << (1 / duration) << " \tkbps:" << (PACK_SIZE * total_pack / duration / 1024 * 8) << endl;
      cout << next_cycle - last_cycle;
      last_cycle = next_cycle;
    }
  } catch (SocketException & e) {
    cerr << e.what() << endl;
    exit(1);
  }

  return 0;
}
