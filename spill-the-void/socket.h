#pragma once
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <spill-the-void/socket-exception.h>

using namespace std;

class Socket {
  public:
    ~Socket();
    string getLocalAddress() throw(SocketException);
    unsigned short getLocalPort() throw(SocketException);
    void setLocalPort(unsigned short localPort) throw(SocketException);

    void setLocalAddressAndPort(
      const string &localAddress,
      unsigned short localPort = 0
    ) throw(SocketException);

    static unsigned short resolveService(
      const string &service,
      const string &protocol = "tcp"
    );

    static void fillAddr(
      const string &address,
      unsigned short port,
      sockaddr_in &addr
    );

  private:
    Socket(const Socket &sock);
    void operator=(const Socket &sock);

  protected:
    int sockDesc;
    Socket(int type, int protocol) throw(SocketException);
    Socket(int sockDesc);
};
