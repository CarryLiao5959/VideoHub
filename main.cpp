#include "System.h"

#include "Singleton.h"
#include "Logger.h"
using namespace yazi::util;
#include "TaskDispatcher.h"
using namespace yazi::thread;
#include "EchoTask.h"
using namespace yazi::task;
#include "ClientSocket.h"
using namespace yazi::socket;

using namespace std;

int main() {
    System *sys = Singleton<System>::instance();
    sys->init();

    return 0;
}