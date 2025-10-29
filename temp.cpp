#include <iostream>
#include <string>

int main() {
  std::string str = "-12345";
  int value = 0;
  bool isNegative = false;
  size_t startIndex = 0;

  // Check if the number is negative
  if (str[0] == '-') {
    isNegative = true;
    startIndex = 1; // Start conversion from the second character
  }

  // Accumulate the numeric value
  for (size_t i = startIndex; i < str.length(); ++i) {
    char c = str[i];
    if (c < '0' || c > '9') {
      std::cerr << "Invalid character encountered: " << c << std::endl;
      return 1;
    }
    int digit = c - '0';
    value = value * 10 + digit;
  }

  if (isNegative) {
    value = -value;
  }

  std::cout << "Converted value: " << value << std::endl;
  return 0;
}