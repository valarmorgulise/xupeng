#include "../xupeng/log.h"
#include "../xupeng/util.h"
#include <iostream>

int main(int argc, char *argv[]) {
  PENG::Logger::ptr logger(new PENG::Logger);
  logger->addAppender(PENG::LogAppender::ptr(new PENG::StdoutLogAppender));

  PENG::FileLogAppender::ptr file_appender(
      new PENG::FileLogAppender("./log.txt"));

  PENG::LogFormatter::ptr fmt(new PENG::LogFormatter("%d%T%m%n"));
  file_appender->setFormatter(fmt);

  // PENG::LogEvent::ptr event(
  // new PENG::LogEvent(__FILE__, __LINE__, 0, PENG::GetThreadId(),
  // PENG::GetFiberId(), time(0));
  // logger->log(PENG::LogLevel::DEBUG, event);
  std::cout << "hello peng log" << std::endl;
  PENG_LOG_INFO(logger) << "test macro";
  PENG_LOG_ERROR(logger) << "test macro error";
  PENG_LOG_FMT_ERROR(logger, "test macro fmt error %s", "aa");
  return 0;
}
