#include "stream.hpp"

Stream::Stream(const std::string &str):
  data(str.c_str()),
  end(str.c_str() + str.length())
{}

char Stream::read() {
  if (eos()) {
    return '\0';
  }
  return *(data++);
}

char Stream::peek() {
  return eos() ? '\0' : *data;
}

void Stream::unread() {
  --data;
}

bool Stream::eos() {
  return data >= end;
}

