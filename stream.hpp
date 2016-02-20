#pragma once
#include <string>

class Stream {
  public:
    Stream(const std::string &str);
    char read();
    char peek();
    void unread();
    bool eos();
  private:
    const char *data;
    const char *end;
};

