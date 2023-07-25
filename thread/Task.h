#pragma once

#include "Mutex.h"

namespace yazi {
namespace thread {
class Task {
  public:
    Task();
    Task(void *data);
    virtual ~Task();

    void run();
    void destroy();

  protected:
    void *m_data;
};
} // namespace thread
} // namespace yazi
