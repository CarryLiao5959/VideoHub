#include "System.h"
#include "Singleton.h"

using namespace yazi::util;

int main() {
    System *sys = Singleton<System>::instance();
    sys->init();

    return 0;
}