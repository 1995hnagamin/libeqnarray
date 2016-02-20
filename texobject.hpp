#pragma once
#include <vector>
#include <string>

struct TeXObject {
  bool is_leaf;
  std::string entity;
  std::vector<TeXObject> children;
  TeXObject(char c);
  TeXObject(std::string body);
  TeXObject(std::string tag, std::vector<TeXObject> ch);
};

