#pragma once
#include <string>
#include <exception>
#include <string.h>

using namespace std;

class SocketException : public exception {
  public:
    SocketException(const string &message, bool inclSysMsg = false) throw();
    ~SocketException() throw();
    const char *what() const throw();

  private:
    string userMessage;
};
