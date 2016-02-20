#include <iostream>
#include <cassert>
#include "texobject.hpp"
#include "texview.hpp"
#include "reader.hpp"
#include "stream.hpp"

using Macro = std::function<TeXObject(std::vector<TeXObject>)>;

void show(TeXObject obj) {
  if (!obj.is_leaf) {
    std::cout << "(" << obj.entity << " ";
    for (TeXObject child : obj.children) {
      show(child);
      std::cout << " ";
    }
    std::cout << ")";
  } else {
    std::cout << "\'" << obj.entity << "\'";
  }
}

TeXObject parse(char top, Stream &stream) {
  return TeXObject(top);
}

int main() {
  Reader<TeXObject> reader(parse);
  reader.add_read_macro('_',
      [](Stream &stream, char underbar, const Reader<TeXObject>& reader) {
        return TeXObject("subscript", {reader.read(stream)});
      });
  reader.add_read_macro('^',
      [](Stream &stream, char cap, const Reader<TeXObject>& reader) {
        return TeXObject("superscript", {reader.read(stream)});
      });
  reader.add_read_macro('{',
      [](Stream &stream, char paren, const Reader<TeXObject> &reader) {
        std::vector<TeXObject> objs;
        for (char c = stream.read(); c != '}'; c = stream.read()) {
          stream.unread();
          TeXObject obj(reader.read(stream));
          objs.push_back(obj);
        }
        return TeXObject("group", objs);
      });
  std::map<std::string, std::tuple<size_t, Macro>> macros;
  reader.add_read_macro('\\',
      [&macros](Stream &stream, char backslash, const Reader<TeXObject> &reader) {
        std::string name;
        while (std::isalpha(stream.peek())) {
          name.push_back(stream.read());
        }
        if (macros.find(name) == macros.end()) {
          throw;
        }
        size_t arity;
        Macro macro;
        std::vector<TeXObject> args;
        std::tie(arity, macro) = macros.at(name);
        for (size_t i = 0; i < arity; ++i) {
          TeXObject obj(reader.read(stream));
          args.push_back(obj);
        }
        return macro(args);
      });
  macros["frac"] = std::make_tuple(2,
      [](std::vector<TeXObject> objs) {
        return TeXObject("frac", {objs[0], objs[1]});
      });
  macros["pi"] = std::make_tuple(0,
      [](std::vector<TeXObject> objs) {
        return TeXObject("π");
      });
  macros["int"] = std::make_tuple(0,
      [](std::vector<TeXObject> objs) {
        return TeXObject("∫");
      });
  macros["infty"] = std::make_tuple(0,
      [](std::vector<TeXObject> objs) {
        return TeXObject("∞");
      });

  TeXView tv;
  tv.add_converter("frac",
      [&tv](TeXObject obj) {
        assert(obj.children.size() == 2);
        StringBox numer(tv.convert(obj.children[0])),
                  denom(tv.convert(obj.children[1]));
        StringBox vinculum(std::max(numer.width(), denom.width()), 1, '-');
        return v_amend(v_amend(numer, vinculum), denom);
      });

  std::string poe;
  while (getline(std::cin, poe)) {
    Stream stream(poe);
    while (!stream.eos()) {
      TeXObject obj(reader.read(stream));
      StringBox sb(tv.convert(obj));
      std::cout << "\"" << sb.to_string() << "\"";
    }
  }
  return 0;
}
