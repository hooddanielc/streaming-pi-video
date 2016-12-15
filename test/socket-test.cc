#include <iostream>
#include <thread>
#include <vector>
#include <lick/lick.h>
#include <spill-the-void/tcp-socket.h>
#include <spill-the-void/udp-socket.h>
#include <spill-the-void/tcp-server-socket.h>

using namespace std;

FIXTURE(get_local_address) {
  TCPSocket socket;

  EXPECT_EQ(
    socket.getLocalAddress(),
    "0.0.0.0"
  ) << "gets local address";
}

FIXTURE(get_local_port) {
  TCPSocket socket;
  EXPECT_EQ(socket.getLocalPort(), 0);
}

FIXTURE(set_local_port) {
  TCPSocket socket;
  socket.setLocalPort(1337);
  EXPECT_EQ(socket.getLocalPort(), 1337);
}

FIXTURE(set_local_address_port) {
  TCPSocket socket;
  socket.setLocalAddressAndPort("127.0.0.1", 1337);

  EXPECT_EQ(
    socket.getLocalAddress(),
    "127.0.0.1"
  ) << "sets local address and port";

  EXPECT_EQ(socket.getLocalPort(), 1337);
}

FIXTURE(resolve_service) {
  TCPSocket socket;

  EXPECT_EQ(
    socket.resolveService("http"),
    80
  ) << "resolves 'http' to port 80";
}

FIXTURE(tcp_server_socket) {
  {
    TCPServerSocket socket1("127.0.0.1", 1337);
    TCPServerSocket socket2(1338);
    EXPECT_EQ(socket1.getLocalAddress(), "127.0.0.1");
    EXPECT_EQ(socket1.getLocalPort(), 1337);
    EXPECT_EQ(socket1.getLocalAddress(), "127.0.0.1");
    EXPECT_EQ(socket2.getLocalPort(), 1338);
  }
}

FIXTURE(tcp_socket_connect_send) {
  for (int i = 0; i < 500; ++i) {
    {
      TCPServerSocket server("127.0.0.1", 1340 + i);
      TCPSocket client;

      thread startServer([&server]() {
        TCPSocket *theClient = server.accept();
        char buf[6];
        theClient->recv(&buf, 6);
        EXPECT_EQ(string(buf), "hello");
      });

      client.connect("127.0.0.1", 1340 + i);
      client.send("hello", 6);
      startServer.join();
    }
  }
}

FIXTURE(udp_socket_create) {
  UDPSocket socket1;
  UDPSocket socket2(1337);
  UDPSocket socket3("127.0.0.1", 1338);
  EXPECT_EQ(socket1.getLocalPort(), 0);
  EXPECT_EQ(socket1.getLocalAddress(), "0.0.0.0");
  EXPECT_EQ(socket2.getLocalPort(), 1337);
  EXPECT_EQ(socket2.getLocalAddress(), "0.0.0.0");
  EXPECT_EQ(socket3.getLocalPort(), 1338);
  EXPECT_EQ(socket3.getLocalAddress(), "127.0.0.1");
}

FIXTURE(udp_socket_sending) {
  {
    UDPSocket socket1("127.0.0.1", 1337);
    UDPSocket socket2("127.0.0.1", 1338);

    thread receiveThread([&socket1]() {
      char buf[6];
      string str;
      unsigned short port;

      int result = socket1.recvFrom(
        &buf,
        6,
        str,
        port
      );

      EXPECT_EQ(str, "127.0.0.1");
      EXPECT_EQ(port, 1338);
    });

    socket2.sendTo(
      "hello",
      6,
      "127.0.0.1",
      1337
    );

    receiveThread.join();
  }
}

int main(int argc, char *argv[]) {
  return dj::lick::main(argc, argv);
}
