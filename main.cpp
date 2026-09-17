#include <iostream>
#include <chrono>
#include <vector>
#include <future>

class Clicker
{
  public:
    Clicker():
      start_(std::chrono::high_resolution_clock::now())
    {}
    double millisec() const
    {
      using std::chrono::high_resolution_clock;
      using std::chrono::duration_cast;
      using std::chrono::milliseconds;
      auto t = high_resolution_clock::now();
      return duration_cast< milliseconds >(t - start_).count();
    }
  private:
    std::chrono::time_point< std::chrono::high_resolution_clock > start_;
};

using data_t = std::vector< unsigned long long >;
using value_t = data_t::value_type;

value_t sum_thread(const value_t* data, size_t start, size_t block)
{
  value_t sum = 0;
  for (size_t i = 0; i < block; ++i)
  {
    sum += data[start + i];
  }
  return sum;
}

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    return 1;
  }
  const size_t threads = std::stoull(argv[1]);
  constexpr size_t size{1'000'000'000};
  double init{0}, total{0};
  value_t sum{0};
  {
    Clicker cl;
    data_t values(size, 1);
    init = cl.millisec();

    std::vector<std::future<value_t>> futures;
    futures.reserve(threads);

    const size_t blockSize = size / threads;
    const size_t remains = size % threads;
    const value_t* data = values.data();

    for (size_t i = 0; i + 1 < threads; ++i)
    {
      size_t start = i * blockSize;
      futures.push_back(std::async(std::launch::async, sum_thread, data, start, blockSize));
    }
    size_t lastStart = (threads - 1) * blockSize;
    size_t lastSize = blockSize + remains;
    futures.push_back(std::async(std::launch::async, sum_thread, data, lastStart, lastSize));

    for (size_t i = 0; i < threads; ++i)
    {
      sum += futures[i].get();
    }

    total = cl.millisec();
  }
  std::cout << total - init << "\n";
  return 0;
}
