#include "texview.hpp"

StringBox TeXView::convert(TeXObject object) {
  std::string name("hoge");
  return StringBox(name);
}

void TeXView::add_converter(Tag tag, Converter converter) {
  converters[tag] = converter;
}
