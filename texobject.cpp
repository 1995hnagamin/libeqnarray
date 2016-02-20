#include <tuple>
#include <functional>
#include <map>
#include "reader.hpp"
#include "texobject.hpp"

TeXObject::TeXObject(char c):
  is_leaf(true), entity(1, c), children() {}

TeXObject::TeXObject(std::string body):
  is_leaf(true), entity(body), children() {}

TeXObject::TeXObject(std::string tag, std::vector<TeXObject> ch):
  is_leaf(false), entity(tag), children(ch) {}
