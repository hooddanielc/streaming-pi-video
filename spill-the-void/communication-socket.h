#pragma once
#include <spill-the-void/socket.h>
#include <spill-the-void/socket-exception.h>

class CommunicatingSocket : public Socket {
  public:
    void connect(
      const string &foreignAddress,
      unsigned short foreignPort
    ) throw(SocketException);

    void send(const void *buffer, int bufferLen) throw(SocketException);
    int recv(void *buffer, int bufferLen) throw(SocketException);
    string getForeignAddress() throw(SocketException);
    unsigned short getForeignPort() throw(SocketException);

  protected:
    CommunicatingSocket(int type, int protocol) throw(SocketException);
    CommunicatingSocket(int newConnSD);
};
