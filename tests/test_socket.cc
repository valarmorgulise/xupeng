#include "../xupeng/xupeng.h"

static PENG::Logger::ptr g_logger = PENG_LOG_ROOT();

void test_socket() {
  PENG::IPAddress::ptr addr =
      PENG::Address::LookupAnyIPAddress("www.baidu.com");
  if (addr) {
    PENG_LOG_INFO(g_logger) << "get address : " << addr->toString();
  } else {
    PENG_LOG_ERROR(g_logger) << "get address fail";
    return;
  }

  PENG::Socket::ptr sock = PENG::Socket::CreateTCP(addr);
  addr->setPort(80);
  PENG_LOG_INFO(g_logger) << "addr = " << addr->toString();
  if (!sock->connect(addr)) {
    PENG_LOG_ERROR(g_logger) << "connect " << addr->toString() << " fail";
    return;
  } else {
    PENG_LOG_INFO(g_logger) << "connect " << addr->toString() << " connected";
  }

  const char buff[] = "GET / HTTP/1.0\r\n\r\n";
  int rt = sock->send(buff, sizeof(buff));
  if (rt <= 0) {
    PENG_LOG_INFO(g_logger) << "send fail rt = " << rt;
    return;
  }
  std::string buffs;
  buffs.resize(4096);
  rt = sock->recv(&buffs[0], buffs.size());

  if (rt <= 0) {
    PENG_LOG_INFO(g_logger) << "recv fail rt = " << rt;
    return;
  }
  buffs.resize(rt);
  PENG_LOG_INFO(g_logger) << buffs;
}

int main() {
  test_socket();
  return 0;
}
