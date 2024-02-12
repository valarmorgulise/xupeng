#ifndef __PENG_MUTEX_H
#define __PENG_MUTEX_H

#include "noncopyable.h"
#include <cstdint>
#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <thread>

namespace PENG {
class Semaphore : Noncopyable {
public:
  Semaphore(uint32_t count = 0);
  ~Semaphore();
  void wait();
  void notify(); // 释放信号量
private:
  sem_t m_semaphore;
};
} // namespace PENG

#endif // !__PENG_MUTEX_H
