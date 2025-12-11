#include <atomic>
#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>
#include <vector>

std::size_t counter{0};
std::atomic_size_t atomic_counter{0};

struct Counter {
  std::atomic_size_t counter_a{};
  std::atomic_size_t counter_b{};
};

struct CounterAligned {
  alignas(64) std::atomic_size_t counter_a{};
  alignas(64) std::atomic_size_t counter_b{};
};

void countFun_incr(std::size_t count, std::size_t &counter,
                   std::atomic_size_t &atomic_counter) {
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  for (std::size_t i{0}; i < count; i++) {
    counter++;
    atomic_counter++;
  }
}

void countFunPlusEq(std::size_t count, std::size_t &counter,
                    std::atomic_size_t &atomic_counter) {
  std::this_thread::sleep_for(std::chrono::milliseconds(50));
  for (std::size_t i{0}; i < count; i++) {
    counter += 1;
    atomic_counter.fetch_add(1, std::memory_order_relaxed);
  }
}

int main(int argc, char *argv[]) {
  std::vector<std::jthread> my_threads{};
  for (std::size_t i{0}; i < 100; i++) {
    my_threads.emplace_back(countFun_incr, 100000, std::ref(counter),
                            std::ref(atomic_counter));
  }
  for (auto &thread : my_threads) {
    thread.join();
  }
  std::cout.imbue(std::locale(""));
  std::cout << "Final value for counter : " << counter << std::endl;
  std::cout << "Final value for atomic_counter : " << atomic_counter
            << std::endl;

  counter = atomic_counter = 0;
  my_threads.clear();

  for (std::size_t i{0}; i < 100; i++) {
    my_threads.emplace_back(countFunPlusEq, 100000, std::ref(counter),
                            std::ref(atomic_counter));
  }
  for (auto &thread : my_threads) {
    thread.join();
  }
  std::cout.imbue(std::locale(""));
  std::cout << "Final value for counter : " << counter << std::endl;
  std::cout << "Final value for atomic_counter : " << atomic_counter
            << std::endl;

  counter = atomic_counter = 0;
}