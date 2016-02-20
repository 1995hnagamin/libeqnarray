#include <functional>
#include <map>
#include <memory>

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
