#include "../xupeng/xupeng.h"
#include <yaml-cpp/node/parse.h>

PENG::Logger::ptr g_logger = PENG_LOG_ROOT();

int count = 0;
PENG::RWMutex s_mutex;

void fun1() {
  PENG_LOG_INFO(g_logger) << "name: " << PENG::Thread::GetName()
                          << " this.name: "
                          << PENG::Thread::GetThis()->getName()
                          << " id: " << PENG::GetThreadId()
                          << " this.id: " << PENG::Thread::GetThis()->getId();
  // sleep(20);
  for (int i = 0; i < 10000; ++i) {
    PENG::RWMutex::ReadLock lock(s_mutex);
    count++;
  }
}

void fun2() {
  while (true) {
    PENG_LOG_INFO(g_logger)
        << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
  }
}

void fun3() {
  while (true) {
    PENG_LOG_INFO(g_logger)
        << "================================================";
  }
}

int main() {
  PENG_LOG_INFO(g_logger) << "thread test begin";
  YAML::Node root =
      YAML::LoadFile("/home/xupeng/valarmorgulis/xupeng/bin/conf/log2.yml");
  PENG::Config::LoadFromYaml(root);

  std::vector<PENG::Thread::ptr> thrs;
  // for (int i = 0; i < 5; ++i) {
  //   PENG::Thread::ptr thr(new PENG::Thread(&fun1, "name_" +
  //   std::to_string(i))); thrs.push_back(thr);
  // }
  // for (int i = 0; i < 5; ++i) {
  //   thrs[i]->join();
  // }
  for (int i = 0; i < 2; ++i) {
    PENG::Thread::ptr thr(
        new PENG::Thread(&fun1, "name_" + std::to_string(i * 2)));
    PENG::Thread::ptr thr2(
        new PENG::Thread(&fun2, "name_" + std::to_string(i * 2 + 1)));
    thrs.push_back(thr);
    thrs.push_back(thr2);
  }

  for (size_t i = 0; i < thrs.size(); ++i) {
    thrs[i]->join();
  }
  PENG_LOG_INFO(g_logger) << "thread test end";
  PENG_LOG_INFO(g_logger) << "count = " << count;
  return 0;
}
