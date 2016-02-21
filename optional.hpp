#ifndef __OPTIONAL_HPP_
#define __OPTIONAL_HPP_

#include <memory>
#include <functional>

template<typename T>
class Optional {
  public:
    Optional();
    Optional(T x);
    Optional(std::nullptr_t ptr);
    operator bool() const;
    T operator*() const;
  private:
    std::shared_ptr<T> ptr;
};

template<typename T>
Optional<T>::Optional():
  ptr()
{}

template<typename T>
Optional<T>::Optional(T x):
  ptr(new T(x))
{}

template<typename T>
Optional<T>::Optional(std::nullptr_t x):
  ptr()
{}

template<typename T>
Optional<T>::operator bool() const {
  return static_cast<bool>(ptr);
}

template<typename T>
T Optional<T>::operator *() const {
  return *ptr;
}

template<typename T, typename U>
Optional<U> fmap(std::function<U(T)> f, Optional<T> x) {
  if (!x) {
    return Optional<U>();
  }
  return f(*x);
}

template<typename T>
Optional<T> fmap(std::function<T(T, T)> f, Optional<T> a, Optional<T> b) {
  if (!a) { return b; }
  if (!b) { return a; }
  return f(*a, *b);
}

template<typename T>
Optional<T> optional(T x) {
  return T(x);
}

template<typename T>
bool operator<(Optional<T> x, Optional<T> y) {
  return !x || (y && *x < *y);
}

template<typename T>
bool operator==(Optional<T> x, Optional<T> y) {
  return (!x && !y) || (x && y && *x == *y);
}

template<typename T>
bool operator>(Optional<T> x, Optional<T> y) { return y < x; }
template<typename T>
bool operator<=(Optional<T> x, Optional<T> y) { return !(x > y); }
template<typename T>
bool operator>=(Optional<T> x, Optional<T> y) { return !(x < y); }
template<typename T>
bool operator!=(Optional<T> x, Optional<T> y) { return !(x == y); }

#endif // __OPTIONAL_HPP_
