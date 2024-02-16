#include "scheduler.h"
#include "fiber.h"
#include "log.h"
#include "macro.h"
#include "util.h"
#include <string>

namespace PENG {

static PENG::Logger::ptr g_logger = PENG_LOG_NAME("system");

static thread_local Scheduler *t_scheduler = nullptr;
static thread_local Fiber *t_fiber = nullptr;

Scheduler::Scheduler(size_t threads, bool use_caller, const std::string &name)
    : m_name(name) {
  PENG_ASSERT(threads > 0);
  if (use_caller) {
    PENG::Fiber::GetThis();
    --threads;
    PENG_ASSERT(GetThis() == nullptr);
    t_scheduler = this;
    m_rootFiber.reset(new Fiber(std::bind(&Scheduler::run, this)));
    PENG::Thread::SetName(m_name);

    t_fiber = m_rootFiber.get();
    m_rootThread = PENG::GetFiberId();
    m_threadIds.push_back(m_rootThread);
  } else {
    m_rootThread = -1;
  }
  m_threadCount = threads;
}

Scheduler::~Scheduler() {
  PENG_ASSERT(m_stopping);
  if (GetThis() == this) {
    t_scheduler = nullptr;
  }
}

Scheduler *Scheduler::GetThis() { return t_scheduler; }

Fiber *Scheduler::GetMainFiber() { return t_fiber; }

void Scheduler::start() {
  MutexType::Lock lock(m_mutex);
  if (!m_stopping) {
    return;
  }
  m_stopping = false;
  PENG_ASSERT(m_threads.empty());
  m_threads.resize(m_threadCount);

  for (size_t i = 0; i < m_threadCount; ++i) {
    m_threads[i].reset(new Thread(std::bind(&Scheduler::run, this),
                                  m_name + "_" + std::to_string(i)));
    m_threadIds.push_back(m_threads[i]->getId());
  }
}

void Scheduler::stop() {
  m_autoStop = true;
  if (m_rootFiber && m_threadCount == 0 &&
      (m_rootFiber->getState() == Fiber::TERM ||
       m_rootFiber->getState() == Fiber::INIT)) {
    PENG_LOG_INFO(g_logger) << this << " stopped";
    m_stopping = true;
    if (stopping()) {
      return;
    }
  }
  bool exit_on_this_fiber = false;
  if (m_rootThread != -1) {
    PENG_ASSERT(GetThis() == this);
  } else {
    PENG_ASSERT(GetThis() != this);
  }
  m_stopping = true;
  for (size_t i = 0; i < m_threadCount; ++i) {
    tickle();
  }

  if (m_rootFiber) {
    tickle();
  }

  if (stopping()) {
    return;
  }

  // if (exit_on_this_fiber) {
  //
  // }
}

void Scheduler::setThis() { t_scheduler = this; }

void Scheduler::run() {
  setThis();
  if (PENG::GetThreadId() != m_rootThread) {
    t_fiber = Fiber::GetThis().get();
  }
  Fiber::ptr idle_fiber(new Fiber(std::bind(&Scheduler::idle, this)));
  Fiber::ptr cb_fiber;

  FiberAndThread ft;
  while (true) {
    ft.reset();
    bool tickle_me = false;
    {
      MutexType::Lock lock(m_mutex);
      auto it = m_fibers.begin();
      while (it != m_fibers.end()) {
        if (it->thread_id != -1 && it->thread_id != PENG::GetThreadId()) {
          ++it;
          tickle_me = true;
          continue;
        }
        PENG_ASSERT(it->fiber || it->cb);
        if (it->fiber && it->fiber->getState() == Fiber::EXEC) {
          ++it;
          continue;
        }
        ft = *it;
        m_fibers.erase(it);
      }
    }
    if (tickle_me) {
      tickle();
    }
    if (ft.fiber && (ft.fiber->getState() != Fiber::TERM) {
      ft.fiber->swapIn();
      if (ft.fiber->getState() == Fiber::READY) {
        schedule(ft.fiber);
      } else if (ft.fiber->getState() != Fiber::TERM &&
                 ft.fiber->getState() != Fiber::EXCEPT) {
        ft.fiber->m_state = Fiber::HOLD;
      }
      ft.reset();
    } else if (ft.cb) {
      if (cb_fiber) {
        cb_fiber->reset(&ft.cb);
      } else {
        cb_fiber.reset(new Fiber(ft.cb));
      }
      ft.reset();
      cb_fiber->swapIn();
      if (cb_fiber->getState() == Fiber::READY) {
        schedule(cb_fiber);
        cb_fiber.reset();
      } else if (cb_fiber->getState() == Fiber::EXCEPT ||
                 cb_fiber->getState() == Fiber::TERM) {
        cb_fiber->reset(nullptr);
      } else /*if (cb_fiber->getState() != Fiber::TERM)*/ {
        cb_fiber->m_state = Fiber::HOLD;
        cb_fiber.reset();
        ++m_activeThreadCount;
        cb_fiber->swapIn();
        --m_activeThreadCount;
      }
    } else {
      if (idle_fiber->getState() == Fiber::TERM) {
        break;
      }

      ++m_idleThreadCount;
      idle_fiber->swapIn();
      if (idle_fiber->getState() != Fiber::TERM ||
          idle_fiber->getState() != Fiber::EXCEPT) {
        idle_fiber->m_state = Fiber::HOLD;
      }
      --m_idleThreadCount;
    }
  }
}

} // namespace PENG
