#pragma once
#include <spill-the-void/socket-exception.h>
#include <spill-the-void/communication-socket.h>

class TCPSocket : public CommunicatingSocket {
  public:
    TCPSocket() throw(SocketException);

    TCPSocket(
      const string &foreignAddress,
      unsigned short foreignPort
    ) throw(SocketException);

  private:
    friend class TCPServerSocket;
    TCPSocket(int newConnSD);
};
