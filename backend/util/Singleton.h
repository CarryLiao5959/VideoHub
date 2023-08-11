#pragma once

namespace yazi {
namespace util {

template <typename T> class Singleton {
  public:
    template <typename... Args>
    static T *instance(Args... args) {
        if (m_instance == nullptr) {
            m_instance = new T(std::forward<Args>(args)...);
        }
        return m_instance;
    }

    static void destroy() {
        delete m_instance;
        m_instance = nullptr;
    }

  private:
    Singleton() {}
    ~Singleton() {}
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

  private:
    static T *m_instance;
};

template <typename T> T *Singleton<T>::m_instance = nullptr;

} // namespace util
} // namespace yazi