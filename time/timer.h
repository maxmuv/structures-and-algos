#ifndef TIMER_H
#define TIMER_H
#include <chrono>

class Timer {
 private:
  using clock_t = std::chrono::high_resolution_clock;
  using microsecond_t = std::chrono::duration<double, std::ratio<1, 1000000> >;

  std::chrono::time_point<clock_t> m_beg;

 public:
  Timer() : m_beg(clock_t::now()) {}

  void reset() { m_beg = clock_t::now(); }

  double elapsed() const {
    return std::chrono::duration_cast<microsecond_t>(clock_t::now() - m_beg)
        .count();
  }
};

#endif
