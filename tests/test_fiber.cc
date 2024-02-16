#include "../xupeng/xupeng.h"

PENG::Logger::ptr g_logger = PENG_LOG_ROOT();

void run_in_fiber() {
  PENG_LOG_INFO(g_logger) << "run_in_fiber begin";
  // PENG::Fiber::GetThis()->swapOut();
  PENG::Fiber::YieldToHold();
  PENG_LOG_INFO(g_logger) << "run_in_fiber end";
  PENG::Fiber::YieldToHold();
}

void test_fiber() {
  PENG_LOG_INFO(g_logger) << "main begin -1";
  {
    PENG::Fiber::GetThis();
    PENG_LOG_INFO(g_logger) << "main begin";

    PENG::Fiber::ptr fiber(new PENG::Fiber(run_in_fiber));
    fiber->swapIn();
    PENG_LOG_INFO(g_logger) << "main after swapIn";
    fiber->swapIn();
    PENG_LOG_INFO(g_logger) << "main after end";
    fiber->swapIn();
  }
  PENG_LOG_INFO(g_logger) << "main after end2";
}

int main() {
  PENG::Thread::SetName("main");
  std::vector<PENG::Thread::ptr> thrs;
  for (int i = 0; i < 3; ++i) {
    thrs.push_back(PENG::Thread::ptr(
        new PENG::Thread(&test_fiber, "name_" + std::to_string(i))));
  }
  for (auto i : thrs) {
    i->join();
  }
  return 0;
}
