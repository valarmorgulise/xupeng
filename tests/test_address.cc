#include "../xupeng/xupeng.h"

PENG::Logger::ptr g_logger = PENG_LOG_ROOT();

void test() {
  std::vector<PENG::Address::ptr> addrs;

  PENG_LOG_INFO(g_logger) << "begin";

  bool v = PENG::Address::Lookup(addrs, "www.baidu.com");

  PENG_LOG_INFO(g_logger) << "end";
  if (!v) {
    PENG_LOG_ERROR(g_logger) << "lookup failed!";
    return;
  }
  for (size_t i = 0; i < addrs.size(); ++i) {
    PENG_LOG_INFO(g_logger) << i << " - " << addrs[i]->toString();
  }
}

int main() {
  test();
  return 0;
}
