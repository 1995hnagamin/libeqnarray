#ifndef __READER_HPP_
#define __READER_HPP_

#include <functional>
#include <map>
#include <memory>
#include "stream.hpp"

template<class T>
class Reader {
  using Parser = std::function<T(char, Stream&)>;
  using ReadMacro = std::function<T(Stream&, char, const Reader<T>&)>;
  public:
    Reader(const Parser &p);
    void add_read_macro(char macro_character, ReadMacro hook);
    T read(Stream &str) const;
  private:
    Parser parser;
    std::map<char, ReadMacro> macros;
};

template<class T>
Reader<T>::Reader(const Parser &p):
  parser(p),
  macros()
{}

template<class T>
void Reader<T>::add_read_macro(char macro_character, ReadMacro hook) {
  macros[macro_character] = hook;
}

template<class T>
T Reader<T>::read(Stream &stream) const {
  char top(stream.read());
  if (macros.find(top) != macros.end()) {
    return (macros.at(top))(stream, top, *this);
  }
  return parser(top, stream);
}

#endif // __READER_HPP_
