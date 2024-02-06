#ifndef __PENG_LOG_H
#define __PENG_LOG_H

#include "singleton.h"
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stdarg.h>
#include <string.h>
#include <vector>

#define PENG_LOG_LEVEL(logger, level)                                          \
  if (logger->getLevel() <= level)                                             \
    PENG::LogEventWrap(                                                        \
        PENG::LogEvent::ptr(new PENG::LogEvent(                                \
            logger, level, __FILE__, __LINE__, 0, PENG::getThreadId(),         \
            PENG::getFiberId(), time(0), PENG::Thread::GetName())))            \
        .getSS();

#define PENG_LOG_DEBUG(logger) PENG_LOG_LEVEL(logger, PENG::LogLevel::DEBUG)
#define PENG_LOG_WARN(logger) PENG_LOG_LEVEL(logger, PENG::LogLevel::WARN)
#define PENG_LOG_ERROR(logger) PENG_LOG_LEVEL(logger, PENG::LogLevel::ERROR)
#define PENG_LOG_FATAL(logger) PENG_LOG_LEVEL(logger, PENG::LogLevel::FATAL)
#define PENG_LOG_INFO(logger) PENG_LOG_LEVEL(logger, PENG::LogLevel::INFO)

#define PENG_LOG_FMT_LEVEL(logger, level, fmt, ...)                            \
  if (logger->getLevel() <= level)                                             \
  PENG::LogEventWrap(                                                          \
      PENG::LogEvent::ptr(new PENG::LogEvent(                                  \
          logger, level, __FILE__, __LINE__, 0, PENG::getThreadId(),           \
          PENG::getFiberId(), time(0), PENG::Thread::GetName())))              \
      .getEvent()                                                              \
      ->format(fmt, __VA_ARGS__)

#define PENG_LOG_FMT_DEBUG(logger, fmt, ...)                                   \
  PENG_LOG_FMT_LEVEL(logger, PENG::LogEvent::DEBUG, fmt, __VA_ARGS__)

#define PENG_LOG_FMT_INFO(logger, fmt, ...)                                    \
  PENG_LOG_FMT_LEVEL(logger, PENG::LogEvent::INFO, fmt, __VA_ARGS__)

#define PENG_LOG_FMT_WARN(logger, fmt, ...)                                    \
  PENG_LOG_FMT_LEVEL(logger, PENG::LogEvent::WARN, fmt, __VA_ARGS__)

#define PENG_LOG_FMT_ERROR(logger, fmt, ...)                                   \
  PENG_LOG_FMT_LEVEL(logger, PENG::LogEvent::ERROR, fmt, __VA_ARGS__)

#define PENG_LOG_FMT_FATAL(logger, fmt, ...)                                   \
  PENG_LOG_FMT_LEVEL(logger, PENG::LogEvent::FATAL, fmt, __VA_ARGS__)

#define PENG_LOG_ROOT() PENG::LoggerMgr::getInstance()->getRoot()

#define PENG_LOG_NAME(name) PENG::LoggerMgr::getInstance()->getLogger(name)

namespace PENG {
class Logger;
class LoggerManager;

class LogLevel {
public:
  enum Level {
    UNKNOW = 0,
    DEBUG = 1,
    INFO = 2,
    WARN = 3,
    ERROR = 4,
    FATAL = 5
  };
  static const char *ToString(LogLevel::Level level);
  static LogLevel::Level FromString(const std::string &str);
};

class LogEvent {
public:
  typedef std::shared_ptr<LogEvent> ptr;
  LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
           const char *file, int32_t line, uint32_t elapse, uint32_t thread_id,
           uint32_t fiber_id, uint64_t time, const std::string &thread_name);
  const char *getFile() const { return m_file; }
  int32_t getLine() const { return m_line; }
  uint32_t getElapse() const { return m_elapse; }
  uint32_t getThreadId() const { return m_threadId; }
  uint32_t getFiberId() const { return m_fiberId; }
  uint64_t getTime() const { return m_time; }
  const std::string &getThreadName() const { return m_threadName; }
  std::string getContent() const { return m_ss.str(); }
  std::shared_ptr<Logger> getLogger() const { return m_logger; }
  LogLevel::Level getLevel() const { return m_level; }
  std::stringstream &getSS() { return m_ss; }
  void format(const char *fmt, ...);
  void format(const char *fmt, va_list al);

private:
  const char *m_file = nullptr;
  int32_t m_line = 0;
  uint32_t m_elapse = 0;
  uint32_t m_threadId = 0;
  uint32_t m_fiberId = 0;
  uint64_t m_time = 0;
  std::string m_threadName;
  std::stringstream m_ss;
  std::shared_ptr<Logger> m_logger;
  LogLevel::Level m_level;
};

class LogEventWrap {
public:
  LogEventWrap(LogEvent::ptr e);
  ~LogEventWrap();
  LogEvent::ptr getEvent() const { return m_event; }
  std::stringstream &getSS();

private:
  LogEvent::ptr m_event;
};

class LogFormatter {
public:
  typedef std::shared_ptr<LogFormatter> ptr;
  LogFormatter(const std::string &pattern);
  std::string format(std::shared_ptr<Logger> logger, LogLevel::Level level,
                     LogEvent::ptr event);
  std::ostream &format(std::ostream &ofs, std::shared_ptr<Logger> logger,
                       LogLevel::Level level, LogEvent::ptr event);

public:
  class FormatItem {
  public:
    typedef std::shared_ptr<FormatItem> ptr;
    virtual ~FormatItem() {}
    virtual void format(std::ostream &os, std::shared_ptr<Logger> logger,
                        LogLevel::Level level, LogEvent::ptr event) = 0;
  };

  void init();
  bool isError() const { return m_error; }
  const std::string getPattern() const { return m_pattern; }

private:
  std::string m_pattern;
  std::vector<FormatItem::ptr> m_items;
  bool m_error = false;
};

class LogAppender {
  friend class Logger;

public:
  typedef std::shared_ptr<LogAppender> ptr;
  virtual ~LogAppender() {}
  virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level,
                   LogEvent::ptr event) = 0;
  virtual std::string toYamlString() = 0;
  void setFormatter(LogFormatter::ptr val);
  LogFormatter::ptr getFormatter();
  LogLevel::Level getLevel() const { return m_level; }
  void setLevel(LogLevel::Level level) { m_level = level; }

protected:
  LogLevel::Level m_level = LogLevel::DEBUG;
  bool m_hasFormatter = false;
  LogFormatter::ptr m_formatter;
};

class Logger : public std::enable_shared_from_this<Logger> {
  friend class LoggerManager;

public:
  typedef std::shared_ptr<Logger> ptr;
  Logger(const std::string &name = "root");
  void log(LogLevel::Level level, LogEvent::ptr event);
  void debug(LogEvent::ptr event);
  void info(LogEvent::ptr event);
  void warn(LogEvent::ptr event);
  void fatal(LogEvent::ptr event);
  void error(LogEvent::ptr event);
  void addAppender(LogAppender::ptr appender);
  void delAppender(LogAppender::ptr appender);
  void clearAppender();
  LogLevel::Level getLevel() { return m_level; }
  void setLevel(LogLevel::Level val) { m_level = val; }
  const std::string &getName() const { return m_name; }
  void setFormatter(LogFormatter::ptr val);
  void setFormatter(const std::string &val);
  LogFormatter::ptr getFormatter();
  std::string toYamlString();

private:
  std::string m_name;
  LogLevel::Level m_level;
  std::list<LogAppender::ptr> m_appenders;
  LogFormatter::ptr m_formatter;
  Logger::ptr m_root;
};

class StdoutLogAppender : public LogAppender {
public:
  typedef std::shared_ptr<StdoutLogAppender> ptr;
  void log(Logger::ptr logger, LogLevel::Level level,
           LogEvent::ptr event) override;
  std::string toYamlString() override;
};

class FileLogAppender : public LogAppender {
public:
  typedef std::shared_ptr<FileLogAppender> ptr;
  FileLogAppender(const std::string &filename);
  void log(Logger::ptr logger, LogLevel::Level level,
           LogEvent::ptr event) override;
  std::string toYamlString() override;

  bool reopen();

private:
  std::string m_filename;
  std::ofstream m_filestream;
  uint64_t m_lastTime = 0;
};

class LoggerManager {
public:
  LoggerManager();
  Logger::ptr getLogger(const std::string &name);
  void init();
  Logger::ptr getRoot() const { return m_root; }
  std::string toYamlString();

private:
  std::map<std::string, Logger::ptr> m_loggers;
  Logger::ptr m_root;
};

typedef PENG::Singleton<LoggerManager> LoggerMgr;

} // namespace PENG

#endif