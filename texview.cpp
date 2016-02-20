#include "texview.hpp"
#include <cassert>

StringBox TeXView::convert(TeXObject object) {
  if (object.is_leaf) {
    return StringBox(object.entity);
  }
  std::string tag(object.entity);
  assert(converters.find(tag) != converters.end());
  return (converters.at(tag))(object);
}

void TeXView::add_converter(Tag tag, Converter converter) {
  converters[tag] = converter;
}
