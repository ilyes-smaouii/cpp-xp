#include <iostream>
#include <utility>

class StrContainer {
public:
  StrContainer() = default;
  StrContainer(const std::string &some_str) : _some_str(some_str) {}
  StrContainer(std::string &&some_str) : _some_str(some_str) {}

  StrContainer(const StrContainer &other) {
    std::cout << "Constructor - copy semantics" << std::endl;
  }
  StrContainer(StrContainer &&other) {
    std::cout << "Constructor - move semantics" << std::endl;
  }

  StrContainer &operator=(const StrContainer &other) {
    std::cout << "Assignment - copy semantics" << std::endl;
    return *this;
  }
  StrContainer &operator=(StrContainer &&other) {
    std::cout << "Assignment - move semantics" << std::endl;
    return *this;
  }

protected:
  std::string _some_str{};
};

template <typename T>
void fun_eq(T a, T *b) {
  *b = a;
}

template <typename T>
void fun_move(T a, T *b) {
  *b = std::move(a);
}

template <typename T>
void fun_forward_noref(T a, T *b) {
  *b = std::forward<T>(a);
}

template <typename T>
void fun_forward_lref(T &a, T *b) {
  *b = std::forward<T &>(a);
}

template <typename T>
void fun_forward_rref(T &&a, T *b) {
  *b = std::forward<T>(a);
}

int main(int argc, char *argv[]) {
  StrContainer a{"a"}, b{"b"};

  fun_eq(a, &b);
  fun_eq(StrContainer{"temp_1"}, &b);
  fun_move(a, &b);
  fun_forward_noref(a, &b);
  fun_forward_noref(StrContainer{"Temp"}, &b);

  fun_forward_lref(a, &b);

  fun_forward_rref(StrContainer{"Temp2"}, &b);
  fun_forward_rref(std::move(a), &b);

  return 0;
}