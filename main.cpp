#include "server/Server.h"
#include "util/IniFile.h"
#include "util/Logger.h"
#include "util/Singleton.h"
#include "util/System.h"

using namespace yazi::util;
using namespace yazi::server;

int main() {
    System *sys = Singleton<System>::instance();
    sys->init();

    return 0;
}