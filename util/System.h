#pragma once

#include <string>
using namespace std;

namespace yazi
{
    namespace util
    {

        class System
        {
        public:
            System();
            ~System();

            void init();
            string get_root_path();

        private:
            string m_root_path;
        };
    }
}
