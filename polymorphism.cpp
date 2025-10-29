#include <iostream>

class A {
  public:
  // void test() { std::cout << "A" << std::endl; }
  virtual void test() { std::cout << "A" << std::endl; }
};

class B : public A {
public:
  void test() { std::cout << "B" << std::endl; }
};

int main(int argc, char *argv[]) {
  A *p = new B(); //
  p->test();
}