#pragma once

#include <string>
#include <map>
#include <iostream>

using namespace std;

namespace yazi
{
    namespace util
    {
        class Value
        {
        public:
            Value();
            Value(bool value);
            Value(int value);
            Value(double value);
            Value(const string &value);
            ~Value();

            template <typename T>
            Value &operator=(const T &value);
            Value &operator=(bool value);

            operator bool();
            operator int();
            operator double();
            // non-const obj is able to call const func
            operator string() const;

        private:
            string m_value;
        };

        class IniFile
        {
        public:
            IniFile();
            IniFile(const string &filename);
            ~IniFile();

            bool load(const string &filename);
            void save(const string &filename);
            void show();
            void clear();

            map<string, Value> &operator[](const string &key)
            {
                return m_inifile[key];
            }

        private:
            string trim(string s);

        private:
            string m_filename;
            map<string, map<string, Value>> m_inifile;
        };
    };
}