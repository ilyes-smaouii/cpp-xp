#include <iostream>

bool exists(int ints[], int size, int k) {
  int *beg_ptr{ints}, *end_ptr{ints + size}, *mid_ptr;
  do {
    mid_ptr = beg_ptr + (end_ptr - beg_ptr) / 2;
    if ((*mid_ptr) == k) {
      return true;
    } else if (*mid_ptr > k) {
      end_ptr = mid_ptr;
    } else {
      beg_ptr = mid_ptr + 1;
    }
  } while (end_ptr - beg_ptr > 0);
  return false;
  return ((*beg_ptr) == k || (*end_ptr) == k);
}

int main(int argc, char *argv[]) {
  int ints[] = {-4, 4, 7, 20, 109};
  std::cout << exists(ints, 5, 109) << std::endl;
  std::cout << exists(ints, 5, 22) << std::endl;
  std::cout << exists(ints, 5, 110) << std::endl;
  std::cout << exists(ints, 5, 4) << std::endl;
}