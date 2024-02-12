#include "util.h"
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

namespace PENG {

pid_t GetThreadId() { return syscall(SYS_gettid); }

uint32_t GetFiberId() { return 0; }

} // namespace PENG
