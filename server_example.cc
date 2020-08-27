#include "TCPServer.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <cstring>

#define SERVER_PORT 6666
#define BUFFER_SIZE 4096

using namespace std;


int main(int argc, char **argv) {
    TCPServer server;
    FILE *filp = nullptr;
    char buffer[BUFFER_SIZE];
    string eof("");

    filp = fopen("test.jpg", "r");
    if (filp == nullptr) {
        printf("Error, failed to open file...\n");
        return -1;
    }

    server.start(SERVER_PORT);

    int clientfd = server.waitForClient();

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int length = fread(buffer, sizeof(char), BUFFER_SIZE, filp);
        if (length == 0) {
            server.sendToClient(clientfd, eof.c_str(), eof.size());
            break;
        }

        server.sendToClient(clientfd, buffer, length);
    }

    server.stop();
    fclose(filp);

    return 0;
}