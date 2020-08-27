#include "TCPServer.h"

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <pthread.h>

#define SERVER_PORT 6666
#define BUFFER_SIZE 4096

using namespace std;


bool isThreadExited = false;


void *ServerThreadMain(void *arg) {
    TCPServer server;
    FILE *filp = nullptr;
    char buffer[BUFFER_SIZE];

    isThreadExited = false;

    while (1) {

        if (isThreadExited) {
            printf("stop server thread...\n");
            break;
        }

        filp = fopen("DRM.jpg", "r");
        if (filp == nullptr) {
            printf("Error, failed to open file...\n");
            break;
        }

        server.start(SERVER_PORT);

        int clientfd = server.waitForClient();

        while (1) {
            if (isThreadExited) {
                printf("stop server thread...\n");
                break;
            }

            int length = fread(buffer, sizeof(char), BUFFER_SIZE, filp);
            if (length == 0) {
                server.sendEOFToClient(clientfd);
                printf("server eof\n");
                break;
            }

            int n = server.sendToClient(clientfd, buffer, length);
            printf("sent length: %d\n", n);
        }

        server.stop();
        fclose(filp);
    }

    printf("exit server thread...\n");
}


int main(int argc, char **argv) {
    pthread_t server_thread;

    int status = pthread_create(&server_thread, nullptr, &ServerThreadMain, nullptr);
    if (status != 0) {
        printf("failed to create server thread, status: %d", status);
        return -1;
    }

    pthread_join(server_thread, nullptr);

    return 0;
}