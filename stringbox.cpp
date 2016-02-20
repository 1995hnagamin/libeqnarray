#include "stringbox.hpp"
#include <numeric>
#include <cassert>

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
{
  if (dir == Vertical) {
    assert(first->width() == second->width());
  }
  if (dir == Horizontal) {
    assert(first->height() == second->height());
  }
}

const std::string StringBox::to_string() const {
  if (connection == None) {
    return std::accumulate(std::begin(entity), std::end(entity), std::string{},
      [](const std::string &a, std::string b) {
        return a.empty() ? b : a + "\n" + b;
      });
  }
  if (connection == Vertical) {
    return first->to_string() + "\n" + second->to_string();
  }
  std::string str;
  for (size_t i = 0, len = first->height(); i < len; ++i) {
    str += first->row(i) + second->row(i) + (i == len - 1 ? "" : "\n");
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
  if (connection == None) {
    return entity.empty()? 0:entity.front().length();
  }
  if (connection == Vertical) {
    return first->width();
  }
  return first->width() + second->width();
}

const size_t StringBox::height() const {
  if (connection == None) {
    return entity.size();
  }
  if (connection == Vertical) {
    return first->height() + second->height();
  }
  return first->height();
}

StringBox v_amend(const StringBox &upper, const StringBox &lower) {
  size_t uw = upper.width(), lw = lower.width();
  if (lw < uw) {
    StringBox lower_wide(lower, StringBox(uw - lw, lower.height(), ' '), Horizontal);
    return StringBox(upper, lower_wide, Vertical);
  }
  if (lw > uw) {
    StringBox upper_wide(upper, StringBox(lw - uw, upper.height(), ' '), Horizontal);
    return StringBox(upper_wide, lower, Vertical);
  }
  return StringBox(upper, lower, Vertical);
}

StringBox h_amend(const StringBox &left, const StringBox &right) {
  size_t lh = left.height(), rh = right.height();
  if (lh < rh) {
    StringBox left_wide(left, StringBox(left.width(), rh - lh, ' '), Vertical);
    return StringBox(left_wide, right, Horizontal);
  }
  if (lh > rh) {
    StringBox right_wide(right, StringBox(right.width(), lh - rh, ' '), Vertical);
    return StringBox(left, right_wide, Horizontal);
  }
  return StringBox(left, right, Horizontal);
}

