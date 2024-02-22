#include "../xupeng/xupeng.h"

static PENG::Logger::ptr g_logger = PENG_LOG_ROOT();

void test_fiber() {
  static int s_count = 5;
  PENG_LOG_INFO(g_logger) << "test in fiber s_count=" << s_count;

  sleep(1);
  if (--s_count >= 0) {
    PENG::Scheduler::GetThis()->schedule(&test_fiber, PENG::GetThreadId());
  }
}

int main(int argc, char **argv) {
  PENG_LOG_INFO(g_logger) << "main";
  PENG::Scheduler sc(3, false, "test");
  sc.start();
  sleep(2);
  PENG_LOG_INFO(g_logger) << "schedule";
  sc.schedule(&test_fiber);
  sc.stop();
  PENG_LOG_INFO(g_logger) << "over";
  return 0;
}
