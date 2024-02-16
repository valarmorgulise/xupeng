#ifndef __PENG_SCHEDULER_H__
#define __PENG_SCHEDULER_H__

#include "fiber.h"
#include "mutex.h"
#include "thread.h"
#include <list>
#include <map>
#include <memory>

namespace PENG {

class Scheduler {
public:
  typedef std::shared_ptr<Scheduler> ptr;
  typedef Mutex MutexType;

  Scheduler(size_t threads = 1, bool use_caller = true,
            const std::string &name = "");
  virtual ~Scheduler();

  const std::string &getName() const { return m_name; }

  static Scheduler *GetThis();
  static Fiber *GetMainFiber();

  void start();
  void stop();

  template <class FiberOrCb> void Schedule(FiberOrCb fc, int thread_id = -1) {
    bool need_tickle = false;
    {
      MutexType::Lock lock(m_mutex);
      need_tickle = scheduleNoLock(fc, thread_id);
    }

    if (need_tickle) {
      tickle();
    }
  }

  template <class InputIterator>
  void schedule(InputIterator begin, InputIterator end) {
    bool need_tickle = false;
    {
      MutexType::Lock lock(m_mutex);
      while (begin != end) {
        need_tickle = scheduleNoLock(&*begin) || need_tickle;
      }
      if (need_tickle) {
        tickle();
      }
    }
  }

protected:
  virtual void tickle();
  virtual void run();
  virtual bool stopping();
  virtual void idle();

  void setThis();

private:
  template <class FiberOrCb> bool scheduleNoLock(FiberOrCb fc, int thread_id) {
    bool need_tickle = m_fibers.empty();
    FiberAndThread ft(fc, thread_id);
    if (ft.fiber || ft.cb) {
      m_fibers.push_back(ft);
    }
    return need_tickle;
  }

private:
  struct FiberAndThread {
    Fiber::ptr fiber;
    std::function<void()> cb;
    int thread_id;

    FiberAndThread(Fiber::ptr f, int thr) : fiber(f), thread_id(thr) {}

    FiberAndThread(Fiber::ptr *f, int thr) : thread_id(thr) { fiber.swap(*f); }

    FiberAndThread(std::function<void()> f, int thr) : cb(f), thread_id(thr) {}

    FiberAndThread(std::function<void()> *f, int thr) : thread_id(thr) {
      cb.swap(*f);
    }

    FiberAndThread() : thread_id(-1) {}

    void reset() {
      fiber = nullptr;
      cb = nullptr;
      thread_id = -1;
    }
  };

protected:
  std::vector<int> m_threadIds;
  size_t m_threadCount = 0;
  std::atomic<size_t> m_activeThreadCount = {0};
  std::atomic<size_t> m_idleThreadCount = {0};
  bool m_stopping = true;
  bool m_autoStop = false;
  // 主线程id
  int m_rootThread = 0;

private:
  MutexType m_mutex;
  std::vector<Thread::ptr> m_threads;
  std::list<FiberAndThread> m_fibers;
  std::map<int, std::list<FiberAndThread>> m_thrFibers;
  Fiber::ptr m_rootFiber;
  std::string m_name;
};

} // namespace PENG

#endif // !__PENG_SCHEDULER_H__
