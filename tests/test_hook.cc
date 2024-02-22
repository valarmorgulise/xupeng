#include "../xupeng/xupeng.h"

PENG::Logger::ptr g_logger = PENG_LOG_ROOT();

void test_sleep() {
  PENG::IOManager iom(1);
  iom.schedule([]() {
    sleep(2);
    PENG_LOG_INFO(g_logger) << "sleep 2";
  });

  iom.schedule([]() {
    sleep(3);
    PENG_LOG_INFO(g_logger) << "sleep 3";
  });
  PENG_LOG_INFO(g_logger) << "test_sleep";
}

int main() {
  test_sleep();
  return 0;
}
