#include <array>
#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <new>
#include <vector>

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
  alignas(std::hardware_destructive_interference_size) std::uint32_t price_a{};
  std::uint32_t id_a{};
  // std::uint64_t padding[7]{3, 4, 5, 2, 3, 1, 9};
  alignas(std::hardware_destructive_interference_size) std::uint32_t price_b{};
  std::uint32_t id_b{};
};
static_assert(offsetof(SomeStruct, price_b) ==
                  std::hardware_destructive_interference_size,
              "");

constexpr auto L1_CACHE_SIZE =
    80 * (1 << 10); // Size of L1 on my machine, apparently
constexpr auto L2_CACHE_SIZE =
    2 * (1 << 20); // Size of L2 on my machine, apparently
constexpr auto L3_CACHE_SIZE =
    24 * (1 << 20); // Size of L2 on my machine, apparently

std::array<std::uint64_t, 10 * L3_CACHE_SIZE / sizeof(std::uint64_t)>
    big_array; // Should be two time the size of L1 cache

/* constexpr std::size_t N{0};

// Array of Structures
struct Order {
  int id;
  double price;
  int qty;
};
Order orders[N];

// versus

// Structure of Arrays
int ids[N];
double prices[N];
int qtys[N]; */

volatile std::uint32_t sink;

constexpr std::uint32_t MAGIC_32{0xcafebabe};
constexpr std::uint64_t MAGIC_64{0xcafebabedeadbeef};

void fun_1(SomeStruct &data, std::uint64_t iter_count) {
  constexpr std::uint64_t to_add = 1 << 2;
  for (std::uint64_t i{0}; i < iter_count; i++) {
    data.price_a += data.id_a;
    data.price_a <<= 2;
    data.price_a += to_add;
    data.price_a ^= MAGIC_32 * data.price_a;
    data.price_a >>= 2;
    data.price_a -= data.id_a;
  }
  sink = data.price_a;
}

void fun_2(SomeStruct &data, std::uint64_t iter_count) {
  constexpr std::uint64_t to_add = 1 << 2;
  for (std::uint64_t i{0}; i < iter_count; i++) {
    data.price_a += data.id_b;
    data.price_a <<= 2;
    data.price_a += to_add;
    data.price_a ^= MAGIC_32 * data.price_a;
    data.price_a >>= 2;
    data.price_a -= data.id_b;
  }
  sink = data.price_a;
}

void fun3(std::uint64_t iter_count) {
  std::uint64_t i{0};
  constexpr std::size_t SMALL_POS{L1_CACHE_SIZE / sizeof(std::uint64_t) / 2};
  for (; i < iter_count;) {
    for (std::uint64_t j{0};
         j < (L1_CACHE_SIZE / sizeof(std::uint64_t)) / 2 && i < iter_count;
         j++) {
      big_array[j] += ++big_array[SMALL_POS + j];
      big_array[j] <<= 2;
      big_array[j] ^= MAGIC_64 * big_array[0];
      big_array[j] >>= 2;
      sink = big_array[j];
      i++;
    }
  }
  std::cout << "fun3 - Executed " << i << " iterations" << std::endl;
  sink = big_array[0];
}

void fun4(std::uint64_t iter_count) {
  std::uint64_t i{0};
  constexpr std::size_t BIG_POS{4 * (L3_CACHE_SIZE / sizeof(std::uint64_t))};
  for (; i < iter_count;) {
    for (std::uint64_t j{0};
         //  j < 2 * L3_CACHE_SIZE / sizeof(std::uint64_t) && i < iter_count;
         //  j++) {
         j < (L1_CACHE_SIZE / sizeof(std::uint64_t)) / 2 && i < iter_count;
         j++) {
      big_array[j] += ++big_array[BIG_POS + j];
      big_array[j] <<= 2;
      big_array[j] ^= MAGIC_64 * big_array[0];
      big_array[j] >>= 2;
      sink = big_array[j];
      i++;
    }
  }
  std::cout << "fun4 - Executed " << i << " iterations" << std::endl;
  sink = big_array.at(0);
}

int main(int argc, char *argv[]) {
  // auto new_buf = FIXED_SIZE_BUFFER<256>();
  // new_buf.set_to_zero();
  // auto ui_1 = new_buf.getNthBytePtrAs<std::uint32_t *>(0);
  // auto ui_2 = new_buf.getNthBytePtrAs<std::uint32_t *>(0);
  SomeStruct data{2, 3, 5, 3};

  std::uint64_t iter_count = static_cast<std::uint64_t>(1) << 16;

  auto start_fun1 = std::chrono::high_resolution_clock::now();
  for (std::size_t i{0}; i < (1 << 1); i++) {
    // __builtin_prefetch(&data);
    fun_1(data, iter_count);
  }
  auto end_fun1 = std::chrono::high_resolution_clock::now();
  auto start_fun2 = std::chrono::high_resolution_clock::now();
  for (std::size_t i{0}; i < (1 << 1); i++) {
    // __builtin_prefetch(&data);
    fun_2(data, iter_count);
  }
  auto end_fun2 = std::chrono::high_resolution_clock::now();

  auto exec_time_fun1 = (end_fun1 - start_fun1);
  auto exec_time_fun2 = (end_fun2 - start_fun2);

  std::cout << "fun1 exec time : " << exec_time_fun1 << std::endl;
  std::cout << "fun2 exec time : " << exec_time_fun2 << std::endl;

  for (auto &cell : big_array) {
    cell =
        std::chrono::high_resolution_clock::now().time_since_epoch().count() ^
        MAGIC_64;
  }
  std::cout << "big_array filled" << std::endl;

  iter_count = static_cast<std::uint64_t>(1) << 26;

  auto start_fun3 = std::chrono::high_resolution_clock::now();
  fun3(iter_count);
  auto end_fun3 = std::chrono::high_resolution_clock::now();
  auto start_fun4 = std::chrono::high_resolution_clock::now();
  fun4(iter_count);
  auto end_fun4 = std::chrono::high_resolution_clock::now();

  auto exec_time_fun3 =
      static_cast<std::chrono::nanoseconds>(end_fun3 - start_fun3);
  auto exec_time_fun4 =
      static_cast<std::chrono::nanoseconds>(end_fun4 - start_fun4);

  std::cout << "fun3 exec time : " << exec_time_fun3.count() << "ns"
            << std::endl;
  std::cout << "fun4 exec time : " << exec_time_fun4.count() << "ns"
            << std::endl;
}