#pragma once

#include "Mutex.h"

namespace yazi {
namespace thread {

class AutoLock {
  public:
    AutoLock(Mutex *mutex) {
        m_mutex = mutex;
        m_mutex->lock();
    }
    ~AutoLock() { m_mutex->unlock(); }

  private:
    Mutex *m_mutex;
};

} // namespace thread
} // namespace yazi
