#include "TCPServer.h"
#include <iostream>

int main(int argc, char **argv) {
    TCPServer server;
    server.start(6666);
    server.waitForClient();
    server.stop();
    return 0;
}