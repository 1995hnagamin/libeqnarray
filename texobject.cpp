#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <functional>
#include <map>
#include "reader.hpp"

struct TeXObject {
  bool is_leaf;
  std::string entity;
  std::vector<TeXObject> children;
  TeXObject(char c);
  TeXObject(std::string body);
  TeXObject(std::string tag, std::vector<TeXObject> ch);
};

TeXObject::TeXObject(char c):
  is_leaf(true), entity(1, c), children() {}

TeXObject::TeXObject(std::string body):
  is_leaf(true), entity(body), children() {}

TeXObject::TeXObject(std::string tag, std::vector<TeXObject> ch):
  is_leaf(false), entity(tag), children(ch) {}

TeXObject parse(char top, Stream &stream) {
  return TeXObject(top);
}

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

using Macro = std::function<TeXObject(std::vector<TeXObject>)>;

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
  std::string poe;
  while (getline(std::cin, poe)) {
    Stream stream(poe);
    while (!stream.eos()) {
      TeXObject obj(reader.read(stream));
      show(obj);
      std::cout << std::endl;
    }
  }
  return 0;
}
