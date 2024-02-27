#include "../xupeng/xupeng.h"
#include <cstdint>

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

  auto addr = PENG::Address::LookupAny("localhost:4080");
  if (addr) {
    PENG_LOG_INFO(g_logger) << *addr;
  } else {
    PENG_LOG_ERROR(g_logger) << "error";
  }
}

void test_iface() {
  std::multimap<std::string, std::pair<PENG::Address::ptr, uint32_t>> results;
  bool v = PENG::Address::GetInterfaceAddresses(results);
  if (!v) {
    PENG_LOG_ERROR(g_logger) << "GetInterfaceAddresses fail";
    return;
  }
  for (auto &i : results) {
    PENG_LOG_INFO(g_logger) << i.first << " - " << i.second.first->toString()
                            << " - " << i.second.second;
  }
}

void test_ipv4() {
  auto addr = PENG::IPAddress::Create("127.0.0.8");
  if (addr) {
    PENG_LOG_INFO(g_logger) << addr->toString();
  }
}

int main() {

  test();
  // test_iface();
  // test_ipv4();
  return 0;
}
