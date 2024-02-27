#include "util.h"
#include "fiber.h"
#include "log.h"
#include <execinfo.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

static PENG::Logger::ptr g_logger = PENG_LOG_NAME("system");

namespace PENG {

pid_t GetThreadId() { return syscall(SYS_gettid); }

uint32_t GetFiberId() { return PENG::Fiber::GetFiberId(); }

void Backtrace(std::vector<std::string> &bt, int size, int skip) {
  void **array = (void **)malloc((sizeof(void *) * size));
  size_t s = ::backtrace(array, size);

  char **strings = backtrace_symbols(array, s);
  if (strings == NULL) {
    PENG_LOG_ERROR(g_logger) << "backtrace_symbols error";
  }
  for (size_t i = skip; i < s; ++i) {
    bt.push_back(strings[i]);
  }
  free(strings);
  free(array);
}

std::string BacktraceToString(int size, int skip, const std::string &prefix) {
  std::vector<std::string> bt;
  Backtrace(bt, size, skip);
  std::stringstream ss;
  for (size_t i = 0; i < bt.size(); ++i) {
    ss << prefix << bt[i] << std::endl;
  }
  return ss.str();
}

uint64_t GetCurrentMS() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

uint64_t GetCurrentUS() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 * 1000ul + tv.tv_usec;
}

static int __lstat(const char *file, struct stat *st = nullptr) {
  struct stat lst;
  int ret = lstat(file, &lst);
  if (st) {
    *st = lst;
  }
  return ret;
}

bool FSUtil::Unlink(const std::string &filename, bool exist) {
  if (!exist && __lstat(filename.c_str())) {
    return true;
  }
  return ::unlink(filename.c_str()) == 0;
}

} // namespace PENG
