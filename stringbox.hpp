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
    const std::string to_string();
    const char at(size_t i, size_t j);
    const std::string row(size_t i);
    const bool empty();
    const size_t width();
    const size_t height();
  private:
    Direction connection;
    std::vector<std::string> entity;
    std::shared_ptr<StringBox> first;
    std::shared_ptr<StringBox> second;
};

