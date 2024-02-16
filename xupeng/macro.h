#ifndef __PENG_MACRO_H_
#define __PENG_MACRO_H_

#include "util.h"
#include <assert.h>

#if defined __GNUC__ || defined __llvm__
// LIKCLY 宏的封装， 告诉编译器优化，条件大概率成立
#define PENG_LIKELY(x) __builtin_expect(!!(x), 1)
// UNLIKCLY 宏的封装， 告诉编译器优化，条件大概率不成立
#define PENG_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define PENG_LIKELY(x) (x)
#define PENG_UNLIKELY(x) (x)
#endif

// 断言宏封装
#define PENG_ASSERT(x)                                                         \
  if (PENG_UNLIKELY(!(x))) {                                                   \
    PENG_LOG_ERROR(PENG_LOG_ROOT())                                            \
        << "ASSERTION: " #x << "\nbacktrace:\n"                                \
        << PENG::BacktraceToString(100, 2, "     ");                           \
    assert(x);                                                                 \
  }

#define PENG_ASSERT2(x, w)                                                     \
  if (PENG_UNLIKELY(!(x))) {                                                   \
    PENG_LOG_ERROR(PENG_LOG_ROOT())                                            \
        << "ASSERTION: " #x << "\n"                                            \
        << w << "\nbacktrace:\n"                                               \
        << PENG::BacktraceToString(100, 2, "     ");                           \
    assert(x);                                                                 \
  }

#endif // !__PENG_LOG_H__
