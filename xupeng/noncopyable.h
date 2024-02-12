#ifndef __PENG_NONCOPYABLE_H
#define __PENG_NONCOPYABLE_H

namespace PENG {
class Noncopyable {
public:
  Noncopyable() = default;
  ~Noncopyable() = default;
  Noncopyable(const Noncopyable &) = delete;
  Noncopyable &operator=(const Noncopyable &) = delete;
};
} // namespace PENG

#endif
