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
#include "WebSocket.h"

using namespace std;

int main() {
    System *sys = Singleton<System>::instance();
    sys->init();

    WebSocket* ws=new WebSocket();
    ws->run_ws_server(8081);

    return 0;
}