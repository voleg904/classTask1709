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

value_t sum_thread(const value_t* data, size_t starе, size_t block)
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

    
  }
  return 0;
}
