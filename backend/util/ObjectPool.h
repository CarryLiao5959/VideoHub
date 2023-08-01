#pragma once
#include <list>
#include "AutoLock.h"
#include "TaskDispatcher.h"
using namespace yazi::thread;
using namespace std;
namespace yazi {
namespace util {

template <typename T>
class ObjectPool {
  public:
    ObjectPool() {}

    ~ObjectPool() {
        AutoLock lock(&m_mutex);
        for (auto it = m_pool.begin(); it != m_pool.end(); it++) {
            if ((*it) != nullptr) {
                delete (*it);
            }
        }
        m_pool.clear();
    }

    void init(int n) {
        AutoLock lock(&m_mutex);
        for (int i = 0; i < n; i++) {
            T *p = new T();
            m_pool.push_back(p);
        }
    }

    T *get() {
        AutoLock lock(&m_mutex);
        if (m_pool.size() == 0) {
            return nullptr;
        }
        T *p = m_pool.front();
        m_pool.pop_front();
        return p;
    }

    void put(T *p) {
        AutoLock lock(&m_mutex);
        m_pool.push_back(p);
    }

  private:
    list<T *> m_pool;
    Mutex m_mutex;
};
} // namespace util
} // namespace yazi