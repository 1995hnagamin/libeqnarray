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
