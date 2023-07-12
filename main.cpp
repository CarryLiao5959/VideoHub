#include "server/Server.h"
#include "util/Singleton.h"
#include "util/IniFile.h"
#include "util/System.h"

using namespace yazi::util;
using namespace yazi::server;

int main()
{
    System *sys = Singleton<System>::instance();
    sys->init();

    IniFile *ini = Singleton<IniFile>::instance();
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

    return 0;
}