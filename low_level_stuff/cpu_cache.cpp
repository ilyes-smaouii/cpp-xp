#include <array>
#include <atomic>
#include <cstddef>
#include <cstdint>

using byte_t = std::uint8_t;

template <std::size_t BUFFER_SIZE>
struct FIXED_SIZE_BUFFER {
  std::array<byte_t, BUFFER_SIZE> _data{};

  // TO-DO : add relevant methods
  void set_to_zero() { _data.fill(0); }
  constexpr static std::size_t get_buffer_size() { return BUFFER_SIZE; }
  byte_t *data() { return _data.data(); }
  const byte_t *data() const { return _data.data(); }
  template <typename DT>
  DT dataAs() {
    return reinterpret_cast<DT>(_data.data());
  }
  template <typename DT>
  const DT dataAs() const {
    return reinterpret_cast<DT>(_data.data());
  }
  template <typename DT>
  DT getNthBytePtrAs(std::size_t index) {
    return reinterpret_cast<DT>(this->data() + index);
  };
  // Also, is the second one really necessary, or is just redundant ? I'm not
  // sure
  template <typename DT>
  const DT getNthBytePtrAs(std::size_t index) const {
    return reinterpret_cast<const DT>(this->data() + index);
  };
};

struct SomeStruct {
  std::uint32_t price_a{};
  std::uint32_t id_a{};
  std::uint32_t price_b{};
  std::uint32_t id_b{};
};

void fun_1(SomeStruct &data) {
  std::uint64_t to_add = 1 << 2;
  for (std::uint16_t i{1}; i != 0; i++) {
    data.price_a += data.id_a;
    data.price_a <<= 2;
    data.price_a += to_add;
    data.price_a >>= 2;
    data.price_a -= data.id_a;
  }
}

void fun_2(SomeStruct &data) {
  std::uint64_t to_add = 1 << 2;
  for (std::uint16_t i{1}; i != 0; i++) {
    data.price_a += data.id_b;
    data.price_a <<= 2;
    data.price_a += to_add;
    data.price_a >>= 2;
    data.price_a -= data.id_b;
  }
}

int main(int argc, char *argv[]) {
  // auto new_buf = FIXED_SIZE_BUFFER<256>();
  // new_buf.set_to_zero();
  // auto ui_1 = new_buf.getNthBytePtrAs<std::uint32_t *>(0);
  // auto ui_2 = new_buf.getNthBytePtrAs<std::uint32_t *>(0);
  SomeStruct data{2, 3, 5, 7};

  for (std::size_t i{0}; i < (1<<1); i++) {
    fun_1(data);
  }
}