#pragma once
#include <deque>
#include <vector>
#include <chrono>
#include <mutex>

class Batcher
{
public:
  explicit Batcher(int n)
  {
    for(int i=0; i < n ; ++i)
      d_store.push_back(i);

    randomize();
  }

  explicit Batcher(const std::vector<int>& in)
  {
    for(const auto& i : in)
      d_store.push_back(i);
    randomize();
  }

  auto getBatch(int n)
  {
    std::deque<int> ret;
    for(int i = 0 ; !d_store.empty() && i < n; ++i) {
      ret.push_back(d_store.front());
      d_store.pop_front();
    }
    return ret;
  }

  auto getBatchLocked(int n)
  {
    std::deque<int> ret;
    std::lock_guard<std::mutex> l(d_mut);
    for(int i = 0 ; !d_store.empty() && i < n; ++i) {
      ret.push_back(d_store.front());
      d_store.pop_front();
    }
    return ret;
  }

private:
  std::deque<int> d_store;
  std::mutex d_mut;
  void randomize()
  {
    std::random_device rd;
    std::mt19937 g(rd());
 
    std::shuffle(d_store.begin(), d_store.end(), g);
  }

};

template<typename T>
void printImg(const T& img)
{
  for(unsigned int y=0; y < img.getRows(); ++y) {
    for(unsigned int x=0; x < img.getCols(); ++x) {
      float val = img(y,x).getVal();
      if(val > 0.5)
        std::cout<<'X';
      else if(val > 0.25)
        std::cout<<'*';
      else if(val > 0.125)
        std::cout<<'.';
      else
        std::cout<<' ';
    }
    std::cout<<'\n';
  }
  std::cout<<"\n";
}

struct DTime
{
  void start()
  {
    d_start =   std::chrono::steady_clock::now();
  }
  uint32_t lapUsec()
  {
    auto usec = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now()- d_start).count();
    start();
    return usec;
  }

  std::chrono::time_point<std::chrono::steady_clock> d_start;
};
