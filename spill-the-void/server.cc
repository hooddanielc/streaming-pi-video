#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>
#include <spill-the-void/udp-socket.h>

using namespace cv;
using namespace std;

const int BUF_LEN = 65540;
const int FRAME_HEIGHT = 720;
const int FRAME_WIDTH = 1280;
const int FRAME_INTERVAL = (1000/30);
const int PACK_SIZE = 4096;
const int ENCODE_QUALITY = 80;

int main(int argc, char * argv[]) {
  if (argc != 2) {
    cerr << "Usage: " << argv[0] << " <Server Port>" << endl;
    exit(1);
  }

  unsigned short servPort = atoi(argv[1]);
  namedWindow("recv", CV_WINDOW_AUTOSIZE);

  try {
    UDPSocket sock(servPort);
    char buffer[BUF_LEN];
    int recvMsgSize;
    string sourceAddress;
    unsigned short sourcePort;
    clock_t last_cycle = clock();

    while (1) {
      do {
        recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);
      } while (recvMsgSize > sizeof(int));

      int total_pack = ((int * ) buffer)[0];
      cout << "expecting length of packs:" << total_pack << endl;
      char * longbuf = new char[PACK_SIZE * total_pack];

      for (int i = 0; i < total_pack; i++) {
        recvMsgSize = sock.recvFrom(buffer, BUF_LEN, sourceAddress, sourcePort);

        if (recvMsgSize != PACK_SIZE) {
          cerr << "Received unexpected size pack:" << recvMsgSize << endl;
          continue;
        }

        memcpy( & longbuf[i * PACK_SIZE], buffer, PACK_SIZE);
      }

      cout << "Received packet from " << sourceAddress << ":" << sourcePort << endl;
      Mat rawData = Mat(1, PACK_SIZE * total_pack, CV_8UC1, longbuf);
      Mat frame = imdecode(rawData, CV_LOAD_IMAGE_COLOR);

      if (frame.size().width == 0) {
        cerr << "decode failure!" << endl;
        continue;
      }

      imshow("recv", frame);
      free(longbuf);
      waitKey(1);
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
