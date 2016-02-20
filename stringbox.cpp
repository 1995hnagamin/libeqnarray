#include "stringbox.hpp"
#include <numeric>

namespace {
  const Direction None = 0;
}

StringBox::StringBox(const std::string &str):
  connection(None),
  entity(std::vector<std::string>(1, str)),
  first(),
  second()
{}

StringBox::StringBox(size_t width, size_t height, char c):
  connection(None),
  entity(height, std::string(width, c)),
  first(),
  second()
{}

StringBox::StringBox(const StringBox &fst, const StringBox &snd, Direction dir):
  connection(dir),
  entity(),
  first(new StringBox(fst)),
  second(new StringBox(snd))
{}

const std::string StringBox::to_string() const {
  if (connection == None) {
    return std::accumulate(std::begin(entity), std::end(entity), std::string{},
      [](const std::string &a, std::string b) {
        return a + "\n" + b;
      });
  }
  if (connection == Vertical) {
    return first->to_string() + "\n" + second->to_string();
  }
  std::string str;
  for (size_t i = 0, len = first->height(); i < len; ++i) {
    str += first->row(i) + second->row(i);
  }
  return str;
}

const char StringBox::at(size_t i, size_t j) const {
  if (connection == None) {
    return entity[i][j];
  }
  size_t w(first->width()), h(first->height());
  if (i < w && j < h) {
    return first->at(i, j);
  }
  return second->at(
      connection == Vertical ?   i : i - w,
      connection == Horizontal ? j : j - h
    );
}

const std::string StringBox::row(size_t i) const {
  if (connection == None) {
    return entity[i];
  }
  if (connection == Vertical) {
    size_t h(first->height());
    return i < h? first->row(i):second->row(i - h);
  }
  return first->row(i) + second->row(i);
}

const bool StringBox::empty() const {
  return entity.empty();
}

const size_t StringBox::width() const {
  return entity.empty()? 0:entity.front().length();
}

const size_t StringBox::height() const {
  return entity.size();
}

StringBox v_amend(const StringBox &upper, const StringBox &lower) {
  size_t up_width(upper.width()), lw_width(lower.width());
  StringBox up_half(
      up_width >= lw_width ?
        upper :
        StringBox(upper,
                  StringBox(lw_width - up_width, upper.height(), ' '),
                  Horizontal));
  StringBox lw_half(
      lw_width >= up_width ?
        lower :
        StringBox(lower,
                  StringBox(up_width - lw_width, lower.height(), ' '),
                    Horizontal));
  return StringBox(up_half, lw_half, Vertical);
}

StringBox h_amend(const StringBox &left, const StringBox &right) {
  size_t l_height(left.height()), r_height(right.height());
  StringBox l_half(
      l_height >= r_height ?
        left :
        StringBox(left,
                  StringBox(left.width(), r_height - l_height, ' '),
                  Vertical));
  StringBox r_half(
      r_height >= l_height ?
        right :
        StringBox(right,
                  StringBox(right.width(), l_height - r_height, ' '),
                    Vertical));
  return StringBox(l_half, r_half, Horizontal);
}

