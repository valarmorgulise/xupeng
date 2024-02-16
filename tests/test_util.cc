#include "../xupeng/xupeng.h"
#include <assert.h>

PENG::Logger::ptr g_logger = PENG_LOG_ROOT();

void test_assert() {
  // assert(1);
  PENG_LOG_INFO(g_logger) << PENG::BacktraceToString(10);
  // PENG_ASSERT(false);
  PENG_ASSERT2(0 == 1, "abcdef xx");
}

int main() {
  test_assert();
  return 0;
}
