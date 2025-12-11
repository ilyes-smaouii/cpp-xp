#include <iostream>

int main(int argc, char *argv[]) {
  int a = 10;
  int &b = a;
  int &c = b;
  int &d = c;
  std::cout << "a : " << a << std::endl;
  d++;
  std::cout << "a : " << a << std::endl;
  ; //
}