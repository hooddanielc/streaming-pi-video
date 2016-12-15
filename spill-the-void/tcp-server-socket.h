#pragma once
#include <spill-the-void/socket.h>
#include <spill-the-void/tcp-socket.h>

class TCPServerSocket : public Socket {
  public:
    TCPServerSocket(
      unsigned short localPort,
      int queueLen = 5
    ) throw(SocketException);

    TCPServerSocket(
      const string &localAddress,
      unsigned short localPort,
      int queueLen = 5
    ) throw(SocketException);

    TCPSocket *accept() throw(SocketException);

  private:
    void setListen(int queueLen) throw(SocketException);
};
