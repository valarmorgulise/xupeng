#include "../xupeng/xupeng.h"

PENG::Logger::ptr g_logger = PENG_LOG_ROOT();

int sock = 0;

void test_fiber() {
  PENG_LOG_INFO(g_logger) << "test_fiber sock =  " << sock;

  sock = socket(AF_INET, SOCK_STREAM, 0);
  fcntl(sock, F_SETFL, O_NONBLOCK);

  sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(80);
  inet_pton(AF_INET, "180.101.50.188", &addr.sin_addr.s_addr);

  if (!connect(sock, (const sockaddr *)&addr, sizeof(addr))) {

  } else if (errno == EINPROGRESS) {
    PENG_LOG_INFO(g_logger)
        << "add event errno = " << errno << " " << strerror(errno);
    PENG::IOManager::GetThis()->addEvent(sock, PENG::IOManager::READ, []() {
      PENG_LOG_INFO(g_logger) << " read callback";
    });
    PENG::IOManager::GetThis()->addEvent(sock, PENG::IOManager::WRITE, []() {
      PENG_LOG_INFO(g_logger) << " write callback";
      // close(sock);
    });
  } else {
    PENG_LOG_INFO(g_logger) << "else " << errno << " " << strerror(errno);
  }
}

void test1() {
  std::cout << "EPOLLIN=" << EPOLLIN << " EPOLLOUT=" << EPOLLOUT << std::endl;
  PENG::IOManager iom(2, false);
  iom.schedule(&test_fiber);
}

PENG::Timer::ptr s_timer;
void test_timer() {
  PENG::IOManager iom(2);
  s_timer = iom.addTimer(
      1000,
      []() {
        static int i = 0;
        PENG_LOG_INFO(g_logger) << "hello timer i = " << i;
        if (++i == 3) {
          s_timer->reset(2000, true);
        }
      },
      true);
}

int main() {
  // test1();
  test_timer();
  return 0;
}
