#pragma once
#include <string>
#include <vector>
#include <memory>

using Direction = int;
const Direction Vertical = 1;
const Direction Horizontal = 2;

class StringBox {
  public:
    StringBox(const std::string &str);
    StringBox(const StringBox &fst, const StringBox &snd, Direction dir);
    const std::string to_string() const;
    const char at(size_t i, size_t j) const;
    const std::string row(size_t i) const;
    const bool empty() const;
    const size_t width() const;
    const size_t height() const;
  private:
    Direction connection;
    std::vector<std::string> entity;
    std::shared_ptr<StringBox> first;
    std::shared_ptr<StringBox> second;
};

StringBox v_amend(const StringBox &upper, const StringBox &lower);
StringBox h_amend(const StringBox &left, const StringBox &right);
