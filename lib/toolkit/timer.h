// decl
#ifndef TEMPLATE_CPP_TIMER
#define TEMPLATE_CPP_TIMER 1
#ifndef NO_TEMPLATE_IMPORT
#include <functional>
using std::function;
#endif
// impl
#include <chrono>
namespace toolkit {
struct Timer {
  std::chrono::system_clock::time_point tick;
  long double final_duration;
  Timer() { tick = std::chrono::system_clock::now(); }
  inline void start() { tick = std::chrono::system_clock::now(); }
  inline void stop() { final_duration = (std::chrono::system_clock::now() - tick).count() / 1e6; }
  // get() ms
  inline long double get() { return final_duration; }
  // test a function with no parameter and return value
  inline void test(function<void()> f, unsigned int calls = 1) {
    start(); for (; calls; -- calls) f(); stop();
  }
  Timer(function<void()> f, unsigned int calls = 1) {
    test(f, calls);
  }
};
}
using toolkit::Timer;
#endif