#pragma once
#include <string>
#include <vector>
#include <memory>
#include "optional.hpp"

using Direction = int;
const Direction Vertical = 1;
const Direction Horizontal = 2;

using Baseline = Optional<size_t>;

class StringBox {
  public:
    StringBox(const std::string &str);
    StringBox(size_t width, size_t height, char c);
    StringBox(const StringBox &fst, const StringBox &snd, Direction dir, Baseline base = nullptr);
    const std::string to_string() const;
    const char at(size_t i, size_t j) const;
    const std::string row(size_t i) const;
    const bool empty() const;
    const size_t width() const;
    const size_t height() const;
    Baseline get_baseline() const;
    void set_baseline(size_t base);
  private:
    Direction connection;
    std::vector<std::string> entity;
    std::shared_ptr<StringBox> first;
    std::shared_ptr<StringBox> second;
    Baseline baseline;
};

StringBox v_amend(StringBox upper, StringBox lower);
StringBox h_amend(StringBox left, StringBox right);
