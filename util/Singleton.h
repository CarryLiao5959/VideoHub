#pragma once

namespace yazi
{
    namespace util
    {
        template <typename T>
        class Singleton
        {
        public:
            static T *instance()
            {
                if (m_instance == nullptr)
                {
                    m_instance = new T();
                }
                return m_instance;
            }

        private:
            Singleton() {}
            ~Singleton() {}
            Singleton(const Singleton &) = delete;
            Singleton &operator=(const Singleton &) = delete;

        private:
            static T *m_instance;
        };

        template <typename T>
        T *Singleton<T>::m_instance = nullptr;
    }
}