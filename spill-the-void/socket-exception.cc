#include <spill-the-void/socket-exception.h>

SocketException::SocketException(
  const string &message,
  bool inclSysMsg
) throw() : userMessage(message) {
  if (inclSysMsg) {
    userMessage.append(": ");
    userMessage.append(strerror(errno));
  }
}

SocketException::~SocketException() throw() {}

const char *SocketException::what() const throw() {
  return userMessage.c_str();
}
