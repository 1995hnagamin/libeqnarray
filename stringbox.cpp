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
  second(),
  baseline(size_t(0))
{}

StringBox::StringBox(size_t width, size_t height, char c):
  connection(None),
  entity(height, std::string(width, c)),
  first(),
  second(),
  baseline()
{}

StringBox::StringBox(const StringBox &fst, const StringBox &snd, Direction dir, Baseline base):
  connection(dir),
  entity(),
  first(new StringBox(fst)),
  second(new StringBox(snd)),
  baseline(base)
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

Baseline StringBox::get_baseline() const {
  return baseline;
}

void StringBox::set_baseline(size_t base) {
  baseline = base;
}

StringBox v_amend(StringBox upper, StringBox lower) {
  size_t uw = upper.width(), lw = lower.width();
  if (lw < uw) {
    StringBox lower_wide(lower, StringBox(uw - lw, lower.height(), ' '), Horizontal, lower.get_baseline());
    return StringBox(upper, lower_wide, Vertical);
  }
  if (lw > uw) {
    StringBox upper_wide(upper, StringBox(lw - uw, upper.height(), ' '), Horizontal, upper.get_baseline());
    return StringBox(upper_wide, lower, Vertical);
  }
  return StringBox(upper, lower, Vertical);
}

StringBox h_amend(StringBox left, StringBox right) {
  Baseline base(fmap<size_t>(std::max<size_t>, left.get_baseline(), right.get_baseline())),
           lbase(left.get_baseline()),
           rbase(right.get_baseline());
  if (lbase && lbase < base) {
    left = StringBox(StringBox(left.width(), *base - *lbase, ' '), left, Vertical);
    left.set_baseline(*base);
  }
  if (rbase && rbase < base) {
    right = StringBox(StringBox(right.width(), *base - *rbase, ' '), right, Vertical);
    right.set_baseline(*base);
  }

  size_t lh = left.height(), rh = right.height();

  if (lh < rh) {
    left = StringBox(left, StringBox(left.width(), rh - lh, ' '), Vertical, left.get_baseline());
  } else if (lh > rh) {
    right = StringBox(right, StringBox(right.width(), lh - rh, ' '), Vertical, right.get_baseline());
  }
  return StringBox(left, right, Horizontal, base);
}

