#include "IniFile.h"
#include <fstream>
#include <sstream>

using namespace yazi::util;
using namespace std;

Value::Value() {}
Value::Value(bool value) { *this = value; }
Value::Value(int value) { *this = value; }
Value::Value(double value) { *this = value; }
Value::Value(const string &value) : m_value(value) {}
Value::~Value() {}

template <typename T> Value &Value::operator=(const T &value) {
    std::ostringstream os;
    os << value;
    m_value = os.str();
    return *this;
}

Value &Value::operator=(bool value) {
    m_value = value ? "true" : "false";
    return *this;
}

Value::operator bool() { return m_value == "true" ? true : false; }

Value::operator int() { return std::atoi(m_value.c_str()); }

Value::operator double() { return std::atof(m_value.c_str()); }

Value::operator string() const { return m_value; }

IniFile::IniFile() {}

IniFile::IniFile(const string &filename) { load(filename); }

IniFile::~IniFile() {}

bool IniFile::load(const string &filename) {
    m_filename = filename;
    m_inifile.clear();

    string name;
    string line;
    // open the INI file for reading
    ifstream fin(filename.c_str());
    if (fin.fail()) {
        printf("loading file failed: %s is not found.\n", m_filename.c_str());
        return false;
    }
    while (std::getline(fin, line)) {
        line = trim(line);
        if ('[' == line[0]) // it is a section
        {
            int pos = line.find_first_of(']');
            if (-1 != pos) {
                name = trim(line.substr(1, pos - 1));
                // create a new [k,v] = [name，map]
                m_inifile[name];
            }
        } else if ('#' == line[0]) // it is a comment
        {
            continue;
        } else // it is the "key=value" line
        {
            int pos = line.find_first_of('=');
            if (pos > 0) {
                // add new key to the last section in the storage
                string key = trim(line.substr(0, pos));
                string value =
                    trim(line.substr(pos + 1, line.size() - pos - 1));
                map<string, map<string, Value>>::iterator it =
                    m_inifile.find(name);
                if (it == m_inifile.end()) {
                    printf("parsing error: section=%s key=%s\n", name.c_str(),
                           key.c_str());
                    return false;
                }
                m_inifile[name][key] = value;
            }
        }
    }
    return true;
}

void IniFile::show() {
    std::map<string, map<string, Value>>::iterator it;
    for (it = m_inifile.begin(); it != m_inifile.end(); ++it) {
        // write section line
        cout << "[" << it->first << "]" << endl;
        map<string, Value>::iterator iter;
        for (iter = it->second.begin(); iter != it->second.end(); ++iter) {
            // write "key = value" line
            cout << iter->first << " = " << (string)iter->second << endl;
        }
        cout << endl;
    }
}

void IniFile::clear() { m_inifile.clear(); }

string IniFile::trim(string s) {
    if (s.empty()) {
        return s;
    }
    s.erase(0, s.find_first_not_of(" \r\n"));
    s.erase(s.find_last_not_of(" \r\n") + 1);
    return s;
}
