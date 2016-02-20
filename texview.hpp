#pragma once
#include <functional>
#include <map>
#include "stringbox.hpp"
#include "texobject.hpp"

using Converter = std::function<StringBox(TeXObject)>;
using Tag = std::string;

class TeXView {
  public:
    void add_converter(Tag tag, Converter converter);
    StringBox convert(TeXObject object);
  private:
    std::map<Tag, Converter> converters;
};
