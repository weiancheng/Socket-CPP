#include "TCPClient.h"
#include <iostream>

int main(int argc, char **argv) {
    TCPClient client;

    client.start("127.0.0.1", 6666);
    client.stop();

    return 0;
}
