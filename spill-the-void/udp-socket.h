#pragma once
#include <spill-the-void/communication-socket.h>

typedef void raw_type;

class UDPSocket : public CommunicatingSocket {
  public:
    UDPSocket() throw(SocketException);
    UDPSocket(unsigned short localPort) throw(SocketException);

    UDPSocket(
      const string &localAddress,
      unsigned short localPort
    ) throw(SocketException);

    void disconnect() throw(SocketException);

    void sendTo(
      const void *buffer,
      int bufferLen,
      const string &foreignAddress,
      unsigned short foreignPort
    ) throw(SocketException);

    int recvFrom(
      void *buffer,
      int bufferLen,
      string &sourceAddress,
      unsigned short &sourcePort
    ) throw(SocketException);

    void setMulticastTTL(unsigned char multicastTTL) throw(SocketException);
    void joinGroup(const string &multicastGroup) throw(SocketException);
    void leaveGroup(const string &multicastGroup) throw(SocketException);

  private:
    void setBroadcast();
};
