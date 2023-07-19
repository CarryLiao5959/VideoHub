#pragma once
#include "IniFile.h"
#include "Logger.h"
#include "Server.h"
#include "Singleton.h"
#include "System.h"
#include <dirent.h>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <unistd.h>
using namespace yazi::util;
using namespace yazi::server;
using namespace std;
namespace yazi {
namespace util {
class System {
  private:
    string m_root_path;

  public:
    System() {}
    ~System() {}
    void init() {
        m_root_path = get_root_path();

        Logger::instance()->open(m_root_path + "/log/main.log");
        info("init logger");

        IniFile *ini = Singleton<IniFile>::instance();
        ini->load(get_root_path() + "/config/main.ini");
        info("init inifile");

        const string &ip = (*ini)["server"]["ip"];
        int port = (*ini)["server"]["port"];
        int threads = (*ini)["server"]["threads"];
        int max_conn = (*ini)["server"]["max_conn"];
        int wait_time = (*ini)["server"]["wait_time"];
        (*ini).show();

        Server *server = Singleton<Server>::instance();
        server->set_threads(threads);
        server->set_connects(max_conn);
        server->set_wait_time(wait_time);
        server->listen(ip, port);
        server->start();
    }

    string get_root_path() {
        if (m_root_path != "") {
            return m_root_path;
        }
        char path[1024];
        memset(path, 0, 1024);
        int cnt = readlink("/proc/self/exe", path, 1024);
        debug("[Running Proc] %s", path);
        if (cnt < 0 || cnt >= 1024) {
            return "";
        }
        for (int i = cnt; i >= 0; --i) {
            if (path[i] == '/') {
                path[i] = '\0';
                break;
            }
        }
        return string(path);
    }
};
} // namespace util
} // namespace yazi
