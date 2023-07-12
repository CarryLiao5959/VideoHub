#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "System.h"
#include "Singleton.h"
#include "IniFile.h"
using namespace yazi::util;

System::System() {}
System::~System() {}

void System::init()
{
    m_root_path = get_root_path();

    // init inifile
    IniFile *ini = Singleton<IniFile>::instance();
    ini->load(get_root_path() + "/config/main.ini");
}

// get path of current running proc
string System::get_root_path()
{
    if (m_root_path != "")
    {
        return m_root_path;
    }
    char path[1024];
    memset(path, 0, 1024);
    // "/proc/self/exe" point to the executable file of the current running proc
    int cnt = readlink("/proc/self/exe", path, 1024);
    cout << "[Running Proc]" << path << endl;
    if (cnt < 0 || cnt >= 1024)
    {
        return "";
    }
    for (int i = cnt; i >= 0; --i)
    {
        // file path -> directory path
        if (path[i] == '/')
        {
            path[i] = '\0';
            break;
        }
    }
    return string(path);
}