#include <spill-the-void/socket.h>

void Socket::fillAddr(
  const string &address,
  unsigned short port,
  sockaddr_in &addr
) {
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  hostent *host;

  if ((host = gethostbyname(address.c_str())) == NULL) {
    throw SocketException("Failed to resolve name (gethostbyname())");
  }

  addr.sin_addr.s_addr = *((unsigned long *) host->h_addr_list[0]);
  addr.sin_port = htons(port);
}

Socket::Socket(int type, int protocol) throw(SocketException) {
  if ((sockDesc = socket(PF_INET, type, protocol)) < 0) {
    throw SocketException("Socket creation failed (socket())", true);
  }
}

Socket::Socket(int sockDesc) {
  this->sockDesc = sockDesc;
}

Socket::~Socket() {
  ::close(sockDesc);
  sockDesc = -1;
}

string Socket::getLocalAddress() throw(SocketException) {
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getsockname(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of local address failed (getsockname())", true);
  }

  return inet_ntoa(addr.sin_addr);
}

unsigned short Socket::getLocalPort() throw(SocketException) {
  sockaddr_in addr;
  unsigned int addr_len = sizeof(addr);

  if (getsockname(sockDesc, (sockaddr *) &addr, (socklen_t *) &addr_len) < 0) {
    throw SocketException("Fetch of local port failed (getsockname())", true);
  }

  return ntohs(addr.sin_port);
}

void Socket::setLocalPort(unsigned short localPort) throw(SocketException) {
  sockaddr_in localAddr;
  memset(&localAddr, 0, sizeof(localAddr));
  localAddr.sin_family = AF_INET;
  localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  localAddr.sin_port = htons(localPort);

  if (bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
    throw SocketException("Set of local port failed (bind())", true);
  }
}

void Socket::setLocalAddressAndPort(
  const string &localAddress,
  unsigned short localPort
) throw(SocketException) {
  sockaddr_in localAddr;
  Socket::fillAddr(localAddress, localPort, localAddr);

  if (bind(sockDesc, (sockaddr *) &localAddr, sizeof(sockaddr_in)) < 0) {
    throw SocketException("Set of local address and port failed (bind())", true);
  }
}

unsigned short Socket::resolveService(const string &service, const string &protocol) {
  struct servent *serv;

  if ((serv = getservbyname(service.c_str(), protocol.c_str())) == NULL) {
    return atoi(service.c_str());
  } else {
    return ntohs(serv->s_port);
  }
}
