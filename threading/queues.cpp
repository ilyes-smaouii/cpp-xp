#include <cstddef>
#include <iostream>
#include <memory>

template <typename T>
class RingBuffer {
public:
  RingBuffer(std::size_t capacity)
      : _size(0), _capacity(capacity), _head(0), _tail(0) {
    _data = std::make_shared(capacity);
  }

  bool push(const T &new_data);
  T pop();
  std::size_t size() const { return (_capacity + _tail - _head) % _capacity; }
  bool reset();

protected:
  std::size_t _size{};
  std::size_t _capacity{};
  std::size_t _head{}, _tail{};
  std::shared_ptr<T[]> _data{};
};

int main(int argc, char *argv[]) {
  ; //
}