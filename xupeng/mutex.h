#ifndef __PENG_MUTEX_H
#define __PENG_MUTEX_H

#include "noncopyable.h"
#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <pthread.h>
#include <semaphore.h>
#include <thread>

namespace PENG {
/**
 * @brief 信号量
 */
class Semaphore : Noncopyable {
public:
  /**
   * @brief 构造函数
   * @Param[in] count 信号量值的大小
   */
  Semaphore(uint32_t count = 0);
  ~Semaphore();
  /**
   * @brief 获取信号量
   */
  void wait();
  /**
   * @brief 释放信号量
   */
  void notify();

private:
  sem_t m_semaphore;
};

/**
 * @brief 局部锁的模板实现
 */
template <class T> struct ScopedLockImpl {
public:
  /**
   * @brief 构造函数
   * @param[in] mutex Mutex
   */
  ScopedLockImpl(T &mutex) : m_mutex(mutex) {
    m_mutex.lock();
    m_locked = true;
  }
  /**
   * @brief 析构函数, 自动释放锁
   */
  ~ScopedLockImpl() { unlock(); }

  /**
   * @brief 加锁
   */
  void lock() {
    if (!m_locked) {
      m_mutex.lock();
      m_locked = true;
    }
  }

  /**
   * @brief 解锁
   */
  void unlock() {
    if (m_locked) {
      m_mutex.unlock();
      m_locked = false;
    }
  }

private:
  // mutex
  T &m_mutex;
  // 是否已上锁
  bool m_locked;
};

/**
 * @brief 局部读锁的模板实现
 */
template <class T> struct ReadScopedLockImpl {
public:
  /**
   * @brief 构造函数
   * @param[in] mutex Mutex
   */
  ReadScopedLockImpl(T &mutex) : m_mutex(mutex) {
    m_mutex.rdlock();
    m_locked = true;
  }

  /**
   * @brief 析构函数, 自动释放锁
   */
  ~ReadScopedLockImpl() { unlock(); }

  /**
   * @brief 加锁
   */
  void lock() {
    if (!m_locked) {
      m_mutex.rdlock();
      m_locked = true;
    }
  }

  void unlock() {
    if (m_locked) {
      m_mutex.unlock();
      m_locked = false;
    }
  }

private:
  // mutex
  T &m_mutex;
  // 是否已上锁
  bool m_locked;
};

/**
 * @brief 局部写锁的模板实现
 */
template <class T> struct WriteScopedLockImpl {
public:
  WriteScopedLockImpl(T &mutex) : m_mutex(mutex) {
    m_mutex.rdlock();
    m_locked = true;
  }
  ~WriteScopedLockImpl() { unlock(); }

  void lock() {
    if (!m_locked) {
      m_mutex.rdlock();
      m_locked = true;
    }
  }

  void unlock() {
    if (m_locked) {
      m_mutex.unlock();
      m_locked = false;
    }
  }

private:
  T &m_mutex;
  bool m_locked;
};

/**
 * @brief 互斥量
 */
class Mutex : Noncopyable {
public:
  // 局部锁
  typedef ScopedLockImpl<Mutex> Lock;

  Mutex() { pthread_mutex_init(&m_mutex, nullptr); }

  ~Mutex() { pthread_mutex_destroy(&m_mutex); }

  void lock() { pthread_mutex_lock(&m_mutex); }

  void unlock() { pthread_mutex_unlock(&m_mutex); }

private:
  pthread_mutex_t m_mutex;
};

/**
 * @brief 空锁(用于调试)
 */
class NullMutex : Noncopyable {
public:
  typedef ScopedLockImpl<NullMutex> Lock;
  NullMutex() {}
  ~NullMutex() {}
  void lock() {}
  void unlock() {}
};

/**
 * @brief 读写互斥量
 */
class RWMutex : Noncopyable {
public:
  // 局部读锁
  typedef ReadScopedLockImpl<RWMutex> ReadLock;
  // 局部写锁
  typedef WriteScopedLockImpl<RWMutex> WriteLock;

  RWMutex() { pthread_rwlock_init(&m_lock, nullptr); }

  ~RWMutex() { pthread_rwlock_destroy(&m_lock); }

  // 上读锁
  void rdlock() { pthread_rwlock_rdlock(&m_lock); }

  // 上写锁
  void wrlock() { pthread_rwlock_wrlock(&m_lock); }

  // 解锁
  void unlock() { pthread_rwlock_unlock(&m_lock); }

private:
  pthread_rwlock_t m_lock;
};

class NullRWMutex : Noncopyable {
public:
  typedef ReadScopedLockImpl<NullRWMutex> ReadLock;
  typedef WriteScopedLockImpl<NullRWMutex> WriteLock;

  NullRWMutex() {}
  ~NullRWMutex() {}
  void rdlock() {}
  void wdlock() {}
  void unlock() {}
};

/**
 * @brief 读写锁
 */
class Spinlock : Noncopyable {
public:
  typedef ScopedLockImpl<Spinlock> Lock;
  Spinlock() { pthread_spin_init(&m_mutex, 0); }
  ~Spinlock() { pthread_spin_destroy(&m_mutex); }
  void lock() { pthread_spin_lock(&m_mutex); }

  void unlock() { pthread_spin_unlock(&m_mutex); }

private:
  pthread_spinlock_t m_mutex;
};

/**
 * @brief 原子锁
 */
class CASLock : Noncopyable {
public:
  typedef ScopedLockImpl<CASLock> Lock;
  CASLock() { m_mutex.clear(); }
  ~CASLock() {}
  void lock() {
    while (std::atomic_flag_test_and_set_explicit(&m_mutex,
                                                  std::memory_order_release))
      ;
  }
  void unlock() {
    std::atomic_flag_clear_explicit(&m_mutex, std::memory_order_release);
  }

private:
  volatile std::atomic_flag m_mutex;
};

} // namespace PENG

#endif // !__PENG_MUTEX_H
